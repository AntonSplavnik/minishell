/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:12:37 by abillote          #+#    #+#             */
/*   Updated: 2025/01/02 21:36:59 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Creates a new environment variable node:
- Splits input string at '=' character
- Stores key (before '=') and value (after '=')
- Allocates memory for new node and its contents
Returns: New env node or NULL if memory allocation fails
*/
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

/*Adds a new environment variable to the env list:
- Creates new env node from input string
- Adds it at the end of the list
- Handles empty list case
Returns: SUCCESS or ERR_ENV if creation fails
*/
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

/*
Initializes environment list from main's env:
- Processes each environment string
- Creates and adds env nodes to the list
Returns: SUCCESS or ERR_ENV if initialization fails
*/
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
