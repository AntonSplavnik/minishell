/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:43:11 by abillote          #+#    #+#             */
/*   Updated: 2024/11/12 12:49:48 by abillote         ###   ########.fr       */
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
