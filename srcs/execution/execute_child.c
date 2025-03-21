/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:54:47 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/18 11:51:15 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_exit_status(int status, t_shell *s)
{
	if (WIFEXITED(status))
	{
		s->exit_status = WEXITSTATUS(status);
		return ;
	}
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			s->exit_status = 130;
			if (isatty(STDOUT_FILENO))
				write(1, "\n", 1);
			return ;
		}
		if (WTERMSIG(status) == SIGQUIT)
		{
			s->exit_status = 131;
			return ;
		}
		s->exit_status = 128 + WTERMSIG(status);
	}
}

void	execute_child_process_helper(char *cmd_path, int a)
{
	if (a == 1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd_path, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		exit(126);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd_path, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		exit(126);
	}
}

t_error	execute_child_process(char *cmd_path, char **args, t_shell *s)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve(cmd_path, args, s->envp);
		if (errno == EISDIR)
			execute_child_process_helper(cmd_path, 1);
		if (errno == EACCES)
			execute_child_process_helper(cmd_path, 0);
		exit(127);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		update_exit_status(status, s);
	}
	else
		return (ERR_EXEC);
	return (SUCCESS);
}

void	handle_child_redirections(t_token *cmd, t_shell *s)
{
	t_token	*current;

	current = cmd;
	while (current)
	{
		if (is_redirection(current->type))
		{
			if (get_redir_type(current->type) == REDIR_HEREDOC)
				handle_heredoc_execution(current, s);
			else if (get_redir_type(current->type) == REDIR_IN)
				handle_input(current, s);
			else
				handle_output(current, s);
			current = current->next->next;
		}
		else
			current = current->next;
	}
}
