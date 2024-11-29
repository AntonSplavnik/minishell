/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:43:49 by abillote          #+#    #+#             */
/*   Updated: 2024/11/29 17:37:20 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"



char	*get_var_value(t_env *env_list, char *key)
{
	//if (ft_strcmp(var_name, "?") == 0)
	//	return (ft_itoa(shell->exit_status))
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return ("");
}

char	*get_var_name(const char *str)
{
	size_t name_len;
	char *var_name;

	if (str[0] == '?')
		return (ft_strdup("?"));
	name_len = 0;
	while (str[name_len] && (ft_isalnum(str[name_len]) || str[name_len] == '_'))
		name_len++;
	if (name_len == 0)
		return (ft_strdup(""));
	var_name = ft_substr(str, 0, name_len);
	if (!var_name)
		return (NULL);
	return (var_name);
}

void	get_var_length(const char *str, t_env *env_list, size_t *i, size_t *expanded_len)
{
	char *var_name;
	char *var_value;

	var_name = get_var_name(str);
	var_value = get_var_value(env_list, var_name);
	*expanded_len += ft_strlen(var_value);
	*i += ft_strlen(var_name);
	free(var_name);
}

/*Get the env var value. For $?, need to figure out how to store it in the shell struct to return it
If no var existing, return an empty string*/
//void expand_var(const char *str, t_env *env_list, size_t *i, char *expanded)
//{
//	char *var_name;
//	char *var_value;
//	size_t j;

//	var_name = get_var_name(str);
//	var_value = get_var_value(env_list, var_name);
//	j = ft_strlen(expanded);
//	ft_strcpy(expanded + j, var_value);
//	ft_strlcat(expanded, var_value, ft_strlen(expanded) + ft_strlen(var_value) + 1);
//	printf("expanded: %s\n", expanded);
//	*i += ft_strlen(var_name) + 1;
//	free(var_name);
//}

void expand_var(const char *str, t_env *env_list, size_t *i, char *expanded)
{
    char *var_name;
    char *var_value;
    size_t len;

    var_name = get_var_name(str);
    var_value = get_var_value(env_list, var_name);
    len = 0;
    while (expanded[len] && len < ft_strlen(expanded))
        len++;
    ft_strcpy(expanded + len, var_value);
	printf("expanded: %s\n", expanded);
    *i += ft_strlen(var_name) + 1;
    free(var_name);
}

void	init_expansion_params(size_t *i, size_t *j, int *in_squote, int *in_dquote)
{
	*i = 0;
	*j = 0;
	*in_squote = 0;
	*in_dquote = 0;
}

void process_quote(char quote_char, int *in_squote, int *in_dquote, size_t *i)
{
	if (quote_char == '\'')
		*in_squote = !*in_squote;
	else
		*in_dquote = !*in_dquote;
	(*i)++;
}

size_t calculate_length_loop(t_token *token, t_env *env_list, size_t *expanded_len)
{
	size_t i;
	int in_squote;
	int in_dquote;

	init_expansion_params(&i, expanded_len, &in_squote, &in_dquote);
	while (token->content[i])
	{
		if ((token->content[i] == '\'' && !in_dquote) ||
			(token->content[i] == '"' && !in_squote))
		{
			process_quote(token->content[i], &in_squote, &in_dquote, &i);
			continue;
		}
		else if (token->content[i] == '$' && !in_squote && token->content[i + 1])
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

void fill_token_content_after_expansion(t_token *token, char *expanded, t_env *env_list)
{
	size_t	i;
	size_t	j;
	int		in_squote;
	int		in_dquote;

	in_squote = 0;
	in_dquote = 0;
	i = 0;
	j = 0;
	expanded[j] = '\0';
	while (token->content[i])
	{
		if ((token->content[i] == '\'' && !in_dquote) || \
			(token->content[i] == '"' && !in_squote))
		{
			if (token->content[i] == '\'')
				in_squote = !in_squote;
			else
				in_dquote = !in_dquote;
			i++;
		}
		else if (token->content[i] == '$' && !in_squote && token->content[i + 1])
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
		current= current->next;
	}
	return (SUCCESS);
}
