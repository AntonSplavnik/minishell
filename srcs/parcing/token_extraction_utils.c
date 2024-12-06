/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:06:52 by abillote          #+#    #+#             */
/*   Updated: 2024/11/29 18:27:57 by abillote         ###   ########.fr       */
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
