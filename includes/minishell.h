/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:03:20 by abillote          #+#    #+#             */
/*   Updated: 2024/12/06 14:51:12 by abillote         ###   ########.fr       */
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

/*Include other headers*/
# include "parcing.h"
# include "types.h"

//Store last exit status
extern int			g_last_exit_status;

//ENV
//init_env.c
t_error				init_env(t_env **env_list, char **env);
t_error				add_envvar(t_env **env_list, char *env);
t_env				*create_envvar(char *env);

//utils
char				*ft_strndup(const char *s, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strcpy(char *dest, const char *src);
int					ft_strstr(const char *big, const char *little);

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
