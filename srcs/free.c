/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:43:11 by abillote          #+#    #+#             */
/*   Updated: 2025/01/17 15:38:50 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*Free an array of string*/

void	free_array(char **array)
{
	int	i;

	i = 0;
	while(array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/*
Frees entire token linked list:
- Iterates through list freeing each node
- Frees both node content and node itself
- Sets list pointer to NULL after freeing
*/
void	free_token_list(t_token **token_list)
{
	t_token	*temp;

	while (*token_list)
	{
		temp = *token_list;
		*token_list = (*token_list)->next;
		free(temp->content);
		free(temp);
	}
	*token_list = NULL;
}

/*
Frees entire environment variable linked list:
- Iterates through list freeing each node
- Frees key, value, and node itself
- Sets list pointer to NULL after freeing
*/
void	free_env_list(t_env **env_list)
{
	t_env	*temp;

	while (*env_list)
	{
		temp = *env_list;
		*env_list = (*env_list)->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
	*env_list = NULL;
}
