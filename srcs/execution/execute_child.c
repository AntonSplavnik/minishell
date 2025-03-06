/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:54:47 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/06 16:48:15 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	handle_child_process
	handle_parent_process
	execute_child_process
*/

void	handle_child_process(char *cmd_path, char **args, t_shell *s)
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
Purpose:
Manages the parent process's behavior after a command is executed in a child process.

Functionality:
Waits for the child process to finish using waitpid(pid, &status, 0).
Frees the allocated cmd_path.
Updates the shell's exit status using handle_exit_status(status, s).
Resets signal handling for interactive mode with set_signals_interactive().
*/
t_error	handle_parent_process(pid_t pid, char *cmd_path, t_shell *s)
{
	int	status;

	waitpid(pid, &status, 0);
	free(cmd_path);
	handle_exit_status(status, s);
	if (set_signals_interactive())
		return (ERR_SIGNAL);
	return (SUCCESS);
}


t_error	execute_child_process(char *cmd_path, char **args, t_shell *s)
{
	pid_t	pid;
	t_error	result;

	result = SUCCESS;
	if (set_signals_parent() != 0)
		return (free_resources(cmd_path, s, ERR_SIGNAL));
	pid = fork();
	if (pid == -1)
		return (free_resources(cmd_path, s, ERR_EXEC));
	if (pid == 0)
		handle_child_process(cmd_path, args, s);
	else
		result = handle_parent_process(pid, cmd_path, s);

	return (result);
}
