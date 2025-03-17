/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:02:10 by abillote          #+#    #+#             */
/*   Updated: 2025/03/17 15:15:08 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define PROMPT_COLOR "\001\033[1;36m\002"	// Cyan
#define RESET_COLOR "\001\033[0m\002"		// Reset to default

int	g_sig = 0;

/*
Gets user input from readline:
- Displays shell prompt
- Handles EOF (Ctrl+D) by setting exit status
- Adds non-empty inputs to command history
Returns: Input string or NULL on EOF/error
*/
static char	*get_input(t_shell *s)
{
	char	*args;

	args = readline(PROMPT_COLOR "minishell$ " RESET_COLOR);
	if (g_sig == SIGINT)
	{
		s->exit_status = 130;
		g_sig = 0;
	}
	if (!args)
	{
		s->exit_status = 0;
		return (NULL);
	}
	if (args[0] != '\0')
		add_history(args);
	return (args);
}

/*
Processes user input through the shell pipeline:
- Converts input to tokens
- Validates last token type
- Expands variables and special characters
- Handles errors and memory cleanup
Returns: SUCCESS or appropriate error code
*/
static t_error	process_input(t_shell *s, char *args)
{
	t_error	error;
	t_token	*last;

	error = input_to_token(&s->token_list, args);
	if (error != SUCCESS)
		return (error);
	last = get_last_token(s->token_list);
	if ((s->token_list && ft_strcmp(s->token_list->content, "|") == 0) \
	|| (last && (last->type == TYPE_HEREDOC_OP || \
		!ft_strcmp(last->content, "|") || !ft_strcmp(last->content, ">>") \
		|| !ft_strcmp(last->content, ">") || !ft_strcmp(last->content, "<<") \
		|| !ft_strcmp(last->content, "<"))))
	{
		if (ft_strcmp(last->content, "|") == 0 || \
			!ft_strcmp(s->token_list->content, "|"))
			error = ERR_PARSING_PIPE;
		else
			error = ERR_PARSING_REDIR;
		handle_error_free_tokens(error, &s->token_list, NULL);
		return (error);
	}
	error = expand_tokens(s);
	if (error != SUCCESS)
		handle_error_free_tokens(error, &s->token_list, NULL);
	return (error);
}

static int	process_command(t_shell *s, char *args)
{
	t_error	error;

	if (only_whitespace(args))
	{
		free(args);
		s->exit_status = 0;
		return (1);
	}
	error = process_input(s, args);
	if (error == ERR_SIGNAL)
		return (cleanup_signals(s, args));
	if (error != SUCCESS)
		return (set_exit_status(error, s, args));
	if (s->token_list)
		error = execute_command(s);
	if (error == ERR_SIGNAL)
		return (cleanup_signals(s, args));
	if (s->exit_status == 127)
		error = ERR_CMD_NOT_FOUND;
	if (error != SUCCESS)
		return (handle_error_free_tokens(error, &s->token_list, args));
	free_token_list(&s->token_list);
	free(args);
	return (1);
}

static int	handle_loop_iteration(t_shell *s)
{
	char	*args;

	if (set_signals_interactive())
		return (0);
	args = get_input(s);
	if (!args)
		return (0);
	return (process_command(s, args));
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
	free_array(s->envp);
	free(s);
	rl_clear_history();
	return (0);
}
