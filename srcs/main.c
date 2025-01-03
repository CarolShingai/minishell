/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsouza-r <lsouza-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:49:25 by cshingai          #+#    #+#             */
/*   Updated: 2024/12/29 15:32:20 by lsouza-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile int	g_signal;

/*
	In this first part of the main I have created the prompt. The prompt is
	created by the function readline. There are many functions, that still need
	to explore and probably will need to use, from the library of readline. But
	so far addhistory is the one who will bring history when press arrow up.
*/


// static void	print_list(t_list *list)
// {
// 	t_list *aux;

// 	aux = list;
// 	while (aux)
// 	{
// 		printf("lexema: %s type: %d\n", aux->token.lexeme, aux->token.type);
// 		aux = aux->next;
// 	}
// }

// static void	print_tree(t_tree *tree)
// {
// 	if (tree)
// 	{
// 		printf("arvore: %s type: %d\n", tree->sub_list->token.lexeme, tree->sub_list->token.type);
// 		print_list(tree->sub_list);
// 		print_tree(tree->right);
// 		print_tree(tree->left);
// 	}
// }




int	main(int argc __attribute__((unused)), \
		char **argv __attribute__((unused)), char **envp)
{
	t_minishell	shell;

	init_shell(&shell);
	shell.envp_list = create_env_list(envp);
	while (1)
	{
		shell.envp = list_to_str(shell.envp_list);
		init_signals();
		dup2(shell.fd_stdin, STDIN_FILENO); // lembrar de colocar se der erro dup2 < 0
		dup2(shell.fd_stdout, STDOUT_FILENO); // lembrar de colocar se der erro dup2 < 0
		shell.prompt = readline("minihell: ");
		if (g_signal)
		{
			shell.status = g_signal;
			g_signal = 0;
		}
		if (shell.prompt == NULL)
			break ;
		shell.token_list = NULL;
		shell.tree = NULL;
		shell.token_list = tokenizer(shell.prompt);
		if (shell.token_list)
		{
			if (valid_list(shell.token_list, &shell))
			{
				hunt_heredoc(shell.token_list, &shell);
				if (shell.token_list && g_signal == 0)
					shell.tree = build_root(shell.token_list);
				if (shell.tree)
					executor(shell.tree, &shell);
				wait_pid(&shell);
				close_fd(&shell);

				free_tree(&shell.tree);
				free_pid_list(&shell.pid);
				shell.token_list = NULL;
			}
		}
		else if (shell.prompt[0] != '\0' && ft_is_space_str(shell.prompt) == 0)
		{
			ft_printf_fd(STDERR_FILENO, "Syntax error\n");
			shell.status = 2;
		}
		add_history(shell.prompt);
	}
	rl_clear_history();
	free_env_list(shell.envp_list);
	free_envp_str(shell.envp);
	return (shell.status);
}
