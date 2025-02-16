/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_child_parent.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:23:20 by abillote          #+#    #+#             */
/*   Updated: 2025/02/16 08:04:18 by abillote         ###   ########.fr       */
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
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	return (0);
}

/*
Signal handler for parent process while child is running:
- Set global signal variable
- Let the child process handle the actual signal response
*/
void	signal_handler_parent(int signum)
{
	g_sig = signum;
}

/*
Set up signal handling for parent process while child is running:
- Handle SIGINT but don't take any action (let child handle it)
- Handle SIGQUIT similarly
Returns:
  - 1 if error
  - 0 if success
*/
int	set_signals_parent(void)
{
	struct sigaction	sa;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	sa.sa_handler = signal_handler_parent;
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
