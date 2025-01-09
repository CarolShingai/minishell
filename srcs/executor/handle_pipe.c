/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 22:28:47 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/09 18:30:26 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	execute_left(t_tree *tree, t_minishell *shell)
{
	dup2(tree->fd[1], STDOUT_FILENO);
	close(tree->fd[0]);
	close(tree->fd[1]);
	if (handle_redir(tree->left, shell) == 0)
	{
		expander(tree->left->sub_list, shell);
		close_fd(shell);
		if (is_builtin(tree->left))
		{
			shell->status = execute_builtin(shell, tree->left);
			free_minishell(shell);
			exit(shell->status);
		}
		exec_cmd(tree->left, shell);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	free_minishell(shell);
	exit(1);
}

void	execute_right(t_tree *tree, t_minishell *shell)
{
	dup2(tree->fd[0], STDIN_FILENO);
	if (tree->parent)
	{
		dup2(tree->parent->fd[1], STDOUT_FILENO);
		close(tree->parent->fd[1]);
	}
	close(tree->fd[0]);
	close(tree->fd[1]);
	if (handle_redir(tree->right, shell) == 0)
	{
		expander(tree->right->sub_list, shell);
		close_fd(shell);
		if (is_builtin(tree->right))
		{
			shell->status = execute_builtin(shell, tree->right);
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			free_minishell(shell);
			exit(shell->status);
		}
		exec_cmd(tree->right, shell);
	}
	close_stdio();
	free_minishell(shell);
	exit(1);
}

int	handle_pipe(t_tree *tree, t_minishell *shell, int left)
{
	pid_t	pid[2];

	if (left == 1)
	{
		pid[0] = fork();
		if (pid[0] == 0)
			execute_left(tree, shell);
		ft_lstadd_back(&(shell->pid), ft_lstnew((void *)((long)pid[0])));
	}
	pid[1] = fork();
	if (pid[1] == 0)
		execute_right(tree, shell);
	ft_lstadd_back(&(shell->pid), ft_lstnew((void *)((long)pid[1])));
	return (0);
}

void	close_stdio(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}
