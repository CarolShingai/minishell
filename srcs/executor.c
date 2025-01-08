/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 20:59:12 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/08 17:52:43 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * get_path - Retrieves the PATH environment variable and splits it into an array.
 * @shell: Pointer to the minishell structure containing environment variables.
 */
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
	if (shell->envp[i] != NULL)
		shell->path = ft_split(shell->envp[i] + 5, ':');
	else
		shell->path = NULL;
}

/**
 * get_args - Extracts command arguments from a linked list and stores them in exec structure.
 * @sub_list: Pointer to the linked list containing command tokens.
 * @exec: Pointer to the execve structure to store command and arguments.
 */
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

void	free_minishell(t_minishell *shell)
{
	close_fd(shell);
	ft_free_split(shell->path);
	free_tree(&shell->tree);
	free_env_list(shell->envp_list);
	shell->envp_list = NULL;
	free_envp_str(shell->envp);
	rl_clear_history();
	free_pid_list(&shell->pid);
}

void	free_execve(t_execve *exec)
{
	ft_free_split(exec->args);
	free(exec->cmd);
	free(exec);
}

/**
 * executor - Executes commands or pipelines based on the syntax tree.
 * @tree: Pointer to the syntax tree node.
 * @shell: Pointer to the minishell structure containing environment variables.
 */
void	executor(t_tree *tree, t_minishell *shell)
{
	signals_for_command();
	if (tree->tkn_type == COMMAND && tree->sub_list)
		exec_single_cmd(tree, shell);
	else if (tree->tkn_type == COMMAND && tree->sub_list == NULL)
		shell->status = handle_redir(tree, shell);
	else if (tree->tkn_type == PIPE && tree->left->tkn_type == PIPE)
	{
		tree->left->parent = tree;
		pipe(tree->fd);
		ft_lstadd_back(&(shell->fd_list), ft_lstnew((void *)((long)tree->fd[0])));
		ft_lstadd_back(&(shell->fd_list), ft_lstnew((void *)((long)tree->fd[1])));
		executor(tree->left, shell);
		handle_pipe(tree, shell, 0);
		close(tree->fd[0]);
		close(tree->fd[1]);
	}
	else if (tree->tkn_type == PIPE && tree->left->tkn_type == COMMAND)
	{
		pipe(tree->fd);
		ft_lstadd_back(&(shell->fd_list), ft_lstnew((void *)((long)tree->fd[0])));
		ft_lstadd_back(&(shell->fd_list), ft_lstnew((void *)((long)tree->fd[1])));
		handle_pipe(tree, shell, 1);
		close(tree->fd[0]);
		close(tree->fd[1]);
	}
}

/**
 * exec_single_cmd - Executes a single command, handling redirections and built-ins.
 * @tree: Pointer to the syntax tree node containing the command.
 * @shell: Pointer to the minishell structure containing environment variables.
 */


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

void	close_fd(t_minishell *shell)
{
	t_lst	*curr;
	t_lst	*temp;

	curr = shell->fd_list;
	while (curr)
	{
		close((long)curr->content);
		temp = curr;
		curr = curr->next;
		free(temp);
	}
	shell->fd_list = NULL;
}
