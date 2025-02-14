/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_interactive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 11:17:51 by abillote          #+#    #+#             */
/*   Updated: 2025/02/14 16:48:26 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Signal handler function:
- For SIGINT (CTRL + C), reset readline, clear the line and show new prompt
*/

void	signal_handler_interactive(int signum)
{
	g_sig = signum;
	if (signum == SIGINT)
	{
		write(2, "\n", 1);
		if (rl_on_new_line() == -1)
			exit (1);
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/*
Set sigint and sigquit signals for interactive mode
- For SIGINT, use the signal handler to display a new line
- For SIGQUIT (CTRL + \), ignore the signal
Returns:
  - 1 if error
  - 0 if success
*/
int	set_signals_interactive(void)
{
	struct sigaction	sa;
	struct sigaction	sa_quit;

	sa.sa_handler = signal_handler_interactive;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	return (0);
}
