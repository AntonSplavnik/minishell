/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:21:21 by abillote          #+#    #+#             */
/*   Updated: 2024/11/11 16:38:42 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	input_to_token(t_token **token_list, char *args)
{
	char	**splited_arg;
	int		i;

	i = 0;
	splited_arg = ft_split(args, ' ');
	while (splited_arg[i])
	{
		add_token(token_list, splited_arg[i]);
		i++;
	}
}

t_token	*create_token(char *input)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (new_token == NULL)
		exit (1);
	new_token->content = ft_strdup(input);
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **token_list, char *input)
{
	t_token	*new_token;
	t_token	*current;

	current = token_list;
	new_token = create_token(input);
	if (*token_list == NULL)
		*token_list = new_token;
	while (current->next == NULL)
		current = current->next;
	current->next = new_token;
}
