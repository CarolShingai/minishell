/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 18:59:24 by cshingai          #+#    #+#             */
/*   Updated: 2024/12/09 17:50:00 by cshingai         ###   ########.fr       */
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
	if (check_key_name(new_env_var->key, "export"))
		add_node_to_list(env_list, new_env_var);
	return (0);
}

char	**new_var_split(char *arg)
{
	char	**new_var;

	if (!arg)
		return (NULL);
	else
		new_var = ft_split(arg, '=');
	return (new_var);
}

void	change_env_value(char *key, char *value, t_envp **env_list)
{
	t_envp	*temp;

	temp = *env_list;
	while (temp && ft_strcmp(temp->key, key) != 0)
			temp = temp->next;
	free(temp->value);
	if (value == NULL)
		value = "";
	temp->value = ft_strdup(value);
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

int	export(char **arg, t_envp **env_list)
{
	char	*var;
	char	**new_var;
	int		status_command;
	int		i;

	status_command = 0;
	var = NULL;
	i = -1;
	if (*arg == NULL)
	{
		order_env_list(env_list);
		print_env_list(*env_list);
		return (status_command);
	}
	while (arg[++i])
	{
		new_var = new_var_split(arg[i]);
		if (ft_check_key(new_var[0], *env_list) == 1)
			change_env_value(new_var[0], new_var[1], env_list);
		else
			status_command = export_new_var(new_var, env_list);
		ft_free_split(new_var);
	}
	return (status_command);
}
