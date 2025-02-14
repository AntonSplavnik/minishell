/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manipulate_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:47:47 by abillote          #+#    #+#             */
/*   Updated: 2025/02/14 10:13:08 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to set content in env variable:
- if append operation (+=), concatenate key and value to remove '+'
- else, duplicate the full env
Returns: SUCCESS or ERR_MALLOC if memory allocation fails
*/

t_error	set_env_content(t_env *new_envvar, char *env)
{
	char	*temp1;
	char	*temp2;

	if (ft_strstr(env, "+="))
	{
		temp1 = ft_strjoin(new_envvar->key, "=");
		if (!temp1)
			return (ERR_MALLOC);
		temp2 = ft_strjoin(temp1, new_envvar->value);
		if (!temp2)
			return (ERR_MALLOC);
		new_envvar->content = temp2;
	}
	else
		new_envvar->content = ft_strdup(env);
	if (!new_envvar->content)
		return (ERR_MALLOC);
	return (SUCCESS);
}

/*
Helper function to set key and value in env variable:
- Handles append operation (+=)
- Handles standard assignment (=)
- Handles variables without values
Returns: SUCCESS or ERR_MALLOC if memory allocation fails
*/
t_error	set_env_key_value(t_env *new_envvar, char *env)
{
	char	*equal_sign;
	char	*append_sign;

	append_sign = ft_strstr(env, "+=");
	equal_sign = ft_strchr(env, '=');
	if (append_sign)
	{
		new_envvar->key = ft_strndup(env, append_sign - env);
		new_envvar->value = ft_strdup(append_sign + 2);
	}
	else if (equal_sign)
	{
		new_envvar->key = ft_strndup(env, equal_sign - env);
		new_envvar->value = ft_strdup(equal_sign + 1);
	}
	else
	{
		new_envvar->key = ft_strdup(env);
		new_envvar->value = ft_strdup("");
	}
	if (!new_envvar->key || !new_envvar->value)
		return (ERR_MALLOC);
	else
		return (SUCCESS);
}

/*
Free one env var.
*/
void	free_env_var(t_env *envvar)
{
	if (envvar->content)
		free(envvar->content);
	if (envvar->key)
		free(envvar->key);
	if (envvar->value)
		free(envvar->value);
	free(envvar);
}

/*
Searches environment list for variable by key
Returns: Pointer to env node if found, NULL if not found
*/
t_env	*find_env_var(t_env *env_list, char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}
