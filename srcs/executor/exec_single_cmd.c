/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsouza-r <lsouza-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 22:07:17 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/07 22:13:58 by lsouza-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exec_single_cmd(t_tree *tree, t_minishell *shell)
{
	int	pid;

	if (is_builtin(tree) == 1)
	{
		exec_single_builtin(tree, shell);
		return ;
	}
	else
	{
		pid = fork();
		if (pid == 0)
			execute_single_cmd(tree, shell);
		waitpid(pid, &shell->status, 0);
		shell->status = WEXITSTATUS(shell->status);
	}
}

void	exec_single_builtin(t_tree *tree, t_minishell *shell)
{
	if (handle_redir(tree, shell) == 0)
	{
		expander(tree->sub_list, shell);
		shell->status = execute_builtin(shell, tree);
	}
	else
		shell->status = 1;
}

void	execute_single_cmd(t_tree *tree, t_minishell *shell)
{
	if (handle_redir(tree, shell) == 0)
	{
		expander(tree->sub_list, shell);
		close_fd(shell);
		exec_cmd(tree, shell);
	}
	else
	{
		free_minishell(shell);
		close_fd(shell);
	}
	exit(1);
}