/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:06:08 by cshingai          #+#    #+#             */
/*   Updated: 2025/01/05 18:25:14 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	*ft_free_split(char **split)
{
	int	i;

	if (!split)
		return (NULL);
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (NULL);
}

void	init_shell(t_minishell *shell)
{
	shell->token_list = NULL;
	shell->tree = NULL;
	shell->envp_list = NULL;
	shell->envp = NULL;
	shell->status = 0;
	shell->pid = NULL;
	shell->fd_list = NULL;
	shell->path = NULL;
	g_signal = 0;
	shell->fd_stdin = dup(STDIN_FILENO);
	shell->fd_stdout = dup(STDOUT_FILENO);
}

int	ft_is_space_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_is_space(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}
