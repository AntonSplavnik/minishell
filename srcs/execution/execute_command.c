/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:44:50 by abillote          #+#    #+#             */
/*   Updated: 2025/02/28 14:40:10 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	{
		free(cmd_path);
		free_array(args);
		exit(1);
	}
	execve(cmd_path, args, s->envp);
	free(cmd_path);
	free_array(args);
	s->exit_status = 127;
	exit(127);
}

/*
Handles parent process after fork:
- Waits for child process to complete
- Handles cleanup of command resources
- Processes exit status and signals
- Add delays to restore the prompt for signals otherwise process keeps writing
	- WIFEXITED(status) -> check that child process
ended up normally (not killed by a signal)
	- WEXITSTATUS(status) : get the child exit code
- Restores interactive signal handling
Parameters:
  - pid: Process ID of child
  - cmd_path: Command path to free
  - s: Shell structure for status updates
Returns:
  - SUCCESS or ERR_SIGNAL if signal handling fails
*/
static t_error handle_parent_process(pid_t pid, char *cmd_path, t_shell *s)
{
	int	status;

	waitpid(pid, &status, 0);
	free(cmd_path);
	handle_exit_status(status, s);
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
t_error execute_child_process(char *cmd_path, char **args, t_shell *s)
{
	pid_t	pid;

	if (set_signals_parent() != 0)
		return (free_resources(cmd_path, s, ERR_SIGNAL));
	pid = fork();
	if (pid == -1)
		return (free_resources(cmd_path, s, ERR_EXEC));
	if (pid == 0)
		handle_child_process(cmd_path, args, s);
	else
		return (handle_parent_process(pid, cmd_path, s));
	return (SUCCESS);
}

static t_error execute_external_command(t_token *cmd, char **args, t_shell *s)
{
	char *cmd_path;
	t_error result;

	cmd_path = ind_command_path(cmd->content, s);
	if (!cmd_path)
		return (ERR_CMD_NOT_FOUND);
	result = execute_child_process(cmd_path, args, s);
	return (result);
}

/*
Executes pipes.
Executes redirections.
Executes a single command with its arguments
Find the args by going through the token list (stop at the first pipe)
- If the first token of the list is a built-in,
go to the function execute_built-in
- Else, find the command path with the first token of the list
RETURNS: appropriate error code
To add:
- handling pipes
*/
t_error	execute_single_command(t_token *cmd_token, t_shell *s)
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
		return (handle_malloc_error(s));
	if (has_redirection(s))
		result = execute_redirection(s);
	else if (is_built_in(cmd_token->content))
		result = execute_built_in(cmd_token, args, s);
	else
		result = execute_external_command(cmd_token, args, s);
	free_array(args);
	return (result);
}


t_error execute_command(t_token *cmd_token, t_shell *s)
{
	if (has_pipe(s))
		return (handle_pipe_operations(s));
	else
		return (execute_single_command(cmd_token, s));
}