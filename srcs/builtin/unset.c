/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 18:40:29 by cshingai          #+#    #+#             */
/*   Updated: 2025/01/06 17:39:50 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	unset(char **arg, t_envp **env_list)
{
	int		i;
	char	**new_var;
	int		status_command;
	int		temp_status;

	i = 0;
	status_command = 0;
	temp_status = 0;
	while (arg[i])
	{
		if (is_equal(arg[i], "unset") == 1)
			return (1);
		new_var = ft_split(arg[i], '=');
		temp_status = check_key_name(new_var[0], "unset");
		if (!status_command)
		{
			if (key_exist(new_var[0], *env_list) == 1)
				remove_node_from_list(new_var[0], env_list);
		}
		ft_free_split(new_var);
		i++;
		if (temp_status != 0)
			status_command = temp_status;
	}
	return (status_command);
}

void	remove_node_from_list(char *arg, t_envp **env_list)
{
	t_envp	*temp;
	t_envp	*prev;

	temp = *env_list;
	prev = NULL;
	while (temp)
	{
		if (ft_strcmp(arg, temp->key) == 0)
		{
			if (prev == NULL)
				*env_list = temp->next;
			else
				prev->next = temp->next;
			free(temp->key);
			free(temp->value);
			free(temp);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
}
