/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:48 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/03 18:04:34 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	handle_parent_process
	handle_child_process_io
	handle_child_signal
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

static void	handle_child_process_io(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

void	handle_child_signal(int status, t_shell *s)
{
	if (WTERMSIG(status) == SIGINT)
	{
		s->exit_status = 130;
		write(1, "\n", 1);
	}
	else if (WTERMSIG(status) == SIGQUIT)
	{
		s->exit_status = 131;
		write(2, "Quit: 3\n", 9);
	}
}