/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:20:39 by abillote          #+#    #+#             */
/*   Updated: 2025/01/24 16:47:29 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
