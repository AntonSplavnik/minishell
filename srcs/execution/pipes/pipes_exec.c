/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:26:40 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/11 17:49:41 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	create_pipe_and_fork
	process_child
	set_output_fd
*/

/*
Purpose:
Creates a pipe and forks a new child process if necessary.

Functionality:
If cmd_count > 0, creates a pipe using pipe(pipe_fd).
Forks a new process using fork().
Returns ERR_EXEC if either operation fails.
*/
t_error	create_pipe_and_fork(int cmd_count, int pipe_fd[2], pid_t *pid)
{
	if (cmd_count > 0 && pipe(pipe_fd) == -1)
		return (ERR_EXEC);
	*pid = fork();
	if (*pid == -1)
		return (ERR_EXEC);
	return (SUCCESS);
}

/*
Purpose:
Executes a command as a child process within a pipeline.

Functionality:
Redirects input/output based on the pipeline structure.
Closes unnecessary file descriptors.
Calls execute_single_command(cmd, s) to execute the command.
Exits with the shell’s exit status.
*/
t_error	process_child(t_token *cmd, int prev_pipe, int cmd_count,
										int pipe_fd[2], t_shell *s)
{
	int	out_fd;

	set_output_fd(cmd_count, pipe_fd, &out_fd);
	handle_child_process_io(prev_pipe, out_fd);
	if (prev_pipe != -1)
		close(prev_pipe);
	if (cmd_count > 0)
		close(pipe_fd[0]);
	execute_single_command(cmd, s);
	exit(s->exit_status);
	return (SUCCESS);
}

/*
Purpose:
Sets the output file descriptor based on whether the command is part of a
pipeline.

Functionality:
If cmd_count > 0, assigns pipe_fd[1] (write end of the pipe) to out_fd.
Otherwise, sets out_fd to STDOUT_FILENO.
*/
void	set_output_fd(int cmd_count, int pipe_fd[2], int *out_fd)
{
	if (cmd_count > 0)
		*out_fd = pipe_fd[1];
	else
		*out_fd = STDOUT_FILENO;
}
