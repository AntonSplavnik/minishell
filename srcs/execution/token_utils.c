/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:23:28 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/13 13:06:44 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*copy_tokens(t_token *src)
{
	t_token	*dst;
	t_token	*new;
	t_token	*current;

	dst = NULL;
	current = src;
	while (current)
	{
		new = token_new(current->content, current->type);
		if (!new)
		{
			token_clear(&dst);
			return (NULL);
		}
		token_add_back(&dst, new);
		current = current->next;
	}
	return (dst);
}

void	token_clear(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	current = *tokens;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	*tokens = NULL;
}

t_token	*token_new(char *content, int type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->content = ft_strdup(content);
	if (!new->content)
	{
		free(new);
		return (NULL);
	}
	new->type = type;
	new->next = NULL;
	return (new);
}

void	token_add_back(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
}
