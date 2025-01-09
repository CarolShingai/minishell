/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:55:41 by cshingai          #+#    #+#             */
/*   Updated: 2025/01/09 18:11:13 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	env(t_envp *env_list)
{
	print_env_list(env_list);
	return (0);
}

void	print_env_list(t_envp *env_list)
{
	t_envp	*temp;

	temp = env_list;
	while (temp != NULL)
	{
		if (env_list->key)
		{
			printf("%s=%s\n", temp->key, temp->value);
			temp = temp->next;
		}
	}
}
