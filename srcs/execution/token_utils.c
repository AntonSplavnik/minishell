/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:23:28 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/11 17:32:15 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Purpose:
Creates a deep copy of a linked list of tokens.

Functionality:
Iterates through the source token list.
Creates new token nodes using token_new and appends them to the new list
using token_add_back.
Returns the copied list or NULL on failure.
*/
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

/*
Purpose:
Frees all allocated memory for a list of tokens.

Functionality:
Iterates through the token list and frees each node's content and memory.
Sets the pointer to NULL to prevent dangling references.
*/
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

/*
Purpose:
Creates a new token with given content and type.

Functionality:
Allocates memory for a new t_token structure.
Duplicates the content string.
Initializes the type and sets the next pointer to NULL.
Returns the new token or NULL on failure.
*/
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

/*
urpose:
Adds a token to the end of a linked list of tokens.

Functionality:
If the list is empty, assigns new as the first element.
Otherwise, iterates to the last token and appends new.
*/

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
