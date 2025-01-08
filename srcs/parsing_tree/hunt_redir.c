/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hunt_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 22:28:55 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/08 18:33:24 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_redir	*hunt_redir(t_list	**tkn_list)
{
	t_list	*node;
	t_redir	*redir;

	node = *tkn_list;
	redir = NULL;
	while (node)
	{
		if (node->token.type >= REDIRECT_INPUT
			&& node->token.type <= REDIRECT_OUTPUT_APPEND)
		{
			ft_lst_add_back(&redir,
				ft_lst_new(node->next->token.lexeme, node->token.type));
			redir_removal(&node, tkn_list);
		}
		else
			node = node->next;
	}
	return (redir);
}

void	redir_removal(t_list **node, t_list	**tkn_list)
{
	t_list	*to_be_free;

	if ((*node)->prev)
		(*node)->prev->next = (*node)->next->next;
	if ((*node)->next->next && (*node)->prev)
		(*node)->next->next->prev = (*node)->prev;
	else if ((*node)->next->next && (*node)->prev == NULL)
	{
		(*node)->next->next->prev = NULL;
		*tkn_list = (*node)->next->next;
	}
	else if ((*node)->prev == NULL && (*node)->next->next == NULL)
		*tkn_list = NULL;
	to_be_free = *node;
	*node = (*node)->next;
	free(to_be_free);
	to_be_free = NULL;
	to_be_free = *node;
	*node = (*node)->next;
	free(to_be_free->token.lexeme);
	free(to_be_free);
	to_be_free = NULL;
}
