/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:02:10 by abillote          #+#    #+#             */
/*   Updated: 2024/11/15 17:31:59 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_token	*token_list;
	char	*args;
	t_error	error;
	t_env	*env_list;

	(void) argv;
	token_list = NULL;
	env_list = NULL;
	if (argc != 1)
	{
		print_error(ERR_LAUNCH);
		return (0);
	}
	//store env
		error = init_env(&env_list, env);
		if (error != SUCCESS)
			return (1) ;
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

		//print env variable to check our code
		// print_env(env_list);
		//print token to check our code
		print_token(token_list);

		//free functions
		free_token_list(&token_list);
		free(args);
	}
	free_env_list(&env_list);
	return (0);
}
