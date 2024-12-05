/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion_quotes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:45:01 by abillote          #+#    #+#             */
/*   Updated: 2024/12/05 18:35:52 by abillote         ###   ########.fr       */
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
						int *in_dquote)
{
	if (quote_char == '\'')
		*in_squote = !*in_squote;
	else
		*in_dquote = !*in_dquote;
}
