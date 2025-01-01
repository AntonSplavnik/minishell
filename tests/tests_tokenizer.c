// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   tests_tokenizer.c                                  :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/11/16 12:28:42 by abillote          #+#    #+#             */
// /*   Updated: 2024/12/03 14:53:13 by abillote         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../includes/minishell.h"

// /*This file is used to test our Tokenizer (Files starting with "token").

//Here we define colors to make test results more readable
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

//test structure to easily add new test
typedef struct s_test {
	char *name;
	char *input;
	t_token_type expected_types[10];	// Array of expected token types
	char *expected_contents[10];	// Array of expected token contents
	int token_count;				// Number of tokens to verify
	int expect_fail; //for test that we need to fail (syntax error)
} t_test;

//tests array to store all the tests for tokenizer - you can add a new test at the bottom
t_test tests_tokenizer[] = {
	//{"Basic pwd", "pwd", {TYPE_COMMAND}, {"pwd"}, 1, 0},
	//{"Basic ls", "ls -la", {TYPE_COMMAND, TYPE_ARG}, {"ls", "-la"}, 2, 0},
	//{"Basic echo", "echo hi", {TYPE_COMMAND, TYPE_ARG}, {"echo", "hi"}, 2, 0},
	//{"Basic echo + newline", "echo hi\n", {TYPE_COMMAND, TYPE_ARG, TYPE_ARG}, {"echo", "hi", "\n"}, 3, 0},
	//{"Basic echo + flag", "echo -n hi", {TYPE_COMMAND, TYPE_ARG, TYPE_ARG}, {"echo", "-n", "hi"}, 3, 0},
	//{"Basic double quotes", "echo \"hello\"", {TYPE_COMMAND, TYPE_ARG}, {"echo", "hello"}, 2, 0},
	//{"Basic single quotes", "echo 'hello'", {TYPE_COMMAND, TYPE_ARG}, {"echo", "hello"}, 2, 0},
	//{"Missing last single quote", "echo 'hello", {}, {}, 0, 1},
	//{"Missing first single quote", "echo hello'", {}, {}, 0, 1},
	//{"Missing last double quote", "echo \"hello", {}, {}, 0, 1},
	//{"Missing first double quote", "echo hello\"", {}, {}, 0, 1},
	//{"Complex quotes 1", "echo \"hello\"\'\'", {TYPE_COMMAND, TYPE_ARG, TYPE_ARG}, {"echo", "hello", ""}, 3, 0},
	//{"Complex quotes 2", "echo '\"hello\"'", {TYPE_COMMAND, TYPE_ARG, TYPE_ARG}, {"echo", "\"hello\""}, 2, 0},
	//{"Complex quotes 3", "echo \"'hello'\"", {TYPE_COMMAND, TYPE_ARG, TYPE_ARG}, {"echo", "'hello'"}, 2, 0},
	//{"Basic input redirection", "cat < input.txt", {TYPE_COMMAND, TYPE_REDIRIN, TYPE_ARG}, {"cat", "<", "input.txt"}, 3, 0},
	//{"Input redirection without spaces", "cat<input.txt", {TYPE_COMMAND, TYPE_REDIRIN, TYPE_ARG}, {"cat", "<", "input.txt"}, 3, 0},
	//{"Input redirection double quotes", "echo \"< input.txt\"", {TYPE_COMMAND, TYPE_ARG}, {"echo", "< input.txt"}, 2, 0},
	//{"Input redirection single quotes", "echo \'< input.txt\'", {TYPE_COMMAND, TYPE_ARG}, {"echo", "< input.txt"}, 2, 0},
	//{"Basic output redirection", "echo hello > output.txt", {TYPE_COMMAND, TYPE_ARG, TYPE_REDIROUT, TYPE_ARG}, {"echo", "hello", ">", "output.txt"}, 4, 0},
	//{"Output redirection without spaces", "echo hello>output.txt", {TYPE_COMMAND, TYPE_ARG, TYPE_REDIROUT, TYPE_ARG}, {"echo", "hello", ">", "output.txt"}, 4, 0},
	//{"Output redirection double quotes", "echo \"hello>output.txt\"", {TYPE_COMMAND, TYPE_ARG}, {"echo", "hello>output.txt"}, 2, 0},
	//{"Output redirection double quotes 2", "echo hello>\"output.txt\"", {TYPE_COMMAND, TYPE_ARG, TYPE_REDIROUT, TYPE_ARG}, {"echo", "hello", ">", "output.txt"}, 4, 0},
	//{"Output redirection single quotes", "echo \'hello>output.txt\'", {TYPE_COMMAND, TYPE_ARG}, {"echo", "hello>output.txt"}, 2, 0},
	//{"Output redirection single quotes 2", "echo hello>\'output.txt\'", {TYPE_COMMAND, TYPE_ARG, TYPE_REDIROUT, TYPE_ARG}, {"echo", "hello", ">", "output.txt"}, 4, 0},
	//{"Simple pipe", "ls | wc -l", {TYPE_COMMAND, TYPE_PIPE, TYPE_COMMAND, TYPE_ARG}, {"ls", "|", "wc", "-l"}, 4, 0},
	//{"Environment variable", "echo $HOME", {TYPE_COMMAND, TYPE_ARG}, {"echo", "var name"}, 2, 0},
	//{"Environment variable double quotes", "echo \"$HOME\"", {TYPE_COMMAND, TYPE_ARG}, {"echo", "var name"}, 2, 0},
	//{"Environment variable single quotes", "echo \'$HOME\'", {TYPE_COMMAND, TYPE_ARG}, {"echo", "$HOME"}, 2, 0},
	//{"Environment variable multiple quotes 1", "echo \"'$HOME'\"", {TYPE_COMMAND, TYPE_ARG}, {"echo", "var name"}, 2, 0},
	//{"Environment variable multiple quotes 2", "echo '\"$HOME\"'", {TYPE_COMMAND, TYPE_ARG}, {"echo", "\"$HOME\""}, 2, 0},
	//{"Complex command", "cat < input.txt | grep 'pattern' > output.txt", {TYPE_COMMAND, TYPE_REDIRIN, TYPE_ARG, TYPE_PIPE, TYPE_COMMAND, TYPE_ARG, TYPE_REDIROUT, TYPE_ARG}, {"cat", "<", "input.txt", "|", "grep", "pattern", ">", "output.txt"}, 8, 0},
	{"Basic heredoc", "cat << EOF", {TYPE_COMMAND, TYPE_HEREDOC_OP, TYPE_HEREDOC_DELIM}, {"cat", "<<", "EOF"}, 3, 0},
	//{"Complex heredoc 1", "cat<<1\nhi", {TYPE_COMMAND, TYPE_HEREDOC_OP, TYPE_HEREDOC_DELIM, TYPE_HEREDOC_CONTENT}, {"cat", "<<", "1", "\nhi"}, 4, 0},
	//{"Complex heredoc 2", "cat<<1\nhi\n", {TYPE_COMMAND, TYPE_HEREDOC_OP, TYPE_HEREDOC_DELIM, TYPE_HEREDOC_CONTENT}, {"cat", "<<", "1", "\nhi\n"}, 4, 0},
	//{"Complex heredoc 3", "cat<<1\nhi\n1", {TYPE_COMMAND, TYPE_HEREDOC_OP, TYPE_HEREDOC_DELIM, TYPE_HEREDOC_CONTENT, TYPE_HEREDOC_DELIM}, {"cat", "<<", "1", "\nhi\n", "1"}, 5, 0},
	//{"Complex heredoc 4", "cat<<1\necho hi\n1", {TYPE_COMMAND, TYPE_HEREDOC_OP, TYPE_HEREDOC_DELIM, TYPE_HEREDOC_CONTENT, TYPE_HEREDOC_DELIM}, {"cat", "<<", "1", "\necho hi\n", "1"}, 5, 0},
	//{"Complex heredoc 5", "cat << DELIM | grep 'pattern' > output.txt", {TYPE_COMMAND, TYPE_HEREDOC_OP, TYPE_HEREDOC_DELIM, TYPE_PIPE, TYPE_COMMAND, TYPE_ARG, TYPE_REDIROUT, TYPE_ARG}, {"cat", "<<", "DELIM", "|", "grep", "'pattern'", ">", "output.txt"}, 8, 0}
};

//function to verify if the token content and type match the expectations
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

//function to print out the expected vs current content in case of failure
void print_token_debug(t_token *token, char *expected_content, t_token_type expected_type)
{
	printf("  Expected: [%s] && type=%s\n  Got:      [%s] && type=%s\n",expected_content, g_token_type_str[expected_type], \
	token ? token->content : "NULL", token ?  g_token_type_str[token->type] : "UNDEFINED");
}

//run a test - split & expand input into token
//print test name and wether it fails or successes
//if it fails, print the expected vs current token content and type
int run_test(t_test test, t_env *env_list) {
	t_token *token_list = NULL;
	int success = 1;
	int result = input_to_token(&token_list, test.input);

	printf(BLUE "Running: %s" RESET "\n", test.name);
	printf("Input: '%s'\n", test.input);

	if (test.expect_fail)
		 success = (result != SUCCESS);
	else
	{
		 if (result != SUCCESS)
		{
			success = 0;
			printf(RED "  Failed: Token creation failed\n" RESET);
		}
	}
	//(void)env_list;
	if (success && expand_tokens(&token_list, env_list) != SUCCESS)
	{
		success = 0;
		printf(RED "  Failed: Token expansion failed\n" RESET);
	}

	t_token *current = token_list;
	for (int i = 0; i < test.token_count && success && current; i++)
	{
		if (!verify_token(current, test.expected_contents[i], test.expected_types[i]))
		{
			success = 0;
			printf(RED "  Failed at token %d:\n" RESET, i + 1);
			print_token_debug(current, test.expected_contents[i], test.expected_types[i]);
		}
			current = current->next;
	}
	print_token(token_list);

	if (success && current)
	{
		printf(RED "  Failed: Extra unexpected tokens found\n" RESET);
		success = 0;
	}

	printf(success ? GREEN "  ✓ Passed\n" RESET : RED "  ✗ Failed\n" RESET);
	printf("\n");
	print_token(token_list);
	free_token_list(&token_list);
	return success;
}

//Go through each test from the test array, run them and print the summary
int main(int argc, char **argv, char **env)
{
	(void)argv;
	if (argc < 0)
		return 0;
	t_env *env_list;
	int count = sizeof(tests_tokenizer) / sizeof(tests_tokenizer[0]);
	int success = 0;

	env_list = NULL;
	init_env(&env_list, env);
	printf(YELLOW "\n=== Tokenizer Tests (%d total) ===\n\n" RESET, count);
	for (int i = 0; i < count; i++) {
		success += run_test(tests_tokenizer[i], env_list);
	}
	printf(YELLOW "\n=== Summary ===\n" RESET);
	printf("Passed: %d/%d (%d%%)\n", success, count, (success * 100) / count);
	printf(success == count ? GREEN "All tests passed!\n" : RED "Some tests failed!\n" RESET);
}
