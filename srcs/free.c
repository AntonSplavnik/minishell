/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:43:11 by abillote          #+#    #+#             */
/*   Updated: 2025/03/18 12:53:39 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*In case of exit: free all (including array if needed) and clear history*/
void	free_all(t_shell *s, char **array)
{
	if (array)
		free_array(array);
	if (s->token_list)
		free_token_list(&s->token_list);
	if (s->env_list)
		free_env_list(&s->env_list);
	if (s->envp)
		free_array(s->envp);
	free (s);
	rl_clear_history();
}

/*Free command path and args array*/
void	free_command_path(char *cmd_path, char **args)
{
	if (cmd_path)
		free(cmd_path);
	if (args)
		free_array(args);
}

/*Free an array of string*/

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/*
Frees entire token linked list:
- Iterates through list freeing each node
- Frees both node content and node itself
- Sets list pointer to NULL after freeing
*/
void	free_token_list(t_token **token_list)
{
	t_token	*temp;

	while (*token_list)
	{
		temp = *token_list;
		*token_list = (*token_list)->next;
		free(temp->content);
		free(temp);
	}
	*token_list = NULL;
}

/*
Frees entire environment variable linked list:
- Iterates through list freeing each node
- Frees key, value, and node itself
- Sets list pointer to NULL after freeing
*/
void	free_env_list(t_env **env_list)
{
	t_env	*temp;

	while (*env_list)
	{
		temp = *env_list;
		*env_list = (*env_list)->next;
		free(temp->content);
		free(temp->key);
		free(temp->value);
		free(temp);
	}
	*env_list = NULL;
}
