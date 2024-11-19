/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:43:11 by abillote          #+#    #+#             */
/*   Updated: 2024/11/15 17:17:40 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
