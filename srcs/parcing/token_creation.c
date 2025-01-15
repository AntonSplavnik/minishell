/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:21:21 by abillote          #+#    #+#             */
/*   Updated: 2025/01/15 12:11:45 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Determines if next token should be treated as a command:
- First token in the list is always a command
- First token after a pipe is a command
- Checks for NULL list and traverses to find last token
Returns: 1 if next token should be command, 0 otherwise
*/
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

/*
Determines the type of token based on its content and context:
- Uses get_heredoc_state_ptr to access heredoc state
- Special handling for heredoc operator (<<) and its delimiter
- Identifies commands using is_command()
- Falls back to get_other_types() for standard tokens
Returns: Appropriate token type from t_token_type enum
*/
t_token_type	get_token_type(char *input, t_token **token_list)
{
	int	*expect_heredoc_delim;

	expect_heredoc_delim = get_heredoc_state_ptr();
	if (*expect_heredoc_delim)
	{
		*expect_heredoc_delim = 0;
		return (TYPE_HEREDOC_DELIM);
	}
	if (ft_strcmp(input, "<<") == 0)
	{
		*expect_heredoc_delim = 1;
		return (TYPE_HEREDOC_OP);
	}
	if (is_command(token_list) == 1)
		return (TYPE_COMMAND);
	return (get_other_types(input));
}

/*
Creates a new token node:
- Allocates memory for token structure
- Duplicates input string for token content
- Initializes type and next pointer
- Handles memory allocation failures
Returns: New token node or NULL if allocation fails
*/
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

/*
Adds a new token to the end of token list:
- Creates new token using create_token()
- Handles empty list case
- Traverses to find last node for non-empty list
- Links new token into list
Returns: SUCCESS or ERR_MALLOC if creation fails
*/
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

/*
Main tokenization function that processes input string:
- Iterates through input character by character
- Extracts tokens using ft_split_token()
- Determines token type and adds to list
- Special handling for heredoc content
- Frees temporary strings and handles errors
Returns: SUCCESS or appropriate error code
*/

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
		free(token);
		if (error == SUCCESS)
		{
			last = get_last_token(*token_list);
			if (last && last->type == TYPE_HEREDOC_DELIM)
				error = handle_heredoc(token_list, last->content, &i, args);
		}
		if (error != SUCCESS)
			return (handle_error_free_tokens(error, token_list, NULL));
	}
	return (SUCCESS);
}
