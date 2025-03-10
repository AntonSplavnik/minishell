/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:46:41 by abillote          #+#    #+#             */
/*   Updated: 2025/03/10 18:18:08 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "types.h"

//execute_child.c
void	handle_child_process(char *cmd_path, char **args, t_shell *s);
t_error	handle_parent_process(pid_t pid, char *cmd_path, t_shell *s);
t_error	execute_child_process(char *cmd_path, char **args, t_shell *s);
void	handle_child_redirections(t_token *cmd, t_shell *s);

//execute_command.c
t_error	execute_command(t_shell *s);

//execute_prepare_cmd_args.c
char	**prepare_command_args(t_token *cmd_token);
int		count_command_args(t_token *cmd_token);


//execute_single.c
t_error	execute_single_command(t_token *cmd, t_shell *s);
t_error	execute_external_command(t_token *cmd, char **args, t_shell *s);
t_error handle_builtin_redirections(t_token *cmd, t_shell *s, int std_fds[2]);
int has_input_redirection(t_token *cmd);

//execute_utils.c
t_error	free_resources(char *cmd_path, t_shell *s, t_error result);
t_error	handle_malloc_error(t_shell *s);
int		is_builtin(char *cmd);
void	handle_exit_status(int status, t_shell *s);

//pipes_exec.c
t_error	create_pipe_and_fork(int cmd_count, int pipe_fd[2], pid_t *pid);
t_error	process_child(t_token *cmd, int prev_pipe, int cmd_count,
										int pipe_fd[2], t_shell *s);
void	set_output_fd(int cmd_count, int pipe_fd[2], int *out_fd);

//pipes_utils.c
t_error	process_parent(int *prev_pipe, int pipe_fd[2], int cmd_count);
t_error	process_pipe_stage(t_shell *s, t_token **current,
									int *prev_pipe, int cmd_count);
int		count_pipes(t_token *tokens);
t_token	*get_next_cmd(t_token **tokens);


//pipes.c
int		has_pipe(t_shell *s);
t_error	handle_pipe_operations(t_shell *s);
t_error	execute_pipeline(t_shell *s, int cmd_count);


//redirections.c
t_error	handle_redirections(t_shell *s);


//signal_utils.c
void	handle_child_process_io(int in_fd, int out_fd);
void	handle_child_signal(int status, t_shell *s);


//token_utils.c
t_token	*copy_tokens(t_token *src);
void	token_clear(t_token **tokens);
t_token	*token_new(char *content, int type);
void	token_add_back(t_token **lst, t_token *new);


//command_path.c
char	*try_path_dir(char *dir, char *cmd, t_shell *s);
char	*find_command_path(char *cmd, t_shell *s);

//execute_built_in.c
t_error	execute_built_in(t_token *cmd_token, char **args, t_shell *s);
int		is_built_in(char *cmd);
// t_error	execute_builtin_with_redirs(t_token *cmd, char **args, t_shell *s);



//echo.c
t_error	execute_echo(char **args, t_shell *s);

//pwd.c
t_error	execute_pwd(t_shell *s);

//cd.c
t_error	execute_cd(char **args, t_shell *s);

//env.c
t_error	execute_env(char **args, t_env *env_list, t_shell *s);

//exit
t_error	execute_exit(char **args, t_shell *s);

//unset
t_error	execute_unset(char **args, t_shell *s);

//export
t_error	execute_export(char **args, t_shell *s);
t_error	print_sorted_env(t_env	*env_list);
int		is_valid_var_name(char *arg);

//export_handlers
t_error	handle_equal_case(char *arg, char *equal_sign, t_shell *s);
t_error	handle_append_case(char *arg, char *append_sign, t_shell *s);
t_error	handle_append_operation(char *append_sign, t_env *existing);
t_error	update_existing_var(t_env *existing, char *new_value);
t_error	handle_no_equal_sign(char *arg, t_shell *s);

//export_array
char	**create_env_array(t_env *env_list, int count_var);
int		store_env_var_in_array(t_env *current, char **env_array, int i);
void	sort_env_array(char **env_array);
size_t	count_env_var(t_env *env_list);

//export_format
char	*format_env_with_equals(t_env *current, char **env_array);
char	*format_env_prefix(t_env *current, char **env_array);

#endif
