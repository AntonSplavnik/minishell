/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:06:01 by abillote          #+#    #+#             */
/*   Updated: 2025/03/07 12:08:31 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 Updates length counters when handling dollar sign expansion
 - Determines length needed for expanded variables
 - Special case: $ with non-variable chars in double quotes
 - Special case: handles $? for exit status expansion
 - Updates position index to skip processed content
 - Updates expanded length count for allocation
 Parameters:
   token: Current token being processed
   s: Shell context containing environment variables
   i: Pointer to current position in token content
   expanded_len: Pointer to running length counter
*/
void	process_dollar_length(t_token *token, t_shell *s,
	size_t *i, size_t *expanded_len)
{
	if (token->content[*i + 1] != '_' && !ft_isalnum(token->content[*i + 1])
		&& token->content[*i + 1] != '?')
	{
		(*expanded_len)++;
		(*i)++;
	}
	else
		get_var_length(token->content + *i + 1, s, i, expanded_len);
}

/*
 Handles expansion of dollar sign variables in token content
 - Performs actual replacement of variables with their values
 - Handles special case for $ in double quotes
 - Expands variables only when not in single quotes
 - Maintains proper null-termination of strings
 - Updates position indices for both input and output
 Parameters:
   t: Token being processed
   exp: Output buffer for expanded content
   s: Shell context containing environment variables
   pars: Parse state tracking indices and quote context
*/
void	process_dollar_expansion(t_token *t, char *exp,
	t_shell *s, t_parse_params *pars)
{
	if (pars->in_dquote && t->content[pars->i + 1] != '_'
		&& !ft_isalnum(t->content[pars->i + 1])
		&& t->content[pars->i + 1] != '?')
	{
		exp[pars->j++] = t->content[pars->i++];
		exp[pars->j] = '\0';
	}
	else if (!pars->in_squote && t->content[pars->i + 1])
	{
		exp[pars->j] = '\0';
		expand_var(t->content + pars->i + 1, s, &pars->i, exp);
		pars->j = ft_strlen(exp);
	}
	else
	{
		exp[pars->j++] = t->content[pars->i++];
		exp[pars->j] = '\0';
	}
}
