/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 12:28:42 by abillote          #+#    #+#             */
/*   Updated: 2024/11/16 16:35:54 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*This file is used to test our Tokenizer (Split+tokenisation files).
If a test fails, you can add print_tokens to the function in order to understand why*/

int	verify_token(t_token *token_list, char *expected_content, \
					t_token_type expected_type)
{
	if (!token_list)
		return (0);
	if (strcmp(expected_content, token_list->content) == 0 && \
		token_list->type == expected_type)
		return (1);
	else
		return (0);
}
void	print_test_result(char *test_name, int success)
{
	if (success == 0)
		printf("Test %s : FAILED\n\n", test_name);
	else
		printf("Test %s : SUCCESS\n\n", test_name);
}

void	test_basic_command_ls(void)
{
	t_token *token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "ls -la";
	success = 1;

	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "ls" ,TYPE_COMMAND) || \
		!verify_token(token_list->next, "-la", TYPE_ARG)))
		success = 0;
	print_test_result("Basic command ls", success);
	free_token_list(&token_list);
}

void	test_basic_command_echo(void)
{
	t_token *token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo hello";
	success = 1;

	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo" ,TYPE_COMMAND) || \
		!verify_token(token_list->next, "hello", TYPE_ARG)))
		success = 0;
	print_test_result("Basic command echo", success);
	free_token_list(&token_list);
}

void	test_basic_command_echo_flag(void)
{
	t_token *token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo -n hello";
	success = 1;

	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo" ,TYPE_COMMAND) || \
		!verify_token(token_list->next, "-n", TYPE_ARG) || \
		!verify_token(token_list->next->next, "hello", TYPE_ARG)))
		success = 0;
	print_test_result("Basic command echo with flag", success);
	free_token_list(&token_list);
}

int	main(void)
{
	printf("-----Tokenizer Tests-----\n\n");
	test_basic_command_ls();
	test_basic_command_echo();
	test_basic_command_echo_flag();
	printf("-----Tests Completed-----\n");
	return (0);
}
