/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_string_unquoted.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:20:48 by abillote          #+#    #+#             */
/*   Updated: 2025/03/18 15:23:21 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Helper function to determine if character is part of adjacent unquoted text
*/
int	is_adjacent_unquoted_char(char c)
{
	return (c && !is_space(c) && !is_delimiter(c) && c != '"' && c != '\'');
}

/*
** Creates token with unquoted text and handles errors
*/
char	*create_combined_token(char *token, char *unquoted_part,
	t_error *error)
{
	char	*new_token;

	new_token = concatenate_tokens(token, unquoted_part);
	free(unquoted_part);
	free(token);
	if (!new_token)
		*error = ERR_MALLOC;
	return (new_token);
}

/*
** Extracts adjacent unquoted text that follows a quoted token
*/
char	*handle_adjacent_unquoted(char *args, size_t *i,
char *token, t_error *error)
{
	size_t	start;
	size_t	len;
	char	*unquoted_part;

	if (!is_adjacent_unquoted_char(args[*i]))
		return (token);
	start = *i;
	len = 0;
	while (is_adjacent_unquoted_char(args[*i]))
	{
		(*i)++;
		len++;
	}
	unquoted_part = ft_substr(args, start, len);
	if (!unquoted_part)
	{
		free(token);
		*error = ERR_MALLOC;
		return (NULL);
	}
	return (create_combined_token(token, unquoted_part, error));
}
