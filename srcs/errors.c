/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:08:53 by abillote          #+#    #+#             */
/*   Updated: 2025/01/31 16:34:11 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error_exit(char *function, char *arg, char *message)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(function, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(message, 2);
}

/*
Prints appropriate error message to stderr based on error code:
- SUCCESS: Operation completed successfully
- ERR_LAUNCH: Incorrect program usage
- ERR_MALLOC: Memory allocation failure
- ERR_parsing: Syntax error in input
- ERR_ENV: Environment initialization error
- ERR_CMD_NOT_FOUND: Error in the command name
- ERR_EXEC: Error during the command execution
- ERR_PWD: Too many args for pwd
*/
void	print_error(t_error error_code)
{
	if (error_code == 0)
		ft_putendl_fd("SUCCESS", 2);
	else if (error_code == 1)
		ft_putendl_fd("Please enter \
		./minishell without any arguments", 2);
	else if (error_code == 2)
		ft_putendl_fd("Memory allocation failed", 2);
	else if (error_code == 3)
		ft_putendl_fd("Syntax error", 2);
	else if (error_code == 4)
		ft_putendl_fd("Environment could not be set", 2);
	else if (error_code == 5)
		ft_putendl_fd("Command not found", 2);
	else if (error_code == 6)
		ft_putendl_fd("Command could not be executed", 2);
	else if (error_code == 7)
		ft_putendl_fd("minishell: env: No such file or directory", 2);
	else if (error_code == 8)
		ft_putendl_fd("minishell: cd: No such file or directory", 2);
	else if (error_code == 9)
		ft_putendl_fd("minishell: exit: too many arguments", 2);
}

/*
Handles error with single pointer cleanup:
- Frees provided pointer if not NULL
- Prints error message
Returns: The error code passed in
*/
t_error	handle_error(t_error error_code, void *ptr_to_free)
{
	if (ptr_to_free)
		free(ptr_to_free);
	print_error(error_code);
	return (error_code);
}

/*
Handles error with token list cleanup:
- Frees entire token list
- Frees additional args string if provided
- Prints error message
Returns: The error code passed in
*/
t_error	handle_error_free_tokens(t_error error_code, \
								t_token **token_list, char *args)
{
	if (token_list)
		free_token_list(token_list);
	if (args)
		free(args);
	reset_heredoc_state();
	print_error(error_code);
	return (error_code);
}

t_error	handle_error_free_env(t_error error_code, \
								t_env **env_list)
{
	if (env_list)
		free_env_list(env_list);
	print_error(error_code);
	return (error_code);
}
