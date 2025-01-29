/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:22:48 by abillote          #+#    #+#             */
/*   Updated: 2025/01/29 18:04:28 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Removes a variable from the environment list:
- Handles case where variable is first in list
- Handles case where variable is in middle/end of list
- Properly maintains list linkage
- Do nothing if variable doesn't exist
*/
void	remove_env_var(t_env **env_list, char *var_name)
{
	t_env	*current;
	t_env	*prev;

	if (!env_list || !*env_list || !var_name)
		return ;
	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			free(current->content);
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}


/*
Built-in unset command implementation:
- Removes variables from environment
- Handles multiple arguments
- Do nothing on non-existent variables
Returns: SUCCESS
*/
t_error	execute_unset(char **args, t_shell *s)
{
	int	i;

	i = 1;
	while (args[i])
	{
		remove_env_var(&s->env_list, args[i]);
		i++;
	}
	s->exit_status = 0;
	return (SUCCESS);
}
