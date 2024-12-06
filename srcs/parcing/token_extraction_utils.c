/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:06:52 by abillote          #+#    #+#             */
/*   Updated: 2024/12/06 16:31:02 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || \
			c == '\v' || c == '\f' || c == '\r');
}

int	is_delimiter(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

int	get_delimiter_len(const char *str)
{
	if (!str || !*str)
		return (0);
	if ((*str == '<' && *(str + 1) == '<') || \
		(*str == '>' && *(str + 1) == '>'))
		return (2);
	if (is_delimiter(*str))
		return (1);
	return (0);
}

t_token *get_last_token(t_token *token_list)
{
    if (!token_list)
        return (NULL);
    while (token_list->next)
        token_list = token_list->next;
    return (token_list);
}
