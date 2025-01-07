/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 17:49:59 by cshingai          #+#    #+#             */
/*   Updated: 2025/01/06 20:57:49 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_redir	*ft_lst_new(char *file, int rd_type)
{
	t_redir	*new_node;

	new_node = ft_calloc(1, sizeof(t_redir));
	if (!new_node)
		return (NULL);
	new_node->file = ft_strdup(file);
	new_node->rd_type = rd_type;
	new_node->next = NULL;
	return (new_node);
}

void	ft_lst_add_back(t_redir **lst, t_redir *new)
{
	t_redir	*last;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = ft_lst_last(*lst);
	last->next = new;
}

t_redir	*ft_lst_last(t_redir *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

void	free_redir(t_redir **redir_list)
{
	t_redir	*aux1;
	t_redir	*aux2;

	if (redir_list)
	{
		aux1 = *redir_list;
		while (aux1)
		{
			aux2 = aux1->next;
			if (aux1->file)
				free(aux1->file);
			free(aux1);
			aux1 = aux2;
		}
		*redir_list = NULL;
	}
}

void	free_pid_list(t_lst **pid)
{
	t_lst	*curr;

	while (*pid)
	{
		curr = *pid;
		*pid = (*pid)->next;
		free(curr);
	}
	*pid = NULL;
}
