/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:03:20 by abillote          #+#    #+#             */
/*   Updated: 2024/11/28 18:27:58 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Required Standard Libraries */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h>
# include <dirent.h>
# include <termios.h>
# include <errno.h>
# include <limits.h>

/* Readline Libraries */
# include <readline/readline.h>
# include <readline/history.h>

/*Include libft*/
# include "../libft/libft.h"

typedef enum e_error
{
	SUCCESS = 0,
	ERR_LAUNCH = 1,
	ERR_MALLOC = 2,
	ERR_PARCING = 3,
	ERR_ENV = 4,
	//..add any other relevant error
	//do not forget to add them in print_error function in errors.c
}	t_error;

//When you add token type, add it in the utils to print file as well
typedef enum e_token_type
{
	TYPE_ARG, //all regular words or arguments
	TYPE_COMMAND, //built-in commands
	TYPE_DQUOTE, // double quote
	TYPE_SQUOTE, //single quote
	TYPE_PIPE, // pipe
	TYPE_REDIRIN, //< (input redirection)
	TYPE_REDIROUT, // > (output redirection)
	TYPE_REDIRAPPEND, // >> (append output redirection)
	TYPE_HEREDOC, // << (here document)
	TOKEN_EMPTY,
}			t_token_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}			t_env;

typedef struct s_token
{
	t_token_type	type;
	char			*content;
	struct s_token	*next;
}			t_token;

//Used during token extraction to store quote information and pass it to different functions
typedef struct s_quote_info
{
	int		in_inner_quote;
	char	inner_quote;
	char	outer_quote;
}			t_quote_info;

//Main Shell structure, to enhanced and figure out how to use it
typedef struct s_shell
{
	t_env	*env_list;
	int		exit_status;
} t_shell;

//PARCING
//token_creation.c
t_error				input_to_token(t_token **token_list, char *args);
t_token				*create_token(char *input, t_token_type type);
t_error				add_token(t_token **token_list, \
					char *input, t_token_type type);
t_token_type		get_token_type(char *input, t_token **token_list);
int					is_command(t_token **token_list);

//token_extraction.c
t_error				ft_split_token(t_token **token_list, \
						char *args, size_t *i, char **token);

//token_expansion.c
t_error 			expand_tokens(t_token **token_list, t_env *env_list);

//token_utils.c
int					is_space(char c);
int					is_delimiter(char c);
int					get_delimiter_len(const char *str);

//ENV
//init_env.c
t_error				init_env(t_env **env_list, char **env);
t_error				add_envvar(t_env **env_list, char *env);
t_env				*create_envvar(char *env);

//utils
char				*ft_strndup(const char *s, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strcpy(char *dest, const char *src);

//free.c
void				free_token_list(t_token **token_list);
void				free_env_list(t_env **env_list);

//error.c
void				print_error(t_error error_code);
t_error				handle_error_free_tokens(t_error error_code, \
					t_token **token_list, char *args);
t_error				handle_error(t_error error_code, void *ptr_to_free);
t_error				handle_error_free_env(t_error error_code, \
								t_env **env_list);

//TO DELETE BEFORE SUB
//utils_to_print
void				print_token(t_token *token_list);
extern const char	*g_token_type_str[];
void				print_env(t_env *env_list);

#endif
