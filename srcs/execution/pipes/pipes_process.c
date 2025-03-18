/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:47:20 by abillote          #+#    #+#             */
/*   Updated: 2025/03/18 15:50:22 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_pipe_info(t_pipe_info *pinfo, int *prev_pipe,
			int pipe_fd[2], int cmd_count)
{
	pinfo->prev_pipe = *prev_pipe;
	pinfo->cmd_count = cmd_count;
	pinfo->pipe_fd[0] = pipe_fd[0];
	pinfo->pipe_fd[1] = pipe_fd[1];
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

t_error	process_pipe_stage(t_shell *s, t_token **current, int *prev_pipe,
															int cmd_count)
{
	int			pipe_fd[2];
	pid_t		pid;
	t_token		*cmd;
	t_error		result;
	t_pipe_info	pinfo;

	cmd = get_next_cmd(current);
	result = create_pipe_and_fork(cmd_count, pipe_fd, &pid);
	if (result != SUCCESS)
		return (result);
	if (pid == 0)
	{
		setup_pipe_info(&pinfo, prev_pipe, pipe_fd, cmd_count);
		result = process_child(cmd, &pinfo, s);
		token_clear(&cmd);
		exit(s->exit_status);
	}
	if (cmd_count == 0)
		s->last_pid = pid;
	token_clear(&cmd);
	return (process_parent(prev_pipe, pipe_fd, cmd_count));
}
