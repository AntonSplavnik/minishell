/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:54:53 by abillote          #+#    #+#             */
/*   Updated: 2025/03/17 15:16:20 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cleanup_signals(t_shell *s, char *args)
{
	s->exit_status = 130;
	free(args);
	if (s->token_list)
		free_token_list(&s->token_list);
	return (1);
}

/*
Helper function to set exit status after processing input
*/
int	set_exit_status(t_error error, t_shell *s, char *args)
{
	if (error == ERR_SIGNAL && g_sig == SIGINT)
	{
		s->exit_status = 130;
		g_sig = 0;
	}
	else if (error == ERR_CMD_NOT_FOUND)
		s->exit_status = 127;
	else if (error == ERR_PARSING || error == ERR_PARSING_PIPE || \
			error == ERR_PARSING_REDIR || error == ERR_SYNTAX)
		s->exit_status = 2;
	else if (error != SUCCESS)
		s->exit_status = 1;
	if (args)
		free(args);
	return (1);
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
