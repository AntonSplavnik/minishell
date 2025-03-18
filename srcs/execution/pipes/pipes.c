/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:11 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/18 11:19:26 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_pipe(t_shell *s)
{
	return (count_pipes(s->token_list) > 0);
}

t_error	handle_pipe_operations(t_shell *s)
{
	int		pipe_count;
	int		cmd_count;
	t_error	result;

	pipe_count = count_pipes(s->token_list);
	if (pipe_count == 0)
		return (ERR_PARSING);
	cmd_count = pipe_count + 1;
	result = execute_pipeline(s, cmd_count);
	return (result);
}

t_error	execute_pipeline(t_shell *s, int cmd_count)
{
	int		prev_pipe;
	int		status;
	t_token	*current;
	t_error	result;
	pid_t	pid;
	int		is_sigint;

	is_sigint = 0;
	prev_pipe = -1;
	current = s->token_list;
	result = SUCCESS;
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, signal_handler_parent);
	while (cmd_count > 0 && result == SUCCESS)
	{
		cmd_count = cmd_count - 1;
		// printf("in execute pipeline before process pipe stage token list is\n");
		// print_token(s->token_list);
		// printf("\n");
		result = process_pipe_stage(s, &current, &prev_pipe, cmd_count);
		// printf("in execute pipeline after process pipe stage, token list is\n");
		// print_token(s->token_list);
		// printf("\n");
	}
	if (prev_pipe != -1)
		close(prev_pipe);
	pid = wait(&status);
	while (pid > 0)
    {
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
            is_sigint = 1;

        if (pid == s->last_pid)
        {
            if (WIFEXITED(status))
                s->exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                s->exit_status = 128 + WTERMSIG(status);
        }
        pid = wait(&status);
    }

     // Always print a newline to the terminal for SIGINT in pipeline
	 if (is_sigint)
	 {
		 // Save stdout and restore it after writing the newline
		 int saved_stdout = dup(STDOUT_FILENO);
		 if (saved_stdout != -1)
		 {
			 // Force output to the terminal
			 int tty = open("/dev/tty", O_WRONLY);
			 if (tty != -1)
			 {
				 dup2(tty, STDOUT_FILENO);
				 write(STDOUT_FILENO, "\n\n", 1);
				 close(tty);
			 }
			 // Restore original stdout
			 dup2(saved_stdout, STDOUT_FILENO);
			 close(saved_stdout);
		 }
	 }

    // Restore interactive signal handling
    set_signals_interactive();
	return (result);
}
