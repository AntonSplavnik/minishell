/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:02:10 by abillote          #+#    #+#             */
/*   Updated: 2024/11/13 15:39:30 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	t_token	*token_list;
	char	*args;

	token_list = NULL;
	while (1)
	{
		//store input with readline
		args = readline("minishell$ ");
		if (!args)
			break ;
		//add raw_input to history if it's not empty
		if (args[0] != '\0')
		{
			add_history(args);
		}
		input_to_token(&token_list, args);

		//print token to check our code
		print_token(token_list);

		//free functions
		free_token_list(&token_list);
		free(args);
	}
	return (0);
}
