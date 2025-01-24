/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:08:53 by abillote          #+#    #+#             */
/*   Updated: 2025/01/24 17:28:14 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Prints appropriate error message to stderr based on error code:
- SUCCESS: Operation completed successfully
- ERR_LAUNCH: Incorrect program usage
- ERR_MALLOC: Memory allocation failure
- ERR_PARCING: Syntax error in input
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
		ft_putendl_fd("Error: Please enter \
		./minishell without any arguments", 2);
	else if (error_code == 2)
		ft_putendl_fd("Error: Memory allocation failed", 2);
	else if (error_code == 3)
		ft_putendl_fd("Error: Syntax error", 2);
	else if (error_code == 4)
		ft_putendl_fd("Error: Environment could not be set", 2);
	else if (error_code == 5)
		ft_putendl_fd("Error: Command could not be found", 2);
	else if (error_code == 6)
		ft_putendl_fd("Error: Command could not be executed", 2);
	else if (error_code == 7)
		ft_putendl_fd("Error pwd: too many arguments", 2);
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
