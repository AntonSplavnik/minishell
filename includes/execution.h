/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:46:41 by abillote          #+#    #+#             */
/*   Updated: 2025/01/17 17:58:54 by abillote         ###   ########.fr       */
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

#endif
