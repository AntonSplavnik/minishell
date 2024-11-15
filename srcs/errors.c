/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:08:53 by abillote          #+#    #+#             */
/*   Updated: 2024/11/15 16:02:20 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//print error message in stderr
void	print_error(t_error error_code)
{
	if (error_code == 0)
		ft_putendl_fd("SUCCESS\n", 2);
	else if (error_code == 1)
		ft_putendl_fd("Please enter ./minishell without any arguments\n", 2);
	else if (error_code == 2)
		ft_putendl_fd("Memory allocation failed\n", 2);
	else if (error_code == 3)
		ft_putendl_fd("Parcing failed\n", 2);
}

//free a pointer and print error message
t_error	handle_error(t_error error_code, void *ptr_to_free)
{
	if (ptr_to_free)
		free(ptr_to_free);
	print_error(error_code);
	return (error_code);
}

//free the token list and print error message
t_error	handle_error_free_all(t_error error_code, \
								t_token **token_list, char *args)
{
	if (token_list)
		free_token_list(token_list);
	if (args)
		free(args);
	print_error(error_code);
	return (error_code);
}
