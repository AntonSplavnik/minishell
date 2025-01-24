/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:20:39 by abillote          #+#    #+#             */
/*   Updated: 2025/01/24 16:56:09 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Check the nb of args:
 - if 1, then it's just echo > print a new line and return
Check for the -n flag :
 - if the flag exists, do not print a new line after the args
Go through the list of args and print them.
 */
t_error	execute_echo(char **args)
{
	int	i;
	int	count_args;

	count_args = 0;
	while (args[count_args])
		count_args++;
	if (count_args <= 1)
	{
		write (1, "\n", 1);
		return (SUCCESS);
	}
	if (ft_strcmp(args[1], "-n") == 0)
		i = 2;
	else
		i = 1;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!(ft_strcmp(args[1], "-n") == 0))
		write(1, "\n", 1);
	return (SUCCESS);
}