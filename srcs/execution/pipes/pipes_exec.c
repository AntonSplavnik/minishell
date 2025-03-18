/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:26:40 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/18 12:44:06 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	create_pipe_and_fork(int cmd_count, int pipe_fd[2], pid_t *pid)
{
	if (cmd_count > 0 && pipe(pipe_fd) == -1)
		return (ERR_EXEC);
	*pid = fork();
	if (*pid == -1)
		return (ERR_EXEC);
	return (SUCCESS);
}

t_error	process_child(t_token *cmd, t_pipe_info *pinfo, t_shell *s)
{
	int	out_fd;

	set_signals_child();
	set_output_fd(pinfo->cmd_count, pinfo->pipe_fd, &out_fd);
	handle_child_process_io(pinfo->prev_pipe, out_fd);
	if (pinfo->cmd_count > 0)
	{
		if (pinfo->pipe_fd[0] != -1)
		{
			close(pinfo->pipe_fd[0]);
			pinfo->pipe_fd[0] = -1;
		}
	}
	execute_single_command(cmd, s);
	exit(s->exit_status);
	return (SUCCESS);
}

void	set_output_fd(int cmd_count, int pipe_fd[2], int *out_fd)
{
	if (cmd_count > 0)
		*out_fd = pipe_fd[1];
	else
		*out_fd = STDOUT_FILENO;
}

t_error	process_parent(int *prev_pipe, int pipe_fd[2], int cmd_count)
{
	if (*prev_pipe != -1)
	{
		close(*prev_pipe);
		*prev_pipe = -1;
	}
	if (cmd_count > 0)
	{
		*prev_pipe = pipe_fd[0];
		close(pipe_fd[1]);
	}
	return (SUCCESS);
}

void	setup_pipe_info(t_pipe_info *pinfo, int *prev_pipe,
			int pipe_fd[2], int cmd_count)
{
	pinfo->prev_pipe = *prev_pipe;
	pinfo->cmd_count = cmd_count;
	pinfo->pipe_fd[0] = pipe_fd[0];
	pinfo->pipe_fd[1] = pipe_fd[1];
}
