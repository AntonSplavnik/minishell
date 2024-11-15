/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:02:10 by abillote          #+#    #+#             */
/*   Updated: 2024/11/15 13:48:34 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	t_token	*token_list;
	char	*args;
	t_error	error;

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
		error = input_to_token(&token_list, args);
		if (error != SUCCESS)
		{
			free(args);
			continue ;
		}

		//print token to check our code
		print_token(token_list);

		//free functions
		free_token_list(&token_list);
		free(args);
	}
	return (0);
}
// testing brach