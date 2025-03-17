/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:31:19 by abillote          #+#    #+#             */
/*   Updated: 2025/03/17 18:38:49 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Check for signal interruption during heredoc processing
** Returns ERR_SIGNAL if interrupted, SUCCESS otherwise
*/
t_error	check_heredoc_signal(void)
{
	if (g_sig == SIGINT)
	{
		set_signals_interactive();
		return (ERR_SIGNAL);
	}
	return (SUCCESS);
}

/*
 * Updated signal_handler_heredoc in signals_heredoc.c
 * Make it more robust
 */
void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		g_sig = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		close(STDIN_FILENO);
	}
}

/*
 * Updated set_signals_heredoc in signals_heredoc.c
 * Make sure it's consistent with how you set other signals
 */
int	set_signals_heredoc(void)
{
	struct sigaction	sa;
	struct sigaction	sa_quit;

	sa.sa_handler = signal_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sa.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1 || \
		sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	return (0);
}

/*
 * Helper function to reset global signal state
 */
void	reset_signal_state(void)
{
	g_sig = 0;
}

/*
** Handles signal interruption during heredoc collection
** Returns: ERR_SIGNAL after cleanup
*/
t_error	handle_heredoc_interruption(char **content_ptr, int stdin_copy)
{

	if (*content_ptr)
	{
		free(*content_ptr);
		*content_ptr = NULL;
	}
	dup2(stdin_copy, STDIN_FILENO);
	close(stdin_copy);
	set_signals_interactive();
	return (ERR_SIGNAL);
}
