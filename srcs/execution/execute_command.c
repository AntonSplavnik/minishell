/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:44:50 by abillote          #+#    #+#             */
/*   Updated: 2025/01/31 16:44:35 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Return the number of args needed for the command execution
*/
int	count_command_args(t_token *cmd_token)
{
	t_token	*current;
	int		arg_count;

	arg_count = 1;
	current = cmd_token->next;
	while (current && current->type != TYPE_PIPE)
	{
		if (current->type == TYPE_ARG)
			arg_count++;
		current = current->next;
	}
	return (arg_count);
}

/*
Prepares command arguments array from token list
Returns array of strings suitable for execve:
- args[0] should be the file name
*/
char	**prepare_command_args(t_token *cmd_token)
{
	t_token	*current;
	char	**args;
	int		arg_count;
	int		i;

	arg_count = count_command_args(cmd_token);
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	args[0] = ft_strdup(cmd_token->content);
	i = 1;
	current = cmd_token->next;
	while (current && current->type != TYPE_PIPE)
	{
		if (current->type == TYPE_ARG)
		{
			args[i] = ft_strdup(current->content);
			i++;
		}
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

/*
Use fork() to split the process into two processes:
- Parent process gets the child's PID (>0)
- Child process get PID 0
- if (pid == 0) -> only runs in the child process
- Then execve replaces current process (the child) with a new program.
- If successful, execve NEVER returns - the process becomes the new program.
- Exit 127 is conventionnal exit code meaning "command not found"
In the parents process:
- waitpid -> make parents wait for the child process to finish
- store the child's exit info in status
- won't continue until child process is done
- if (WIFEXITED(status)) -> check that child process
ended up normally (not killed by a signal)
- WEXITSTATUS(status) : get the child exit code
RETURNS: appropriate error code
*/
t_error	execute_child_process(char *cmd_path, char **args, t_shell *s)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		free_command_path(cmd_path, args);
		s->exit_status = 1;
		return (ERR_EXEC);
	}
	if (pid == 0)
	{
		execve(cmd_path, args, s->envp);
		s->exit_status = 127;
		ft_putendl_fd("Command could not be executed", 2);
		exit(127);
	}
	else
	{
		waitpid(pid, &status, 0);
		free_command_path(cmd_path, args);
		if (WIFEXITED(status))
			s->exit_status = WEXITSTATUS(status);
	}
	return (SUCCESS);
}

/*
Executes a single command with its arguments
Find the args by going through the token list
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

	//Check for pipes and handle them here

	//if no pipes, execute simple command
	args = prepare_command_args(cmd_token);
	if (!args)
	{
		s->exit_status = 12;
		return (ERR_MALLOC);
	}
	if (is_built_in(cmd_token->content))
		return (execute_built_in(cmd_token, args, s));
	cmd_path = find_command_path(cmd_token->content, s);
	if (!cmd_path)
	{
		free_array(args);
		return (ERR_CMD_NOT_FOUND);
	}
	return (execute_child_process(cmd_path, args, s));
}
