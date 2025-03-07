/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:43:49 by abillote          #+#    #+#             */
/*   Updated: 2025/03/07 12:01:38 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 Calculates the required length for a token after expansion
 - Tracks quote context to handle nested quotes
 - Handles special cases for variable expansion
 - Counts characters for the expanded version of the token
 - Uses process_dollar_length for $ handling
 Returns: Total length needed for expanded token
*/
size_t	calculate_length_loop(t_token *token, t_shell *s, size_t *expanded_len)
{
	size_t	i;
	int		in_squote;
	int		in_dquote;

	init_expansion_params(&i, expanded_len, &in_squote, &in_dquote);
	while (token->content[i])
	{
		if (((token->content[i] == '\'' && !in_dquote) \
			|| (token->content[i] == '"' && !in_squote))
			&& token->type != TYPE_HEREDOC_CONTENT)
		{
			process_quote(token->content[i], &in_squote, &in_dquote);
			i++;
		}
		else if (token->content[i] == '$' && !in_squote)
			process_dollar_length(token, s, &i, expanded_len);
		else if (token->content[i] == '$' && in_dquote)
			process_dollar_length(token, s, &i, expanded_len);
		else
		{
			(*expanded_len)++;
			i++;
		}
	}
	return (*expanded_len);
}

/*
Wrapper function that initializes parameters and calls calculate_length_loop
Returns the total length needed for the expanded token content
*/
size_t	calculate_expanded_length(t_token *token, t_shell *s)
{
	size_t	expanded_len;
	size_t	i;
	int		in_squote;
	int		in_dquote;

	expanded_len = 0;
	init_expansion_params(&i, &expanded_len, &in_squote, &in_dquote);
	return (calculate_length_loop(token, s, &expanded_len));
}

/*
 Performs token content expansion by processing each character
 - Handles quoted sections correctly
 - Removes quote characters while preserving their content
 - Expands environment variables based on quote context
 - Uses process_dollar_expansion for variable replacement
 - Handles special cases like heredoc content
*/
void	fill_token_content_after_expansion(t_token *t, char *exp, t_shell *s)
{
	t_parse_params	pars;

	pars = (t_parse_params){0, 0, 0, 0};
	exp[0] = '\0';
	while (t->content[pars.i])
	{
		if (((t->content[pars.i] == '\'' && !pars.in_dquote) \
			|| (t->content[pars.i] == '"' && !pars.in_squote))
			&& t->type != TYPE_HEREDOC_CONTENT)
		{
			process_quote(t->content[pars.i], &pars.in_squote, &pars.in_dquote);
			pars.i++;
		}
		else if (t->content[pars.i] == '$')
			process_dollar_expansion(t, exp, s, &pars);
		else
		{
			exp[pars.j++] = t->content[pars.i++];
			exp[pars.j] = '\0';
		}
	}
	free(t->content);
	t->content = exp;
}

/*
Main token expansion function that:
1. Iterates through all tokens
2. Calculates required space for expanded content
3. Allocates memory for expansion
4. Expands environment variables
5. Removes unnecessary quotes
Returns SUCCESS or ERR_MALLOC if memory allocation fails
*/
t_error	expand_tokens(t_shell *s)
{
	t_token	*current;
	char	*expanded;
	size_t	expanded_len;

	current = s->token_list;
	while (current)
	{
		if (current->type == TYPE_HEREDOC_CONTENT_QUOTED)
		{
			current = current->next;
			continue ;
		}
		expanded_len = calculate_expanded_length(current, s);
		expanded = malloc(sizeof(char) * (expanded_len + 1));
		if (!expanded)
			return (ERR_MALLOC);
		fill_token_content_after_expansion(current, expanded, s);
		current = current->next;
	}
	return (SUCCESS);
}
