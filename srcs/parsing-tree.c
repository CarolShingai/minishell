/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing-tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsouza-r <lsouza-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 19:53:50 by lsouza-r          #+#    #+#             */
/*   Updated: 2024/10/14 18:27:17 by lsouza-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	build_branch(t_list *tkn_list, t_tree *pivot)
{
	pivot->sub_list = hunt_pipe_redir(tkn_list);
	if (pivot->sub_list)
	{	
		pivot->tkn_type = pivot->sub_list->token.type;
		pivot->sub_list->next->prev = NULL;
		pivot->sub_list->prev->next = NULL;
		pivot->right = build_root(pivot->sub_list->next);
		pivot->left = build_root(tkn_list);
		pivot->sub_list->next = NULL;
		pivot->sub_list->prev = NULL;
	}
	else
	{
		pivot->sub_list = tkn_list;
		pivot->left = NULL;
		pivot->right = NULL;
	}
}

t_tree	*build_root(t_list	*tkn_list)
{
	t_tree	*pivot;

	pivot = (t_tree *)ft_calloc(1, sizeof(t_tree));
	build_branch(tkn_list, pivot);
	return (pivot);
}

t_list	*hunt_pipe_redir(t_list *tkn_list)
{
	t_list	*node;
	
	node = hunt_last_pipe(tkn_list);
	if (node)
		return (node);
	node = hunt_redir(tkn_list);
	if (node)
		return (node);
	return (NULL);
}
t_list	*hunt_last_pipe(t_list	*tkn_list)
{
	t_list	*node;
	
	node = NULL;
	node = get_last_token(tkn_list);
	if (node && !node->prev && node->token.type == PIPE)
		return (node);
	while (node)
	{
		if (node->token.type == PIPE)
		{
			return (node);
			break;
		}
		node = node->prev;
	}
	return (NULL);
}

t_list	*hunt_redir(t_list	*tkn_list)
{
	t_list	*node;
	
	node = NULL;
	node = get_last_token(tkn_list);
	if (node && !node->prev && node->token.type >= REDIRECT_INPUT && node->token.type <= REDIRECT_OUTPUT_APPEND)
		return (node);
	while (node)
	{
		if (node->token.type >= REDIRECT_INPUT && node->token.type <= REDIRECT_OUTPUT_APPEND)
		{
			return (node);
			break;
		}
		node = node->prev;
	}
	return (NULL);
}
