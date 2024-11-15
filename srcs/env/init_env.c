/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:12:37 by abillote          #+#    #+#             */
/*   Updated: 2024/11/15 17:20:41 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*create_envvar(char *env)
{
	t_env	*new_envvar;
	char	*equal_sign;

	new_envvar = malloc(sizeof(t_env));
	if (!new_envvar)
		return (NULL);
	equal_sign = ft_strchr(env, '=');
	if (equal_sign)
	{
		new_envvar->key = ft_strndup(env, equal_sign - env);
		new_envvar->value = ft_strdup(equal_sign + 1);
		new_envvar->next = NULL;
	}
	return (new_envvar);
}

t_error	add_envvar(t_env **env_list, char *env)
{
	t_env	*new_envvar;
	t_env	*current;

	new_envvar = create_envvar(env);
	if (!new_envvar)
		return (ERR_ENV);
	if (*env_list == NULL)
	{
		*env_list = new_envvar;
		return (SUCCESS);
	}
	current = *env_list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_envvar;
	return (SUCCESS);
}

t_error	init_env(t_env **env_list, char **env)
{
	int		i;

	i = 0;
	while (env[i])
	{
		if (add_envvar(env_list, env[i]) != SUCCESS)
			return (handle_error_free_env(ERR_ENV, env_list));
		i++;
	}
	return (SUCCESS);
}
