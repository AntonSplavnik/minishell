/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:59:38 by abillote          #+#    #+#             */
/*   Updated: 2025/03/13 13:17:05 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

t_error	execute_builtin(t_token *cmd_token, char **args, t_shell *s)
{
	if (ft_strcmp(cmd_token->content, "echo") == 0)
		return (execute_echo(args, s));
	if (ft_strcmp(cmd_token->content, "pwd") == 0)
		return (execute_pwd(s));
	if (ft_strcmp(cmd_token->content, "cd") == 0)
		return (execute_cd(args, s));
	if (ft_strcmp(cmd_token->content, "env") == 0)
		return (execute_env(args, s->env_list, s));
	if (ft_strcmp(cmd_token->content, "exit") == 0)
		return (execute_exit(args, s));
	if (ft_strcmp(cmd_token->content, "unset") == 0)
		return (execute_unset(args, s));
	if (ft_strcmp(cmd_token->content, "export") == 0)
		return (execute_export(args, s));
	(void)args;
	(void)s;
	return (SUCCESS);
}
