/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:48 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/13 18:39:19 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_child_process_io(int in_fd, int out_fd)
{
    if (in_fd != STDIN_FILENO && in_fd != -1)
    {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (out_fd != STDOUT_FILENO && out_fd != -1)
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
