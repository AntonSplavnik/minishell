/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_in.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:59:38 by abillote          #+#    #+#             */
/*   Updated: 2025/03/10 15:08:16 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
	is_built_in
	execute_built_in
*/

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
/*
t_error	execute_builtin_with_redirs(t_token *cmd, char **args, t_shell *s)
{
	t_error	res;
	int		original_stdout;
	int		original_stdin;

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	res = execute_built_in(cmd, args, s);
	dup2(original_stdout, STDOUT_FILENO);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdout);
	close(original_stdin);
	return (res);
}
 */