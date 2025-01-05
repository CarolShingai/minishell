/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:32:45 by cshingai          #+#    #+#             */
/*   Updated: 2024/11/09 15:49:22 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	echo(char **arg)
{
	int	i;
	int	has_flag;

	i = 0;
	has_flag = 0;
	if (arg == NULL)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (0);
	}

	// Aqui tem que ver se vai aceitar "-nnnn" e "-ne"
	// Ou só "-n" e "-n -n"

	while (arg[i] && ft_strncmp(arg[i], "-n", 2) == 0)
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
