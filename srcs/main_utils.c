/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:54:53 by abillote          #+#    #+#             */
/*   Updated: 2025/03/17 10:07:59 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Helper function to set exit status after processing input
*/
void	set_exit_status(t_error error, t_shell *s)
{
	if (error == ERR_MALLOC)
		s->exit_status = 1;
	else
		s->exit_status = 2;
}

int	only_whitespace(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
			return (0);
		i = i + 1;
	}
	return (1);
}
