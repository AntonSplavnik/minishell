/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:02:10 by abillote          #+#    #+#             */
/*   Updated: 2024/11/11 17:38:10 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	t_token	*token_list;
	char	*args;

	token_list = malloc(sizeof(t_token));
	while (1)
	{
		//store input with readline
		args = readline("minishell$ ");
		//add raw_input to history if it's not empty
		if (args[0] != '\0')
		{
			add_history(args);
		}
		input_to_token(&token_list, args);
		t_token *current = token_list;
		while(current)
		{
			ft_printf("The content of the node is %s and its type is %d", current->content, current->type);
			current = current->next;
		}
	}
	return (0);
}
