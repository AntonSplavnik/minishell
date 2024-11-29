/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion_quotes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:45:01 by abillote          #+#    #+#             */
/*   Updated: 2024/11/29 18:50:28 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_expansion_params(size_t *i, size_t *j, \
					int *in_squote, int *in_dquote)
{
	*i = 0;
	*j = 0;
	*in_squote = 0;
	*in_dquote = 0;
}

void	process_quote(char quote_char, int *in_squote, \
						int *in_dquote, size_t *i)
{
	if (quote_char == '\'')
		*in_squote = !*in_squote;
	else
		*in_dquote = !*in_dquote;
	(*i)++;
}

size_t	process_quotes_status(char c, int *in_dquote, int *in_squote, size_t i)
{
	if ((c == '\'' && !(*in_dquote)) || \
			(c == '"' && !(*in_squote)))
	{
		if (c == '\'')
			*in_squote = !(*in_squote);
		else
			*in_dquote = !(*in_dquote);
		return (i + 1);
	}
	return (i);
}
