/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 20:59:12 by lsouza-r          #+#    #+#             */
/*   Updated: 2024/11/20 19:52:32 by cshingai         ###   ########.fr       */
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
			break;
		i++;
	}
	shell->path = ft_split(shell->envp[i] + 5, ':');
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

void	exec_cmd(t_tree	*tree, t_minishell *shell)
{
	t_execve	*exec;
	int			i;
	char		*path_slash;
	char		*full_path;

	i = 0;
	exec = ft_calloc(1, sizeof(t_execve));
	get_path(shell);
	get_args(tree->sub_list, exec);
	while (shell->path[i])
	{
		path_slash = ft_strjoin(shell->path[i], "/");
		full_path = ft_strjoin(path_slash, exec->cmd);
		free(path_slash);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			execve(full_path, exec->args, shell->envp);
			break;
		}
		free(full_path);
		i++;
	}
	ft_free_split(shell->path);
	ft_free_split(exec->args);
	free(exec->cmd);
	free(exec);
}

void	executor(t_tree *tree, t_minishell *shell)
{
	if (tree->tkn_type == COMMAND)
		exec_single_cmd(tree, shell);
	else if (tree->tkn_type == PIPE && tree->left->tkn_type == PIPE)
	{
		tree->left->parent = tree;
		pipe(tree->fd);
		executor(tree->left, shell);
		handle_pipe(tree, shell, 0);
	}
	else if (tree->tkn_type == PIPE && tree->left->tkn_type == COMMAND)
	{
		pipe(tree->fd);
		handle_pipe(tree, shell, 1);
	}
}

int	handle_pipe(t_tree *tree, t_minishell *shell, int left)
{
	pid_t pid[2];

	if (left == 1)
	{
		pid[0] = fork();
		if (pid[0] == 0)
		{
			dup2(tree->fd[1], STDOUT_FILENO);
			close(tree->fd[0]);
			close(tree->fd[1]);
			handle_redir(tree->left);
			if (is_builtin(tree->left))
			{
				execute_builtin(shell, tree->left);
				exit(1);
			}
			exec_cmd(tree->left, shell);
		}
	}
	pid[1] = fork();
	if (pid[1] == 0)
	{
		dup2(tree->fd[0], STDIN_FILENO);
		if (tree->parent)
		{
			dup2(tree->parent->fd[1], STDOUT_FILENO);
			close(tree->parent->fd[1]);
			close(tree->parent->fd[0]);
		}
		close(tree->fd[0]);
		close(tree->fd[1]);
		handle_redir(tree->right);
		if (is_builtin(tree->right))
		{
			execute_builtin(shell, tree->right);
			exit(1);
		}
		exec_cmd(tree->right, shell);
	}
	close(tree->fd[1]);
	if (left)
	{
		close(tree->fd[0]);
		waitpid(pid[0], NULL, 0);
	}
	waitpid(pid[1], NULL, 0);
	return (0);
}

void	handle_redir(t_tree	*tree)
{
	t_redir	*node;
	int		fd;

	node = tree->redir;
	while (node)
	{
		if (node->rd_type == REDIRECT_OUTPUT)
		{
			fd = open(node->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (node->rd_type == REDIRECT_OUTPUT_APPEND)
		{
			fd = open(node->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (node->rd_type == REDIRECT_INPUT)
		{
			fd = open(node->file, O_RDONLY);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		node = node->next;
	}
}

void	exec_single_cmd(t_tree *tree, t_minishell *shell)
{
	int	pid;

	if (is_builtin(tree) == 1)
	{
		handle_redir(tree);
		execute_builtin(shell, tree);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		handle_redir(tree);
		exec_cmd(tree, shell);
	}
	waitpid(pid, NULL, 0);
}