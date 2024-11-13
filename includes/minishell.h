/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:03:20 by abillote          #+#    #+#             */
/*   Updated: 2024/11/13 15:17:06 by abillote         ###   ########.fr       */
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

typedef struct s_token
{
	t_token_type	type;
	char			*content;
	struct s_token	*next;
}			t_token;

typedef struct s_token_map
{
	char			*content;
	t_token_type	type;
}			t_token_map;

//static const t_token_map	g_token_types[] = {
//{"echo", TYPE_COMMAND},
//{"pwd", TYPE_COMMAND},
//{"\"", TYPE_QUOTES},
//{NULL, TOKEN_ERROR}
//};

// typedef struct s_command
// {
// 	char	**args;
// 	char	*function;
// 	t_token	*tokens;
// }			t_command;

//PARCING
//tokenisation.c
void				input_to_token(t_token **token_list, char *args);
t_token				*create_token(char *input, t_token_type type);
void				add_token(t_token **token_list, char *input,\
						 t_token_type type);
t_token_type		get_token_type(char *input);

//parcing_utils
int					is_command(char *input);

//free.c
void				free_token_list(t_token **token_list);

//TO DELETE BEFORE SUB
//utils_to_print
void				print_token(t_token *token_list);
extern const char	*g_token_type_str[];

#endif
