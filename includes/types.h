/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:08:54 by abillote          #+#    #+#             */
/*   Updated: 2025/03/17 17:15:51 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

#include "minishell.h"

//When you add token type, add it in the utils to print file as well
typedef enum e_token_type
{
	TYPE_ARG,
	TYPE_COMMAND,
	TYPE_DQUOTE,
	TYPE_SQUOTE,
	TYPE_PIPE,
	TYPE_REDIRIN,
	TYPE_REDIROUT,
	TYPE_REDIRAPPEND,
	TYPE_HEREDOC_OP,
	TYPE_HEREDOC_DELIM,
	TYPE_HEREDOC_CONTENT,
	TYPE_HEREDOC_CONTENT_QUOTED,
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
	ERR_PARSING = 3,
	ERR_ENV = 4,
	ERR_CMD_NOT_FOUND = 5,
	ERR_EXEC = 6,
	ERR_ENV_EXEC = 7,
	ERR_CD = 8,
	ERR_EXIT_ARGS = 9,
	ERR_EXPORT = 10,
	ERR_SIGNAL = 11,
	ERR_PARSING_PIPE = 12,
	ERR_PARSING_REDIR = 13,
	ERR_REDIR = 14,
	ERR_SYNTAX = 15,
}	t_error;

typedef struct s_env
{
	char			*content;
	char			*key;
	char			*value;
	struct s_env	*next;
}			t_env;

//Main Shell structure, to enhanced and figure out how to use it
typedef struct s_shell
{
	char	**envp;
	t_env	*env_list;
	t_token	*token_list;
	int		exit_status;
	int		stdin_copy;
	pid_t	last_pid;
	int		std_fds[2];
	int		printed_newline;
}	t_shell;

#endif
