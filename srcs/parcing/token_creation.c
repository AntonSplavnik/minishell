/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:21:21 by abillote          #+#    #+#             */
/*   Updated: 2025/01/02 19:20:46 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//Each first word and each word after a pipe is a command
int	is_command(t_token **token_list)
{
	t_token	*current;

	current = *token_list;
	if (!token_list || !*token_list)
		return (1);
	while (current->next)
		current = current->next;
	if (current ->type == TYPE_PIPE)
		return (1);
	return (0);
}

/*Return the type of the token according to its content
First check if there is previous token is a heredoc DELIM.
If yes, then current token is heredoc CONTENT.
Then check for other types.*/
t_token_type	get_token_type(char *input, t_token **token_list)
{
	static int	expect_heredoc_delim = 0;

	if (expect_heredoc_delim)
	{
		expect_heredoc_delim = 0;
		return (TYPE_HEREDOC_DELIM);
	}
	if (ft_strcmp(input, "<<") == 0)
	{
		expect_heredoc_delim = 1;
		return (TYPE_HEREDOC_OP);
	}
	if (is_command(token_list) == 1)
		return (TYPE_COMMAND);
	return (get_other_types(input));
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
		return (ERR_MALLOC);
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

/*Takes the input from readline, split it
and store each part into tokens in a token list.
Return an error status*/

t_error	input_to_token(t_token **token_list, char *args)
{
	size_t	i;
	t_error	error;
	char	*token;
	t_token	*last;

	i = 0;
	while (args[i])
	{
		error = ft_split_token(token_list, args, &i, &token);
		if (error != SUCCESS)
			return (error);
		if (!token)
			return (SUCCESS);
		error = add_token(token_list, token, get_token_type(token, token_list));
		if (error == SUCCESS)
		{
			last = get_last_token(*token_list);
			if (last && last->type == TYPE_HEREDOC_DELIM)
				error = handle_heredoc(token_list, last->content, &i, args);
		}
		if (error != SUCCESS)
			return (handle_error_free_tokens(error, token_list, NULL));
		free(token);
	}
	return (SUCCESS);
}
