/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:26:40 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/13 16:36:13 by asplavni         ###   ########.fr       */
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

	set_output_fd(pinfo->cmd_count, pinfo->pipe_fd, &out_fd);
	handle_child_process_io(pinfo->prev_pipe, out_fd);
	while (pinfo->prev_pipe != -1)
	{
		close(pinfo->prev_pipe);
		break ;
	}
	while (pinfo->cmd_count > 0)
	{
		close(pinfo->pipe_fd[0]);
		break ;
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
