/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 18:59:24 by cshingai          #+#    #+#             */
/*   Updated: 2025/01/06 17:06:43 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	export_new_var(char **new_var, t_envp **env_list)
{
	t_envp	*new_env_var;

	new_env_var = env_create_node();
	if (!new_env_var)
		return (1);
	new_env_var->key = ft_strdup(new_var[0]);
	if (new_var[1])
		new_env_var->value = ft_strdup(new_var[1]);
	else
		new_env_var->value = ft_strdup("");
	add_node_to_list(env_list, new_env_var);
	return (0);
}

void	change_env_value(char *key, char *value, t_envp **env_list)
{
	t_envp	*temp;

	temp = *env_list;
	while (temp && ft_strcmp(temp->key, key) != 0)
		temp = temp->next;
	if (temp)
	{
		free(temp->value);
		if (value == NULL)
			value = "";
		temp->value = ft_strdup(value);
	}
}

void	order_env_list(t_envp **env_list)
{
	char	*temp_key;
	char	*temp_value;
	t_envp	*temp;

	temp = *env_list;
	while (temp && temp->next)
	{
		if (ft_strcmp(temp->key, temp->next->key) > 0)
		{
			temp_key = temp->key;
			temp_value = temp->value;
			temp->key = temp->next->key;
			temp->value = temp->next->value;
			temp->next->key = temp_key;
			temp->next->value = temp_value;
			order_env_list(env_list);
		}
		temp = temp->next;
	}
}

int	export_no_args(t_envp **env_list)
{
	t_envp	*temp_env_list;

	temp_env_list = env_list_copy(*env_list);
	order_env_list(&temp_env_list);
	print_env_list(temp_env_list);
	free_env_list(temp_env_list);
	return (0);
}

int	export(char **arg, t_envp **env_list)
{
	int		status_command;
	int		status_tmp;
	int		i;

	i = 0;
	status_command = 0;
	if (*arg == NULL)
		export_no_args(env_list);
	while (arg[i])
	{
		if (is_equal(arg[i], "export") == 1)
		{
			status_command = 1;
			i++;
		}
		status_tmp = insert_var(arg[i], env_list);
		if (status_tmp != 0)
			status_command = 1;
		i++;
	}
	return (status_command);
}
