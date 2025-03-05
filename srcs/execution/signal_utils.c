/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:48 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/05 14:11:23 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	handle_parent_process
	handle_child_process_io
	handle_child_signal
*/

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

/*
Purpose:
Redirects input and output file descriptors in the child process before executing a command.

Functionality:
If in_fd is not STDIN_FILENO, it redirects standard input to in_fd.
If out_fd is not STDOUT_FILENO, it redirects standard output to out_fd.
Closes the original file descriptors after duplication to prevent leaks.
*/
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