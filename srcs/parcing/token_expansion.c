/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:43:49 by abillote          #+#    #+#             */
/*   Updated: 2024/12/06 15:46:48 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static void	process_variable_expansion(char *expanded, const char *content, \
			t_env *env_list, t_parse_params *params)
{
	expanded[params->j] = '\0';
	expand_var(content + params->i + 1, env_list, &params->i, expanded);
	params->j = ft_strlen(expanded);
}

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

/*This function is used to go through the token list and:
- Expand env variable with their real value
- Remove unecessary quotes
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
