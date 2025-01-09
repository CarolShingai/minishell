/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:49:25 by cshingai          #+#    #+#             */
/*   Updated: 2025/01/08 20:52:42 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile int	g_signal;

int	main(int argc __attribute__((unused)), \
		char **argv __attribute__((unused)), char **envp)
{
	t_minishell	shell;

	//print_menu();
	init_shell(&shell);
	shell.envp_list = create_env_list(envp);
	read_prompt(&shell);
	ft_printf("\nexit\n");
	rl_clear_history();
	free_minishell(&shell);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	return (shell.status);
}

void	read_prompt(t_minishell *shell)
{
	int	prompt_has_only_spaces;

	while (1)
	{
		set_prompt(shell);
		shell->prompt = readline("minihell: ");
		if (g_signal)
		{
			shell->status = g_signal;
			g_signal = 0;
		}
		if (shell->prompt == NULL)
			break ;
		shell->token_list = tokenizer(shell->prompt);
		prompt_has_only_spaces = (shell->prompt[0] != '\0'
				&& ft_is_space_str(shell->prompt) == 0);
		add_history(shell->prompt);
		free(shell->prompt);
		shell->prompt = NULL;
		if (shell->token_list)
			handle_shell_execution(shell);
		else if (prompt_has_only_spaces)
			ft_printf_fd(STDERR_FILENO, "Syntax error\n");
		free_prompt(shell);
	}
}

void	set_prompt(t_minishell *shell)
{
	shell->token_list = NULL;
	shell->tree = NULL;
	shell->envp = list_to_str(shell->envp_list);
	init_signals();
	dup2(shell->fd_stdin, STDIN_FILENO);
	dup2(shell->fd_stdout, STDOUT_FILENO);
}

void	handle_shell_execution(t_minishell *shell)
{
	if (valid_list(shell->token_list, shell))
	{
		hunt_heredoc(shell->token_list, shell);
		if (shell->token_list && g_signal == 0)
			shell->tree = build_root(shell->token_list);
		else if (g_signal)
			free_list(&shell->token_list);
		if (shell->tree)
			executor(shell->tree, shell);
		wait_pid(shell);
		close_fd(shell);
		free_pid_list(&shell->pid);
		shell->token_list = NULL;
	}
	else
		shell->status = 2;
}

void	free_prompt(t_minishell *shell)
{
	token_clear_list(&shell->token_list);
	free_envp_str(shell->envp);
	free_tree(&shell->tree);
}
