/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:06:52 by abillote          #+#    #+#             */
/*   Updated: 2024/11/19 18:08:53 by abillote         ###   ########.fr       */
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

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || \
			c == '\v' || c == '\f' || c == '\r');
}

int	is_delimiter(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

int	get_delimiter_len(const char *str)
{
	if (!str || !*str)
		return (0);
	if ((*str == '<' && *(str + 1) == '<') || \
		(*str == '>' && *(str + 1) == '>'))
		return (2);
	if (is_delimiter(*str))
		return (1);
	return (0);
}
