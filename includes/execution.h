/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:46:41 by abillote          #+#    #+#             */
/*   Updated: 2025/01/25 21:19:57 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "types.h"

//execute_command.c
t_error	execute_command(t_token *cmd_token, t_shell *shell);

//command_path.c
char	*try_path_dir(char *dir, char *cmd);
char	*find_command_path(char *cmd, t_shell *s);

//execute_built_in.c
t_error	execute_built_in(t_token *cmd_token, char **args, t_shell *s);
int		is_built_in(char *cmd);

//echo.c
t_error	execute_echo(char **args);

//pwd.c
t_error	execute_pwd(char **args);

//cd.c
t_error	execute_cd(t_token *cmd_token, t_shell *s);

//env.c
t_error	execute_env(char **args, t_env *env_list, t_shell *s);

#endif
