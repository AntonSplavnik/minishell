/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 14:38:21 by abillote          #+#    #+#             */
/*   Updated: 2025/01/02 17:46:21 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strstr(const char *big, const char *little)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (little[j] == '\0')
		return (0);
	while (big[i] != '\0')
	{
		j = 0;
		while (big[i + j] == little[j] && big[i + j])
		{
			j++;
			if (little[j] == '\0')
				return (1);
		}
		i++;
	}
	return (0);
}
