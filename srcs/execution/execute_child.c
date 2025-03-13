/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:54:47 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/13 18:36:26 by asplavni         ###   ########.fr       */
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
			write(1, "\n", 1);
			return ;
		}
		if (WTERMSIG(status) == SIGQUIT)
		{
			s->exit_status = 131;
			write(2, "Quit: 3\n", 9);
			return ;
		}
		s->exit_status = 128 + WTERMSIG(status);
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
		execve(cmd_path, args, s->envp);
		exit(127);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		update_exit_status(status, s);
	}
	else
	{
		return (ERR_EXEC);
	}
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
				handle_heredoc_(current, s);
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
