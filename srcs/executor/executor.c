/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsouza-r <lsouza-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 20:59:12 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/08 18:55:56 by lsouza-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_path(t_minishell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i] != NULL)
	{
		if (ft_strncmp(shell->envp[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	if (shell->envp[i] != NULL)
		shell->path = ft_split(shell->envp[i] + 5, ':');
	else
		shell->path = NULL;
}

void	get_args(t_list *sub_list, t_execve *exec)
{
	t_list	*node;
	int		i;
	char	**args;

	i = 0;
	node = sub_list;
	if (!sub_list)
		return ;
	exec->cmd = ft_strdup(node->token.lexeme);
	while (node)
	{
		i++;
		node = node->next;
	}
	args = ft_calloc(i + 1, sizeof (char *));
	i = 0;
	node = sub_list;
	while (node)
	{
		args[i] = ft_strdup(node->token.lexeme);
		i++;
		node = node->next;
	}
	args[i] = NULL;
	exec->args = args;
}

int	is_a_dir(char *path)
{
	struct stat	buf;

	if (stat(path, &buf) == 0 && S_ISDIR(buf.st_mode))
		return (1);
	return (0);
}

void	executor(t_tree *tree, t_minishell *shell)
{
	signals_for_command();
	if (tree->tkn_type == COMMAND && tree->sub_list)
		exec_single_cmd(tree, shell);
	else if (tree->tkn_type == COMMAND && tree->sub_list == NULL)
		shell->status = handle_redir(tree, shell);
	else if (tree->tkn_type == PIPE)
	{
		pipe(tree->fd);
		ft_lstadd_back(&(shell->fd_list),
			ft_lstnew((void *)((long)tree->fd[0])));
		ft_lstadd_back(&(shell->fd_list),
			ft_lstnew((void *)((long)tree->fd[1])));
		if (tree->left->tkn_type == PIPE)
		{
			tree->left->parent = tree;
			executor(tree->left, shell);
			handle_pipe(tree, shell, 0);
		}
		else if (tree->left->tkn_type == COMMAND)
			handle_pipe(tree, shell, 1);
		close(tree->fd[0]);
		close(tree->fd[1]);
	}
}

void	wait_pid(t_minishell *shell)
{
	t_lst	*curr;

	curr = shell->pid;
	while (curr)
	{
		waitpid((pid_t)((long)(curr->content)), &shell->status, 0);
		shell->status = WEXITSTATUS(shell->status);
		curr = curr->next;
	}
}
