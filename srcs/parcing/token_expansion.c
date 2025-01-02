/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:43:49 by abillote          #+#    #+#             */
/*   Updated: 2025/01/02 21:34:45 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Calculates the required length for a token after expansion by:
- Processing quotes (single and double)
- Calculating expanded environment variable lengths
- Counting regular characters
Returns the total length needed for the expanded string
*/
size_t	calculate_length_loop(t_token *token, \
			t_env *env_list, size_t *expanded_len)
{
	size_t	i;
	int		in_squote;
	int		in_dquote;

	init_expansion_params(&i, expanded_len, &in_squote, &in_dquote);
	while (token->content[i])
	{
		if ((token->content[i] == '\'' && !in_dquote)
			|| (token->content[i] == '"' && !in_squote))
		{
			process_quote(token->content[i], &in_squote, &in_dquote);
			i++;
			continue ;
		}
		else if (token->content[i] == '$' && \
					!in_squote && token->content[i + 1])
			get_var_length(token->content + i + 1, env_list, &i, expanded_len);
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
size_t	calculate_expanded_length(t_token *token, t_env *env_list)
{
	size_t	expanded_len;
	size_t	i;
	int		in_squote;
	int		in_dquote;

	expanded_len = 0;
	init_expansion_params(&i, &expanded_len, &in_squote, &in_dquote);
	return (calculate_length_loop(token, env_list, &expanded_len));
}

/*
Handles the expansion of a single environment variable within a token
Updates the expanded string and position indices accordingly
*/

static void	process_variable_expansion(char *expanded, const char *content, \
			t_env *env_list, t_parse_params *params)
{
	expanded[params->j] = '\0';
	expand_var(content + params->i + 1, env_list, &params->i, expanded);
	params->j = ft_strlen(expanded);
}

/*
Performs the actual content expansion for a token:
- Handles quoted sections (preserving content in single quotes)
- Expands environment variables (except within single quotes)
- Removes unnecessary quotes
- Updates the token's content with the expanded version
*/
void	fill_token_content_after_expansion(t_token *token, char *exp, \
						t_env *env_list)
{
	t_parse_params	params;

	params = (t_parse_params){0, 0, 0, 0};
	exp[0] = '\0';
	while (token->content[params.i])
	{
		if ((token->content[params.i] == '\'' && !params.in_dquote) || \
			(token->content[params.i] == '"' && !params.in_squote))
		{
			process_quote(token->content[params.i], &params.in_squote, \
						&params.in_dquote);
			params.i++;
			continue ;
		}
		if (token->content[params.i] == '$' && !params.in_squote
			&& token->content[params.i + 1])
			process_variable_expansion(exp, token->content, env_list, &params);
		else
		{
			exp[params.j++] = token->content[params.i++];
			exp[params.j] = '\0';
		}
	}
	free(token->content);
	token->content = exp;
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
t_error	expand_tokens(t_token **token_list, t_env *env_list)
{
	t_token	*current;
	char	*expanded;
	size_t	expanded_len;

	current = *token_list;
	while (current)
	{
		expanded_len = calculate_expanded_length(current, env_list);
		expanded = malloc(sizeof(char) * (expanded_len + 1));
		if (!expanded)
			return (ERR_MALLOC);
		fill_token_content_after_expansion(current, expanded, env_list);
		current = current->next;
	}
	return (SUCCESS);
}
