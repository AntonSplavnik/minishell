/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:02:10 by abillote          #+#    #+#             */
/*   Updated: 2025/01/15 11:47:28 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_input(t_shell *s)
{
	char	*args;

	args = readline("minishell$ ");
	if (!args)
	{
		s->exit_status = 2;
		return (NULL);
	}
	if (args[0] != '\0')
		add_history(args);
	return (args);
}

static t_error	process_input(t_shell *s, char *args)
{
	t_error	error;
	t_token	*last;

	error = input_to_token(&s->token_list, args);
	if (error != SUCCESS)
		return (error);
	last = get_last_token(s->token_list);
	if (last && (last->type == TYPE_HEREDOC_OP || last->type == TYPE_PIPE \
	|| last->type == TYPE_REDIRAPPEND || last->type == TYPE_REDIRIN \
	|| last->type == TYPE_REDIROUT))
	{
		error = ERR_PARCING;
		handle_error_free_tokens(error, &s->token_list, args);
		return (error);
	}
	error = expand_tokens(s);
	if (error != SUCCESS)
		handle_error_free_tokens(error, &s->token_list, args);
	return (error);
}

static int	handle_loop_iteration(t_shell *s)
{
	char	*args;
	t_error	error;

	args = get_input(s);
	if (!args)
		return (0);
	error = process_input(s, args);
	if (error != SUCCESS)
	{
		s->token_list = NULL;
		s->exit_status = 2;
		return (1);
	}
	//to delete before submitting
	print_token(s->token_list);

	free_token_list(&s->token_list);
	free(args);
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	*s;
	t_error	error;

	(void) argv;
	s = NULL;
	if (argc != 1)
	{
		print_error(ERR_LAUNCH);
		return (0);
	}
	error = initialize_shell(&s, env);
	if (error != SUCCESS)
		return (1);
	while (handle_loop_iteration(s))
		;
	free_env_list(&s->env_list);
	free(s);
	return (0);
}
