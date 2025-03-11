/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:54:47 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/11 18:36:49 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	handle_child_redirections
	execute_child_process
	handle_child_process
	handle_parent_process
*/

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
		handle_child_signal(status, s);
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

/*
	if (something_is_broken)
		uncomment_comented_functions
	else
		remove_comented_functions

void	handle_child_process(char *cmd_path, char **args, t_shell *s)
{
	if (set_signals_child() != 0)
	{
		free(cmd_path);
		free_array(args);
		exit(1);
	}
	execve(cmd_path, args, s->envp);
	free(cmd_path);
	free_array(args);
	s->exit_status = 127;
	exit(127);
} */

/*
Purpose:
Manages the parent process's behavior after a command is executed in a
child process.

Functionality:
Waits for the child process to finish using waitpid(pid, &status, 0).
Frees the allocated cmd_path.
Updates the shell's exit status using handle_exit_status(status, s).
Resets signal handling for interactive mode with set_signals_interactive().
t_error	handle_parent_process(pid_t pid, char *cmd_path, t_shell *s)
{
	int	status;

	waitpid(pid, &status, 0);
	free(cmd_path);
	handle_exit_status(status, s);
	if (set_signals_interactive())
	return (ERR_SIGNAL);
	return (SUCCESS);
}
*/