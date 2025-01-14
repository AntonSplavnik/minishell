/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:02:10 by abillote          #+#    #+#             */
/*   Updated: 2025/01/14 12:19:06 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_shell	*s;
	char	*args;
	t_error	error;
	t_token	*last;

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
	s->exit_status = 0;

	//store env
	error = init_env(&s->env_list, env);
	if (error != SUCCESS)
	{
		free(s);
		s->exit_status = 2;
		return (1);
	}
	while (1)
	{
		//store input with readline
		args = readline("minishell$ ");
		if (!args)
		{
			s->exit_status = 2;
			break ;
		}
		//add raw_input to history if it's not empty
		if (args[0] != '\0')
		{
			add_history(args);
		}
		//split input into tokens
		error = input_to_token(&s->token_list, args);
		last = get_last_token(s->token_list);
		if (last && (last->type == TYPE_HEREDOC_OP || last->type == TYPE_PIPE || last->type == TYPE_REDIRAPPEND || \
		last->type == TYPE_REDIRIN || last->type == TYPE_REDIROUT))
		{
			error = ERR_PARCING;
			handle_error_free_tokens(error, &s->token_list, args);
			s->token_list = NULL;
			continue ;
		}
		if (error != SUCCESS)
		{
			free(args);
			s->exit_status = 2;
			continue ;
		}

		//expand tokens (env var expansion & quotes cleaning)
		if (error == SUCCESS)
			error = expand_tokens(s);
		if (error != SUCCESS)
		{
			handle_error_free_tokens(error, &s->token_list, args);
			s->token_list = NULL;
			s->exit_status = 2;
			continue ;
		}

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
