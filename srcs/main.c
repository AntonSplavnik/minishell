/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:02:10 by abillote          #+#    #+#             */
/*   Updated: 2024/11/07 14:17:33 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	char	*input;

	while (1)
	{
		//store input with readline
		input = readline("minishell$ ");
		//add input to history if it's not empty
		if (input[0] != '\0')
		{
			add_history(input);
		}
	}
	return (0);
}
