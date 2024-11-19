/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:06:52 by abillote          #+#    #+#             */
/*   Updated: 2024/11/18 17:34:18 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Check if the token type is a command*/
//int	is_builtin(char *input)
//{
//	if (ft_strcmp(input, "echo") == 0)
//		return (1);
//	if (ft_strcmp(input, "cd") == 0)
//		return (1);
//	if (ft_strcmp(input, "pwd") == 0)
//		return (1);
//	if (ft_strcmp(input, "export") == 0)
//		return (1);
//	if (ft_strcmp(input, "unset") == 0)
//		return (1);
//	if (ft_strcmp(input, "unset") == 0)
//		return (1);
//	if (ft_strcmp(input, "env") == 0)
//		return (1);
//	if (ft_strcmp(input, "exit") == 0)
//		return (1);
//	else
//		return (0);
//}

//Each first word and each word after a pipe is a command
int is_command(t_token **token_list)
{
	t_token *current;

	current = *token_list;
	if (!token_list || !*token_list)
		return (1);
	while (current->next)
		current = current->next;
	if (current ->type == TYPE_PIPE)
		return (1);
	return (0);
}
