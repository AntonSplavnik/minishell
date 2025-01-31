/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion_quotes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:45:01 by abillote          #+#    #+#             */
/*   Updated: 2025/01/02 21:34:40 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Initializes parameters used during token expansion:
** - i: index for current position in the input string
** - j: index for current position in the output string
** - in_squote: flag for being inside single quotes
** - in_dquote: flag for being inside double quotes
*/
void	init_expansion_params(size_t *i, size_t *j, \
					int *in_squote, int *in_dquote)
{
	*i = 0;
	*j = 0;
	*in_squote = 0;
	*in_dquote = 0;
}

/*
** Handles quote processing by toggling the appropriate quote state:
** - For single quotes: toggles in_squote
** - For double quotes: toggles in_dquote
** This tracking is used to handle nested quotes and variable expansion rules
*/
void	process_quote(char quote_char, int *in_squote, \
						int *in_dquote)
{
	if (quote_char == '\'')
		*in_squote = !*in_squote;
	else
		*in_dquote = !*in_dquote;
}
