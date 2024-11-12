/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:21:21 by abillote          #+#    #+#             */
/*   Updated: 2024/11/12 13:35:37 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	input_to_token(t_token **token_list, char *args)
{
	char	**splitted_arg;
	int		i;

	i = 0;
	splitted_arg = ft_split(args, ' ');
	while (splitted_arg[i])
	{
		add_token(token_list, splitted_arg[i], get_token_type(splitted_arg[i]));
		i++;
	}
	i = 0;
	while (splitted_arg[i])
	{
		free(splitted_arg[i]);
		i++;
	}
	free(splitted_arg);
}

t_token	*create_token(char *input, t_token_type type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (new_token == NULL)
		exit (1);
	new_token->content = ft_strdup(input);
	if (new_token->content == NULL)
	{
		free(new_token);
		exit (1);
	}
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **token_list, char *input, t_token_type type)
{
	t_token	*new_token;
	t_token	*current;

	new_token = create_token(input, type);
	if (*token_list == NULL)
	{
		*token_list = new_token;
		return ;
	}
	current = *token_list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_token;
}

t_token_type	get_token_type(char *input)
{
	int	i;

	i = 0;
	while (g_token_types[i].content)
	{
		if (strcmp(g_token_types[i].content, input) == 0)
			return (g_token_types[i].type);
		i++;
	}
	return (TYPE_WORD);
}
