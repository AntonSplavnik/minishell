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

//test structure to easily add new test
typedef struct s_test {
	char *name;
	char *input;
	t_token_type expected_types[10];	// Array of expected token types
	char *expected_contents[10];	// Array of expected token contents
	int token_count;				// Number of tokens to verify
	int expect_fail; //for test that we need to fail (syntax error)
} t_test;

//tests array to store all the tests - you can add a new test at the bottom
t_test tests_tokenizer[] = {
	{"Basic double quotes", "echo \"hello\"", {TYPE_COMMAND, TYPE_ARG}, {"echo", "\"hello\""}, 2, 0},
	{"Basic single quotes", "echo 'hello'", {TYPE_COMMAND, TYPE_ARG}, {"echo", "'hello'"}, 2, 0},
	{"Missing last single quote", "echo 'hello", {}, {}, 0, 1},
	{"Missing first single quote", "echo hello'", {}, {}, 0, 1},
	{"Missing last double quote", "echo \"hello", {}, {}, 0, 1},
	{"Missing first double quote", "echo hello\"", {}, {}, 0, 1},
	{"Complex quotes", "echo \"hello\"\'\'", {TYPE_COMMAND, TYPE_ARG, TYPE_ARG}, {"echo", "hello", ""}, 3, 0},
	{"Basic input redirection", "cat < input.txt", {TYPE_COMMAND, TYPE_REDIRIN, TYPE_ARG}, {"cat", "<", "input.txt"}, 3, 0},
	{"Input redirection without spaces", "cat<input.txt", {TYPE_COMMAND, TYPE_REDIRIN, TYPE_ARG}, {"cat", "<", "input.txt"}, 3, 0},
	{"Input redirection double quotes", "echo \"< input.txt\"", {TYPE_COMMAND, TYPE_ARG}, {"echo", "\"< input.txt\""}, 2, 0},
	{"Input redirection single quotes", "echo \'< input.txt\'", {TYPE_COMMAND, TYPE_ARG}, {"echo", "\'< input.txt\'"}, 2, 0},
	{"Basic output redirection", "echo hello > output.txt", {TYPE_COMMAND, TYPE_ARG, TYPE_REDIROUT, TYPE_ARG}, {"echo", "hello", ">", "output.txt"}, 4, 0},
	{"Output redirection without spaces", "echo hello>output.txt", {TYPE_COMMAND, TYPE_ARG, TYPE_REDIROUT, TYPE_ARG}, {"echo", "hello", ">", "output.txt"}, 4, 0},
	{"Output redirection double quotes", "echo \"hello>output.txt\"", {TYPE_COMMAND, TYPE_ARG}, {"echo", "\"hello>output.txt\""}, 2, 0},
	{"Output redirection double quotes 2", "echo hello>\"output.txt\"", {TYPE_COMMAND, TYPE_ARG, TYPE_REDIROUT, TYPE_ARG}, {"echo", "hello", ">", "output.txt"}, 4, 0},
	{"Output redirection single quotes", "echo \'hello>output.txt\'", {TYPE_COMMAND, TYPE_ARG}, {"echo", "hello>output.txt"}, 2, 0},
	{"Output redirection single quotes 2", "echo hello>\'output.txt\'", {TYPE_COMMAND, TYPE_ARG, TYPE_REDIROUT, TYPE_ARG}, {"echo", "hello", ">", "\'output.txt\'"}, 4, 0},
	{"Simple pipe", "ls | wc -l", {TYPE_COMMAND, TYPE_PIPE, TYPE_COMMAND, TYPE_ARG}, {"ls", "|", "wc", "-l"}, 4, 0},
	{"Environment variable", "echo $HOME", {TYPE_COMMAND, TYPE_ARG}, {"echo", "$HOME"}, 2, 0},
	{"Environment variable double quotes", "echo \"$HOME\"", {TYPE_COMMAND, TYPE_ARG}, {"echo", "\"$HOME\""}, 2, 0},
	{"Environment variable single quotes", "echo \'$HOME\'", {TYPE_COMMAND, TYPE_ARG}, {"echo", "\'$HOME\'"}, 2, 0},
	{"Complex command", "cat < input.txt | grep 'pattern' > output.txt", {TYPE_COMMAND, TYPE_REDIRIN, TYPE_ARG, TYPE_PIPE, TYPE_COMMAND, TYPE_ARG, TYPE_REDIROUT, TYPE_ARG}, {"cat", "<", "input.txt", "|", "grep", "'pattern'", ">", "output.txt"}, 8, 0},
	{"Basic heredoc", "cat << EOF", {TYPE_COMMAND, TYPE_HEREDOC, TYPE_ARG}, {"cat", "<<", "EOF"}, 3, 0},
	{"Complex heredoc", "cat << DELIM | grep 'pattern' > output.txt", {TYPE_COMMAND, TYPE_HEREDOC, TYPE_ARG, TYPE_PIPE, TYPE_COMMAND, TYPE_ARG, TYPE_REDIROUT, TYPE_ARG}, {"cat", "<<", "DELIM", "|", "grep", "'pattern'", ">", "output.txt"}, 8, 0}
};

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

int run_test(t_test test, t_env *env_list) {
	t_token *token_list = NULL;
	int success = 1;
	int result = input_to_token(&token_list, test.input);

	if (test.expect_fail)
		 success = (result != SUCCESS);
	else
	{
		 if (result != SUCCESS)
			success = 0;
		 if (success && expand_tokens(&token_list, env_list) != SUCCESS)
			success = 0;
		 t_token *current = token_list;
		 for (int i = 0; i < test.token_count && success && current; i++) {
			if (!verify_token(current, test.expected_contents[i], test.expected_types[i]))
					success = 0;
			  current = current->next;
		 }
	}

	print_test_result(test.name, success);
	free_token_list(&token_list);
	return success;
}

//Running a test - if you want to check what is inside the tokens:
//1. In the t_test test_tokenizer, comment out all the tests you are not interested in
//2. In run_test function, uncomment the print_token(&token_list) line
int main(int argc, char **env)
{
	if (argc < 0)
		return 0;
	t_env *env_list;
	int count = sizeof(tests_tokenizer) / sizeof(tests_tokenizer[0]);
	int success = 0;

	env_list = NULL;
	init_env(&env_list, env);
	printf("-----Tokenizer Tests-----\n\n");
	for (int i = 0; i < count; i++) {
		success += run_test(tests_tokenizer[i], env_list);
	}
	printf("SUCCESS : %d out of %d\n", success, count);
}
