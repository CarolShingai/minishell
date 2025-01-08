/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsouza-r <lsouza-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 21:04:36 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/08 18:41:32 by lsouza-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exec_cmd(t_tree *tree, t_minishell *shell)
{
	t_execve	*exec;
	char		*full_path;

	exec = ft_calloc(1, sizeof(t_execve));
	full_path = NULL;
	get_path(shell);
	get_args(tree->sub_list, exec);
	full_path = find_full_path(exec, shell);
	if (full_path == NULL)
		handle_command_not_found(exec, shell);
	check_executable(full_path, exec, shell);
	execute_command(full_path, exec, shell);
}

char	*find_full_path(t_execve *exec, t_minishell *shell)
{
	int		i;
	char	*path_slash;
	char	*full_path;

	i = 0;
	full_path = NULL;
	if (ft_strchr(exec->cmd, '/') == NULL)
	{
		while (shell->path && shell->path[i])
		{
			path_slash = ft_strjoin(shell->path[i], "/");
			full_path = ft_strjoin(path_slash, exec->cmd);
			free(path_slash);
			if (access(full_path, F_OK | X_OK) == 0)
				break ;
			free(full_path);
			full_path = NULL;
			i++;
		}
		if (shell->path == NULL || full_path == NULL)
			handle_command_not_found(exec, shell);
	}
	else
		full_path = ft_strdup(exec->cmd);
	return (full_path);
}

void	handle_command_not_found(t_execve *exec, t_minishell *shell)
{
	ft_printf_fd(STDERR_FILENO, "%s: No such file or directory\n", exec->cmd);
	free_execve(exec);
	free_minishell(shell);
	exit(127);
}

void	check_executable(char *full_path, t_execve *exec, t_minishell *shell)
{
	if (access(full_path, F_OK) != 0)
	{
		perror(exec->cmd);
		free_error_exec(exec, shell, full_path);
		exit(127);
	}
	if (access(full_path, X_OK) != 0)
	{
		perror(exec->cmd);
		free_error_exec(exec, shell, full_path);
		exit(126);
	}
	if (is_a_dir(full_path))
	{
		ft_printf_fd(STDERR_FILENO, "%s: is a directory\n", exec->cmd);
		free_error_exec(exec, shell, full_path);
		exit(126);
	}
}

void	execute_command(char *full_path, t_execve *exec, t_minishell *shell)
{
	execve(full_path, exec->args, shell->envp);
	perror(full_path);
	free_error_exec(exec, shell, full_path);
	exit(1);
}
