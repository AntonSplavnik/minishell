/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:48 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/07 13:36:22 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	handle_child_process_io
	handle_child_signal
*/

/*
Purpose:
Redirects input and output file descriptors in the child process before executing a command.

Functionality:
If in_fd is not STDIN_FILENO, it redirects standard input to in_fd.
If out_fd is not STDOUT_FILENO, it redirects standard output to out_fd.
Closes the original file descriptors after duplication to prevent leaks.
*/
void	handle_child_process_io(int in_fd, int out_fd)
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

/*
Purpose:
Handles child process termination signals and updates the shell exit status accordingly.

Functionality:
If the process was terminated by SIGINT, sets s->exit_status to 130 and prints a newline.
If terminated by SIGQUIT, sets s->exit_status to 131 and prints "Quit: 3".
*/
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
