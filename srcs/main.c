/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:02:10 by abillote          #+#    #+#             */
/*   Updated: 2024/11/07 17:06:15 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	t_command	*command_list;
	t_tockens	*tokens;
	char		*raw_input;

	command_list = malloc(sizeof(t_command));;
	tokens = malloc(sizeof(t_tockens));
	while (1)
	{
		//store input with readline
		raw_input = readline("minishell$ ");
		//add raw_input to history if it's not empty
		if (raw_input[0] != '\0')
		{
			add_history(raw_input);
		}

		tokens->content = ft_strdup("echo");
		tokens->type = ft_strdup("function");

		command_list->args = malloc(2 * sizeof(char *));
		if (command_list->args == NULL)
			exit (1);
		command_list->splitted_input = ft_split(raw_input, ' ');

		int j = 0;
		while (command_list->splitted_input[j])
		{
			printf("here you go: %s\n", command_list->splitted_input[j]);
			fflush(stdout);
			j++;
		}


		int	i;
		i = 0;
		while (command_list->splitted_input[i])
		{
			if (ft_strncmp(command_list->splitted_input[i], tokens->content, ft_strlen(tokens->content)) && ft_strncmp("function", tokens->type, ft_strlen(tokens->type)) == 0)
				command_list->function = ft_strdup(command_list->splitted_input[i]);
			else if (command_list->splitted_input[i] != tokens->content)
			{
				command_list->args[0] = ft_strdup(command_list->splitted_input[i]);
				command_list->args[1] = NULL;
			}
			i++;
		}

		printf ("Funciton: %s\n", command_list->function);
		printf ("args: %s\n", command_list->args[0]);

		free (raw_input);

	}
	return (0);
}
