/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:32:45 by cshingai          #+#    #+#             */
/*   Updated: 2024/10/29 17:44:41 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	echo(char **arg)
{
	int	i;
	int	has_flag;

	i = 1; // Start from 1 to skip the command name
	has_flag = 0;
	if (arg[i] && ft_strcmp(arg[i], "-n") == 0)
	{
		has_flag = 1;
		i++;
	}
	while (arg[i])
	{
		ft_putstr_fd(arg[i], STDOUT_FILENO);
		if (arg[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!has_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
