/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_child_parent.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:23:20 by abillote          #+#    #+#             */
/*   Updated: 2025/03/17 14:35:53 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Set up signal handling for child process:
- Setting back SIGINT and SIGQUIT to their default behavior
- This way they do not inherited the special handling from parents
*/
int	set_signals_child(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1 || \
		sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	return (0);
}

/*
Signal handler for parent process while child is running:
- Set global signal variable
- If child interrupted with SIGINT, print a new line
- If child interrupted with SIGQUIT, write core dumped and a new line
*/
void	signal_handler_parent(int signum)
{
	g_sig = signum;
	if (signum == SIGINT)
		write(2, "\n", 1);
	else if (signum == SIGQUIT)
		write(2, "Quit (core dumped)\n", 18);
}

/*
Set up signal handling for parent process while child is running:
- Handle SIGINT and SIGQUIT
Returns:
  - 1 if error
  - 0 if success
*/
int	set_signals_parent(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler_parent;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1 || \
	sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	return (0);
}
