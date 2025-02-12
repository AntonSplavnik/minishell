/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:46:41 by abillote          #+#    #+#             */
/*   Updated: 2025/02/12 15:13:39 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "types.h"

//execute_command.c
t_error	execute_command(t_token *cmd_token, t_shell *shell);

//command_path.c
char	*try_path_dir(char *dir, char *cmd, t_shell *s);
char	*find_command_path(char *cmd, t_shell *s);

//execute_built_in.c
t_error	execute_built_in(t_token *cmd_token, char **args, t_shell *s);
int		is_built_in(char *cmd);

//echo.c
t_error	execute_echo(char **args, t_shell *s);

//pwd.c
t_error	execute_pwd(t_shell *s);

//cd.c
t_error	execute_cd(t_token *cmd_token, t_shell *s);

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
