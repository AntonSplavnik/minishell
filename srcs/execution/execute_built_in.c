/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_in.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:59:38 by abillote          #+#    #+#             */
/*   Updated: 2025/01/24 17:32:24 by abillote         ###   ########.fr       */
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
		return (execute_echo(args));
	if (ft_strcmp(cmd_token->content, "pwd") == 0)
		return (execute_pwd(args));
	printf("The command '%s' is a built-in \
		and still needs to be implemented\n", cmd_token->content);
	(void)args;
	(void)s;
	return (SUCCESS);
}
