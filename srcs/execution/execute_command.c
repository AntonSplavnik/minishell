/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:44:50 by abillote          #+#    #+#             */
/*   Updated: 2025/02/16 08:20:28 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Handles execution in child process:
- Sets up signal handling for child
- Attempts to execute the command using execve
- If execve fails, exits with code 127 (command not found)
- Command path and arguments are freed by parent process
Parameters:
  - cmd_path: Full path to executable
  - args: Array of command arguments
  - s: Shell structure containing environment
*/
static void	handle_child_process(char *cmd_path, char **args, t_shell *s)
{
	if (set_signals_child() != 0)
		exit(1);
	execve(cmd_path, args, s->envp);
	s->exit_status = 127;
	exit(127);
}

/*
Handles parent process after fork:
- Waits for child process to complete using WUNTRACED flag
- Continues waiting if process was stopped but not terminated
- Handles cleanup of command resources
- Processes exit status and signals
Parameters:
  - pid: Process ID of child
  - cmd_path: Command path to free
  - s: Shell structure for status updates
Returns:
  - SUCCESS or ERR_SIGNAL if signal handling fails
*/
static t_error handle_parent_process(pid_t pid, char *cmd_path, t_shell *s)
{
	int	 status;
	pid_t   wait_result;
	int	 process_completed;

	process_completed = 0;
	while (!process_completed)
	{
		wait_result = waitpid(pid, &status, WUNTRACED);
		if (wait_result == -1)
		{
			free(cmd_path);
			s->exit_status = 1;
			return (ERR_EXEC);
		}
		if (WIFEXITED(status) || WIFSIGNALED(status))
			process_completed = 1;
	}
	free(cmd_path);

	// Handle signal-based termination
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			write(2, "Quit (core dumped)\n", 18);
			usleep(500000);
		}
		else if (WTERMSIG(status) == SIGINT)
		{
			write(2, "\n", 1);
			usleep(10000);
		}
		rl_on_new_line();
		rl_replace_line("", 0);
		s->exit_status = 128 + WTERMSIG(status);
	}
	// Handle normal process termination
	else if (WIFEXITED(status))
		s->exit_status = WEXITSTATUS(status);

	// Restore interactive signal handling
	if (set_signals_interactive())
		return (ERR_SIGNAL);

	return (SUCCESS);
}

/*
Main execution function that manages the fork process:
- Sets up parent signal handling
- Creates child process with fork
- Routes execution to appropriate handler based on fork result
- Handles resource cleanup on errors
Parameters:
  - cmd_path: Full path to executable
  - args: Array of command arguments
  - s: Shell structure containing environment
Returns:
  - SUCCESS on successful execution
  - ERR_SIGNAL for signal handling failures
  - ERR_EXEC for fork failures
*/
t_error	execute_child_process(char *cmd_path, char **args, t_shell *s)
{
	pid_t	pid;

	if (set_signals_parent() != 0)
	{
		free(cmd_path);
		free_array(args);
		s->exit_status = 1;
		return (ERR_SIGNAL);
	}
	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		free_array(args);
		s->exit_status = 1;
		return (ERR_EXEC);
	}
	if (pid == 0)
		handle_child_process(cmd_path, args, s);
	else
		return (handle_parent_process(pid, cmd_path, s));
	return (SUCCESS);
}

/*
Executes a single command with its arguments
Find the args by going through the token list (stop at the first pipe)
- If the first token of the list is a built-in,
go to the function execute_built-in
- Else, find the command path with the first token of the list
RETURNS: appropriate error code
To add:
- handling pipes
*/
t_error	execute_command(t_token *cmd_token, t_shell *s)
{
	char	*cmd_path;
	char	**args;
	t_error	result;

	/*Check if there is a pipe in the token list
	If yes, return execute_pipe (maybe create a command struct
	to store all commands in a linked list?)
	Notes:
	- When pipe failure, s->exit status should be 1
	- Remember to also implement signals in pipe execution
	*/
	args = prepare_command_args(cmd_token);
	if (!args)
	{
		s->exit_status = 1;
		return (ERR_MALLOC);
	}
	if (is_built_in(cmd_token->content))
		result = execute_built_in(cmd_token, args, s);
	else
	{
		cmd_path = find_command_path(cmd_token->content, s);
		if (!cmd_path)
			result = ERR_CMD_NOT_FOUND;
		else
			result = execute_child_process(cmd_path, args, s);
	}
	free_array(args);
	return (result);
}
