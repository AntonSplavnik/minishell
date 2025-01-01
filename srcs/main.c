/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:02:10 by abillote          #+#    #+#             */
/*   Updated: 2025/01/01 13:17:07 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_shell	*s;
	char	*args;
	t_error	error;

	(void) argv;
	s = NULL;
	if (argc != 1)
	{
		print_error(ERR_LAUNCH);
		return (0);
	}

	//initialise t_shell struct
	s = malloc(sizeof(t_shell));
	if (!s)
		return 1;
	s->token_list = NULL;
	s->env_list = NULL;

	//store env
	error = init_env(&s->env_list, env);
	if (error != SUCCESS)
	{
		free(s);
		return (1);
	}
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
		//split input into tokens
		error = input_to_token(&s->token_list, args);
		if (error != SUCCESS)
		{
			free(args);
			continue ;
		}
		printf("after extraction ok\n");

		//expand tokens (env var expansion & quotes cleaning)
		//if (error == SUCCESS)
		//	error = expand_tokens(&s->token_list, s->env_list);
		//if (error != SUCCESS)
		//{
		//	handle_error_free_tokens(error, &s->token_list, args);
		//	continue ;
		//}

		//print env variable to check our code
		// print_env(&s->env_list);
		//print token to check our code
		print_token(s->token_list);

		//free functions
		free_token_list(&s->token_list);
		free(args);
	}
	free_env_list(&s->env_list);
	free(s);
	return (0);
}
