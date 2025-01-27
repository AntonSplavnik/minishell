/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:13:23 by abillote          #+#    #+#             */
/*   Updated: 2025/01/27 22:03:48 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_error	execute_exit(char **args, t_shell *s)
{
	int	count_args;

	count_args = 0;
	while (args[count_args])
		count_args++;
	if (count_args > 1)
	{
		s->exit_status = 1;
		return (ERR_EXIT);
	}

}
