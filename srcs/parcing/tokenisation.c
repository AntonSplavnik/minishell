/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:21:21 by abillote          #+#    #+#             */
/*   Updated: 2024/11/18 12:14:43 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Takes the input from readline, split it
and store each part into tokens in a token list.
Return an error status*/
t_error	input_to_token(t_token **token_list, char *args)
{
	char	**splitted_arg;
	int		i;

	i = 0;
	splitted_arg = ft_split(args, ' ');
	if (!splitted_arg)
		return (handle_error(ERR_PARCING, NULL));
	while (splitted_arg[i])
	{
		if (add_token(token_list, splitted_arg[i], \
				get_token_type(splitted_arg[i])) != SUCCESS)
		{
			while (splitted_arg[i])
				free(splitted_arg[i++]);
			free(splitted_arg);
			return (handle_error_free_tokens(ERR_PARCING, token_list, NULL));
		}
		i++;
	}
	i = 0;
	while (splitted_arg[i])
		free(splitted_arg[i++]);
	free(splitted_arg);
	return (SUCCESS);
}

/*Create and return a token node with its type and content*/
t_token	*create_token(char *input, t_token_type type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->content = ft_strdup(input);
	if (!new_token->content)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

/*Add a new token to the list of tokens
Return an error status*/
t_error	add_token(t_token **token_list, char *input, t_token_type type)
{
	t_token	*new_token;
	t_token	*current;

	new_token = create_token(input, type);
	if (!new_token)
		return (ERR_PARCING);
	if (*token_list == NULL)
	{
		*token_list = new_token;
		return (SUCCESS);
	}
	current = *token_list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_token;
	return (SUCCESS);
}

/*Return the type of the token according to its content*/
t_token_type	get_token_type(char *input)
{
	if (is_command(input) == 1)
		return (TYPE_COMMAND);
	if (ft_strcmp(input, "\"") == 0)
		return (TYPE_DQUOTE);
	if (ft_strcmp(input, "'") == 0)
		return (TYPE_SQUOTE);
	if (ft_strcmp(input, "|") == 0)
		return (TYPE_PIPE);
	if (ft_strcmp(input, "<") == 0)
		return (TYPE_REDIRIN);
	if (ft_strcmp(input, ">") == 0)
		return (TYPE_REDIROUT);
	if (ft_strcmp(input, ">>") == 0)
		return (TYPE_REDIRAPPEND);
	if (ft_strcmp(input, "<<") == 0)
		return (TYPE_HEREDOC);
	if (ft_strcmp(input, "") == 0)
		return (TOKEN_EMPTY);
	else
		return (TYPE_ARG);
}
