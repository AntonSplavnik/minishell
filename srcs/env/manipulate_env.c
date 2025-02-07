/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manipulate_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:47:47 by abillote          #+#    #+#             */
/*   Updated: 2025/02/07 12:09:07 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


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
