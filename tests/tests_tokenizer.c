/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 12:28:42 by abillote          #+#    #+#             */
/*   Updated: 2024/11/19 17:58:02 by abillote         ###   ########.fr       */
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
int	test_single_command_pwd(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "pwd";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && !verify_token(token_list, "pwd", TYPE_COMMAND))
		success = 0;
	print_test_result("Single command pwd without arguments", success);
	free_token_list(&token_list);
	return (success);
}
int	test_single_command_ls(void)
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
	print_test_result("single command ls", success);
	free_token_list(&token_list);
	return (success);
}

int	test_single_command_echo(void)
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
	print_test_result("single command echo", success);
	free_token_list(&token_list);
	return (success);
}

int	test_single_command_echo_flag(void)
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
	print_test_result("single command echo with flag", success);
	free_token_list(&token_list);
	return (success);
}
int	test_basic_double_quotes(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo \"hello\"";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo" ,TYPE_COMMAND) || \
		!verify_token(token_list->next, "\"hello\"", TYPE_ARG)))
		success = 0;
	print_test_result("Basic double quotes", success);
	free_token_list(&token_list);
	return (success);
}

int	test_basic_single_quotes(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo 'hello'";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo" ,TYPE_COMMAND) || \
		!verify_token(token_list->next, "'hello'", TYPE_ARG)))
		success = 0;
	print_test_result("Basic single quotes", success);
	free_token_list(&token_list);
	return (success);
}

int	test_missing_last_single_quotes(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo 'hello";
	success = 0;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 1;
	print_test_result("Missing last single quote", success);
	free_token_list(&token_list);
	return (success);
}

int	test_missing_first_single_quotes(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo hello'";
	success = 0;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 1;
	print_test_result("Missing first single quote", success);
	free_token_list(&token_list);
	return (success);
}

int	test_missing_last_double_quotes(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo \"hello";
	success = 0;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 1;
	print_test_result("Missing last double quote", success);
	free_token_list(&token_list);
	return (success);
}

int	test_missing_first_double_quotes(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo hello\"";
	success = 0;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 1;
	print_test_result("Missing first double quote", success);
	free_token_list(&token_list);
	return (success);
}

int test_input_redirection(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "cat < input.txt";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "cat", TYPE_COMMAND) ||
		!verify_token(token_list->next, "<", TYPE_REDIRIN) ||
		!verify_token(token_list->next->next, "input.txt", TYPE_ARG)))
		success = 0;
	print_test_result("Basic input redirection", success);
	free_token_list(&token_list);
	return (success);
}

int test_input_redirection_without_spaces(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "cat<input.txt";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "cat", TYPE_COMMAND) ||
		!verify_token(token_list->next, "<", TYPE_REDIRIN) ||
		!verify_token(token_list->next->next, "input.txt", TYPE_ARG)))
		success = 0;
	print_test_result("Input redirection without spaces", success);
	free_token_list(&token_list);
	return (success);
}

int test_input_redirection_double_quotes(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo \"< input.txt\"";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo", TYPE_COMMAND) ||
		!verify_token(token_list->next, "\"< input.txt\"", TYPE_ARG)))
		success = 0;
	print_test_result("Input redirection with double quotes", success);
	//print_token(token_list);
	free_token_list(&token_list);
	return (success);
}

int test_input_redirection_single_quotes(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo \'< input.txt\'";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo", TYPE_COMMAND) ||
		!verify_token(token_list->next, "\'< input.txt\'", TYPE_ARG)))
		success = 0;
	print_test_result("Input redirection with single quotes", success);
	//print_token(token_list);
	free_token_list(&token_list);
	return (success);
}

int test_output_redirection(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo hello > output.txt";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo", TYPE_COMMAND) ||
		!verify_token(token_list->next, "hello", TYPE_ARG) ||
		!verify_token(token_list->next->next, ">", TYPE_REDIROUT) ||
		!verify_token(token_list->next->next->next, "output.txt", TYPE_ARG)))
		success = 0;
	print_test_result("Basic output redirection", success);
	free_token_list(&token_list);
	return (success);
}

int test_output_redirection_without_spaces(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo hello>output.txt";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo", TYPE_COMMAND) ||
		!verify_token(token_list->next, "hello", TYPE_ARG) ||
		!verify_token(token_list->next->next, ">", TYPE_REDIROUT) ||
		!verify_token(token_list->next->next->next, "output.txt", TYPE_ARG)))
		success = 0;
	print_test_result("Output redirection without spaces", success);
	free_token_list(&token_list);
	return (success);
}

int test_output_redirection_double_quotes(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo \"hello>output.txt\"";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo", TYPE_COMMAND) ||
		!verify_token(token_list->next, "\"hello>output.txt\"", TYPE_ARG)))
		success = 0;
	print_test_result("Output redirection double quote", success);
	free_token_list(&token_list);
	return (success);
}
int test_output_redirection_double_quotes_2(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo hello>\"output.txt\"";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo", TYPE_COMMAND) ||
		!verify_token(token_list->next, "hello", TYPE_ARG) ||
		!verify_token(token_list->next->next, ">", TYPE_REDIROUT) ||
		!verify_token(token_list->next->next->next, "\"output.txt\"", TYPE_ARG)))
		success = 0;
	print_test_result("Output redirection double quotes around output", success);
	free_token_list(&token_list);
	return (success);
}

int test_output_redirection_single_quotes(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo \'hello>output.txt\'";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo", TYPE_COMMAND) ||
		!verify_token(token_list->next, "\'hello>output.txt\'", TYPE_ARG)))
		success = 0;
	print_test_result("Output redirection single quote", success);
	free_token_list(&token_list);
	return (success);
}
int test_output_redirection_single_quotes_2(void)
{
	t_token	*token_list;
	char	*input;
	int		success;

	token_list = NULL;
	input = "echo hello>\'output.txt\'";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo", TYPE_COMMAND) ||
		!verify_token(token_list->next, "hello", TYPE_ARG) ||
		!verify_token(token_list->next->next, ">", TYPE_REDIROUT) ||
		!verify_token(token_list->next->next->next, "\'output.txt\'", TYPE_ARG)))
		success = 0;
	print_test_result("Output redirection single quotes around output", success);
	free_token_list(&token_list);
	return (success);
}

int test_simple_pipe(void)
{
	t_token *token_list;
	char *input;
	int success;

	token_list = NULL;
	input = "ls | wc -l";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "ls", TYPE_COMMAND) ||
		!verify_token(token_list->next, "|", TYPE_PIPE) ||
		!verify_token(token_list->next->next, "wc", TYPE_COMMAND) ||
		!verify_token(token_list->next->next->next, "-l", TYPE_ARG)))
		success = 0;
	print_test_result("Simple pipe", success);
	free_token_list(&token_list);
	return (success);
}

int test_env_variable(void)
{
	t_token *token_list;
	char *input;
	int success;

	token_list = NULL;
	input = "echo $HOME";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo", TYPE_COMMAND) ||
		!verify_token(token_list->next, "$HOME", TYPE_ARG)))
		success = 0;
	print_test_result("Environment variable", success);
	free_token_list(&token_list);
	return (success);
}

int test_env_variable_double_quotes(void)
{
	t_token *token_list;
	char *input;
	int success;

	token_list = NULL;
	input = "echo \"$HOME\"";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo", TYPE_COMMAND) ||
		!verify_token(token_list->next, "\"$HOME\"", TYPE_ARG)))
		success = 0;
	print_test_result("Environment variable with double quotes", success);
	free_token_list(&token_list);
	return (success);
}

int test_env_variable_single_quotes(void)
{
	t_token *token_list;
	char *input;
	int success;

	token_list = NULL;
	input = "echo \'$HOME\'";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "echo", TYPE_COMMAND) ||
		!verify_token(token_list->next, "\'$HOME\'", TYPE_ARG)))
		success = 0;
	print_test_result("Environment variable with single quotes", success);
	free_token_list(&token_list);
	return (success);
}

int test_complex_command(void)
{
	t_token *token_list;
	char *input;
	int success;

	token_list = NULL;
	input = "cat < input.txt | grep 'pattern' > output.txt";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "cat", TYPE_COMMAND)))
		success = 0;
	if (success && (!verify_token(token_list->next, "<", TYPE_REDIRIN)))
		success = 0;
	if (success && (!verify_token(token_list->next->next, "input.txt", TYPE_ARG)))
		success = 0;
	if (success && (!verify_token(token_list->next->next->next, "|", TYPE_PIPE)))
		success = 0;
	if (success && (!verify_token(token_list->next->next->next->next, "grep", TYPE_COMMAND)))
		success = 0;
	if (success && (!verify_token(token_list->next->next->next->next->next, "'pattern'", TYPE_ARG)))
		success = 0;
	if (success && (!verify_token(token_list->next->next->next->next->next->next, ">", TYPE_REDIROUT)))
		success = 0;
	if (success && (!verify_token(token_list->next->next->next->next->next->next->next, "output.txt", TYPE_ARG)))
		success = 0;
	print_test_result("Complex command", success);
	//print_token(token_list);
	free_token_list(&token_list);
	return (success);
}

int	test_heredoc_basic()
{
	t_token *token_list;
	char *input;
	int success;

	token_list = NULL;
	input = "cat << EOF";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "cat", TYPE_COMMAND)))
		success = 0;
	if (success && (!verify_token(token_list->next, "<<", TYPE_HEREDOC)))
		success = 0;
	if (success && (!verify_token(token_list->next->next, "EOF", TYPE_ARG)))
		success = 0;
	print_test_result("Basic heredoc", success);
	free_token_list(&token_list);
	return (success);
}

int test_complex_heredoc(void)
{
	t_token *token_list;
	char *input;
	int success;

	token_list = NULL;
	input = "cat << DELIM | grep 'pattern' > output.txt";
	success = 1;
	if (input_to_token(&token_list, input) != SUCCESS)
		success = 0;
	if (success && (!verify_token(token_list, "cat", TYPE_COMMAND)))
		success = 0;
	if (success && (!verify_token(token_list->next, "<<", TYPE_HEREDOC)))
		success = 0;
	if (success && (!verify_token(token_list->next->next, "DELIM", TYPE_ARG)))
		success = 0;
	if (success && (!verify_token(token_list->next->next->next, "|", TYPE_PIPE)))
		success = 0;
	if (success && (!verify_token(token_list->next->next->next->next, "grep", TYPE_COMMAND)))
		success = 0;
	if (success && (!verify_token(token_list->next->next->next->next->next, "'pattern'", TYPE_ARG)))
		success = 0;
	if (success && (!verify_token(token_list->next->next->next->next->next->next, ">", TYPE_REDIROUT)))
		success = 0;
	if (success && (!verify_token(token_list->next->next->next->next->next->next->next, "output.txt", TYPE_ARG)))
		success = 0;
	print_test_result("Complex heredoc", success);
	free_token_list(&token_list);
	return (success);
}

int	main(void)
{
	int	count = 0;
	int	success = 0;

	printf("-----Tokenizer Tests-----\n\n");
	if (test_single_command_pwd())
	{
		count++;
		success++;
	}
	if (test_single_command_ls())
	{
		count++;
		success++;
	}
	if (test_single_command_echo())
	{
		count++;
		success++;
	}
	if (test_single_command_echo_flag())
	{
		count++;
		success++;
	}
	if (test_basic_double_quotes())
	{
		count++;
		success++;
	}
	if (test_basic_single_quotes())
	{
		count++;
		success++;
	}
	if (test_missing_last_single_quotes())
	{
		count++;
		success++;
	}
	if (test_missing_first_single_quotes())
	{
		count++;
		success++;
	}
	if (test_missing_last_double_quotes())
	{
		count++;
		success++;
	}
	if (test_missing_first_double_quotes())
	{
		count++;
		success++;
	}
	if (test_input_redirection())
	{
		count++;
		success++;
	}
	if (test_input_redirection_without_spaces())
	{
		count++;
		success++;
	}
	if (test_input_redirection_double_quotes())
	{
		count++;
		success++;
	}
	if (test_input_redirection_single_quotes())
	{
		count++;
		success++;
	}
	if (test_output_redirection())
	{
		count++;
		success++;
	}
	if (test_output_redirection_without_spaces())
	{
		count++;
		success++;
	}
	if (test_output_redirection_double_quotes())
	{
		count++;
		success++;
	}
	if (test_output_redirection_double_quotes_2())
	{
		count++;
		success++;
	}
	if (test_output_redirection_single_quotes())
	{
		count++;
		success++;
	}
	if (test_output_redirection_single_quotes_2())
	{
		count++;
		success++;
	}
	if (test_simple_pipe())
	{
		count++;
		success++;
	}
	if (test_env_variable())
	{
		count++;
		success++;
	}
	if (test_env_variable_double_quotes())
	{
		count++;
		success++;
	}
	if (test_env_variable_single_quotes())
	{
		count++;
		success++;
	}
	if (test_complex_command())
	{
		count++;
		success++;
	}
	if (test_heredoc_basic())
	{
		count++;
		success++;
	}
	if (test_complex_heredoc())
	{
		count++;
		success++;
	}
	printf("SUCCESS : %d out of %d\n", success, count);
	if (success == count)
		printf("ALL GOOD, CONGRATS!\n\n");
	else
		printf("SOME ERRORS, TRY AGAIN!\n\n");
	printf("-----Tests Completed-----\n");
	return (0);
}
