/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 17:07:56 by cshingai          #+#    #+#             */
/*   Updated: 2025/01/09 18:03:41 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_exit(t_minishell *shell, char **arg)
{
	int	exit_status;

	if (arg[0] && (is_numeric_arg(arg[0]) == 0
			|| valide_arg_length(arg[0]) == 0))
		exit_status = 2;
	else if (check_too_many_args(arg) == 0)
		return (1);
	else if (arg[0])
		exit_status = ft_atoi(*arg);
	else
		exit_status = shell->status;
	clear_args(shell->builtin.argv);
	free(shell->builtin.command);
	free_tree(&shell->tree);
	free_env_list(shell->envp_list);
	free_envp_str(shell->envp);
	rl_clear_history();
	free(shell->prompt);
	close_fd(shell);
	close(shell->fd_stdin);
	close(shell->fd_stdout);
	free_pid_list(&shell->pid);
	exit_status = exit_status % 256;
	ft_printf_fd(STDERR_FILENO, "exit\n");
	exit(exit_status);
}

int	check_exit_arg(char **arg)
{
	if (!is_numeric_arg(*arg))
		return (0);
	if (!check_too_many_args(arg))
		return (0);
	return (1);
}

int	check_too_many_args(char **arg)
{
	int	idx1;

	idx1 = 0;
	while (arg[idx1])
		idx1++;
	if (idx1 > 1)
	{
		ft_putstr_fd(
			"minihell: exit: too many arguments\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	is_numeric_arg(char *arg)
{
	int	idx;

	idx = 0;
	if (arg[0] == '\0')
	{
		ft_printf_fd(STDERR_FILENO,
			"minihell: exit: %s: numeric argument required\n", arg);
		return (0);
	}
	while (arg[idx])
	{
		if (ft_isalpha(arg[idx]))
		{
			ft_printf_fd(STDERR_FILENO,
				"minihell: exit: %s: numeric argument required\n", arg);
			return (0);
		}
		idx++;
	}
	return (1);
}

int	valide_arg_length(char *arg)
{
	if (ft_strlen(arg) > 19)
	{
		ft_printf_fd(STDERR_FILENO,
			"minihell: exit: %s: numeric argument required\n", arg);
		return (0);
	}
	return (1);
}
