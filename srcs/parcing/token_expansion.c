/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:43:49 by abillote          #+#    #+#             */
/*   Updated: 2024/12/05 18:54:19 by abillote         ###   ########.fr       */
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

void	fill_token_content_after_expansion(t_token *token, \
					char *expanded, t_env *env_list)
{
	size_t	i;
	size_t	j;
	int		in_squote;
	int		in_dquote;

	init_expansion_params(&i, &j, &in_squote, &in_dquote);
	expanded[j] = '\0';
	while (token->content[i])
	{
		if ((token->content[i] == '\'' && !in_dquote) ||
			(token->content[i] == '"' && !in_squote))
		{
			process_quote(token->content[i], &in_squote, &in_dquote);
			i++;
			continue;
		}
		if (token->content[i] == '$' && !in_squote && token->content[i + 1])
		{
			expanded[j] = '\0';
			expand_var(token->content + i + 1, env_list, &i, expanded);
			j = ft_strlen(expanded);
		}
		else
			expanded[j++] = token->content[i++];
	}
	expanded[j] = '\0';
	free(token->content);
	token->content = expanded;
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
