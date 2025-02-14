/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_child_parent.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:23:20 by abillote          #+#    #+#             */
/*   Updated: 2025/02/14 16:49:39 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Handles signals in child processes:
- SIGINT (Ctrl+C): Exit with 130
- SIGQUIT (Ctrl+\): Print "Quit (core dumped)", exit with 131
*/
void	signal_handler_child(int signum)
{
	g_sig = signum;
	if (signum == SIGINT)
	{
		write(2, "\n", 1);
		exit (130);
	}
	else if (signum == SIGQUIT)
	{
		ft_putendl_fd("Quit (core dumped)", 2);
		exit (131);
	}
}

/*
Set up signal handling for child process:
- SIGINT (Ctrl+C): Exit with 130
- SIGQUIT (Ctrl+\): Print "Quit (core dumped)", exit with 131
Returns:
  - 1 if error
  - 0 if success
*/
int	set_signals_child(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler_child;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1 || sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
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
	if (sigaction(SIGINT, &sa, NULL) == -1 || sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	return (0);
}
