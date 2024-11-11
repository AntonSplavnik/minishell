/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:21:21 by abillote          #+#    #+#             */
/*   Updated: 2024/11/11 17:38:11 by abillote         ###   ########.fr       */
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
		add_token(token_list, splited_arg[i], get_token_type(splited_arg[i]));
		i++;
	}
	//free splitted array
}

t_token	*create_token(char *input, t_token_type *type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (new_token == NULL)
		exit (1);
	new_token->content = ft_strdup(input);
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **token_list, char *input, t_token_type *type)
{
	t_token	*new_token;
	t_token	*current;

	current = token_list;
	new_token = create_token(input, type);
	if (*token_list == NULL)
		*token_list = new_token;
	while (current->next == NULL)
		current = current->next;
	current->next = new_token;
}

t_token_type get_token_type(char *input)
{
	int	i;

	i = 0;
	while (token_types[i].content)
	{
		if (strcmp(token_types[i].content, input) == 0)
			return (token_types[i].type);
		i++;
	}
	return (ft_strdup("TYPE_WORD"));
}
