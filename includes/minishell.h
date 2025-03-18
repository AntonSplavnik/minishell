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
# include <types.h>

/* Readline Libraries */
# include <readline/readline.h>
# include <readline/history.h>

/*Include libft*/
# include "../libft/libft.h"

/*Include other headers*/
# include "parsing.h"
# include "types.h"
# include "execution.h"
# include "redirections_heredoc.h"
# include "pipes.h"

# define PROMPT_COLOR "\001\033[1;36m\002"	// Cyan
# define RESET_COLOR "\001\033[0m\002"		// Reset to default

//global variable for signals
extern int			g_sig;

//to delete
static int token_alloc_count = 0;
static int token_free_count = 0;

//utils
char				*ft_strndup(const char *s, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strcpy(char *dest, const char *src);
char				*ft_strstr(const char *big, const char *little);
char				*ft_strncpy(char *dest, const char *src, size_t n);

//main_utils.c
int					only_whitespace(char *s);
int					set_exit_status(t_error error, t_shell *s, char *args);
int					cleanup_signals(t_shell *s, char *args);

//free.c
void				free_token_list(t_token **token_list);
void				free_env_list(t_env **env_list);
void				free_array(char **array);
void				free_command_path(char *cmd_path, char **args);
void				free_all(t_shell *s, char **array);

//error.c
void				print_error(t_error error_code);
t_error				handle_error_free_tokens(t_error error_code, \
					t_token **token_list, char *args);
t_error				handle_error(t_error error_code, void *ptr_to_free);
t_error				handle_error_free_env(t_error error_code, \
								t_env **env_list);
void				print_error_builtin(char *function, \
						char *arg, char *message);

//INIT
//init_data.c
t_error				initialize_shell(t_shell **s, char **env);

//ENV
//init_env.c
t_error				init_env(t_shell *s, t_env **env_list, char **env);
t_error				add_envvar(t_env **env_list, char *env);
t_env				*create_envvar(char *env);

//manipulate_env.c
t_env				*find_env_var(t_env *env_list, char *key);
void				free_env_var(t_env *envvar);
t_error				set_env_key_value(t_env *new_envvar, char *env);
t_error				set_env_content(t_env *new_envvar, char *env);

//SIGNALS

//signals
int					set_signals_interactive(void);
int					set_signals_child(void);
int					set_signals_parent(void);
int					set_signals_heredoc(void);
t_error				check_heredoc_signal(void);
t_error				handle_heredoc_interruption(char **content_ptr, \
						int stdin_copy);
void				signal_handler_parent(int signum);

//TO DELETE BEFORE SUB
//utils_to_print
void				print_token(t_token *token_list);
extern const char	*g_token_type_str[];
void				print_env(t_env *env_list);

#endif
