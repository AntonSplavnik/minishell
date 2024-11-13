/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:06:52 by abillote          #+#    #+#             */
/*   Updated: 2024/11/13 15:19:27 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_command(char *input)
{
	if (ft_strcmp(input, "echo") == 0)
		return (1);
	if (ft_strcmp(input, "cd") == 0)
		return (1);
	if (ft_strcmp(input, "pwd") == 0)
		return (1);
	if (ft_strcmp(input, "export") == 0)
		return (1);
	if (ft_strcmp(input, "unset") == 0)
		return (1);
	if (ft_strcmp(input, "unset") == 0)
		return (1);
	if (ft_strcmp(input, "env") == 0)
		return (1);
	if (ft_strcmp(input, "exit") == 0)
		return (1);
	else
		return (0);
}
