/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:08:54 by abillote          #+#    #+#             */
/*   Updated: 2024/12/06 16:11:43 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

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
	TYPE_HEREDOC_OP, // << (here document)
	TYPE_HEREDOC_DELIM, // heredoc delimiter
	TYPE_HEREDOC_CONTENT, // heredoc content in between delimiters
	TOKEN_EMPTY,
}			t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*content;
	struct s_token	*next;
}			t_token;

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

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}			t_env;

//Main Shell structure, to enhanced and figure out how to use it
typedef struct s_shell
{
	t_env	*env_list;
	t_token	*token_list;
	int		exit_status;
}	t_shell;

#endif
