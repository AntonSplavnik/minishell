/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion_var.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:28:37 by abillote          #+#    #+#             */
/*   Updated: 2025/01/10 17:46:12 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Searches for an environment variable by key in the env_list
Returns the value if found, empty string if not found
Special case: if key is "?", returns the last exit status
*/
char	*get_var_value(t_env *env_list, char *key, int status)
{
	if (ft_strcmp(key, "?") == 0)
		return (ft_itoa(status));
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return ("");
}

/* Extracts the environment variable name from a string starting after '$'
Returns:
- "?" for $? (exit status)
- The variable name for valid identifiers (alphanumeric + underscore)
- Empty string for invalid identifiers
- NULL if memory allocation fails
*/
char	*get_var_name(const char *str)
{
	size_t	name_len;
	char	*var_name;

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

/* Calculates the length needed for the expanded variable:
- Extracts variable name from the string
- Gets the variable's value from env_list
- Updates expanded_len with the value's length
- Updates index i to skip over the variable name
*/
void	get_var_length(const char *str, t_shell *s, \
					size_t *i, size_t *expanded_len)
{
	char	*var_name;
	char	*var_value;

	var_name = get_var_name(str);
	var_value = get_var_value(s->env_list, var_name, s->exit_status);
	*expanded_len += ft_strlen(var_value);
	*i += ft_strlen(var_name);
	free(var_name);
}

/*Performs the actual expansion of an environment variable:
- Extracts the variable name from the input string
- Gets its value from the environment
- Copies the value to the expanded string
- Updates index i to skip over $VARNAME
- For $?, returns the last exit status
*/
void	expand_var(const char *str, t_shell *s, \
				size_t *i, char *expanded)
{
	char	*var_name;
	char	*var_value;
	size_t	len;

	var_name = get_var_name(str);
	var_value = get_var_value(s->env_list, var_name, s->exit_status);
	len = 0;
	while (expanded[len] && len < ft_strlen(expanded))
		len++;
	ft_strcpy(expanded + len, var_value);
	*i += ft_strlen(var_name) + 1;
	free(var_name);
}
