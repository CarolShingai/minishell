/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:36:25 by cshingai          #+#    #+#             */
/*   Updated: 2024/10/23 17:45:03 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_envp	*env_create_node(void)
{
	t_envp	*new_node;

	new_node = malloc(sizeof(t_envp));
	new_node->key = NULL;
	new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	print_env_list(t_envp *env_list)
{
	t_envp	*temp;

	temp = env_list;
	while (temp != NULL)
	{
		printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
}

void	add_node_to_list(t_envp **head, t_envp *node)
{
	t_envp	*temp;

	if (*head == NULL)
		*head = node;
	else
	{
		temp = *head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = node;
	}
}

t_envp	*node_from_environ(char *environ)
{
	t_envp	*node;
	char	**split_environ;

	split_environ = ft_split(environ, '=');
	node = env_create_node();
	node->key = split_environ[0];
	node->value = split_environ[1];
	free(split_environ);
	return (node);
}

t_envp	*creat_env_list(char **environ)
{
	t_envp	*envp_list;
	t_envp	*head;
	int		i;

	head = NULL;
	i = 0;
	while (environ[i])
	{
		envp_list = node_from_environ(environ[i]);
		add_node_to_list(&head, envp_list);
		i++;
	}
	return (head);
}
