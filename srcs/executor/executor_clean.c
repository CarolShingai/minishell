/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_clean.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsouza-r <lsouza-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:37:10 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/08 20:40:44 by lsouza-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_error_exec(t_execve *exec, t_minishell *shell, char *full_path)
{
	free(full_path);
	free_execve(exec);
	free_minishell(shell);
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
	close(shell->fd_stdin);
	close(shell->fd_stdout);
}

void	free_execve(t_execve *exec)
{
	ft_free_split(exec->args);
	free(exec->cmd);
	free(exec);
}

void	close_fd(t_minishell *shell)
{
	t_lst	*curr;
	t_lst	*temp;
	int		i;

	curr = shell->fd_list;
	while (curr)
	{
		close((long)curr->content);
		temp = curr;
		curr = curr->next;
		free(temp);
	}
	shell->fd_list = NULL;
	i = 3;
	while (i < 1024)
		close(i++);
}
