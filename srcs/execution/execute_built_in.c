/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_in.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:59:38 by abillote          #+#    #+#             */
/*   Updated: 2025/02/05 16:28:14 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Checks if a command is one of the shell's built-in commands:
- Compares command name against list of built-ins
- Built-ins are: echo, cd, pwd, export, unset, env, exit
Returns: 1 if command is built-in, 0 otherwise
*/
int	is_built_in(char *cmd)
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

/*Executes built-in shell commands:
- Currently placeholder implementation
- Will handle: echo, cd, pwd, export, unset, env, exit
Returns: SUCCESS or appropriate error code
*/
t_error	execute_built_in(t_token *cmd_token, char **args, t_shell *s)
{
	if (ft_strcmp(cmd_token->content, "echo") == 0)
		return (execute_echo(args, s));
	if (ft_strcmp(cmd_token->content, "pwd") == 0)
		return (execute_pwd(s));
	if (ft_strcmp(cmd_token->content, "cd") == 0)
		return (execute_cd(cmd_token, s));
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
