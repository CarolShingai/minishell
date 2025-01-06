/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing-tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 19:53:50 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/06 18:20:19 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	build_branch(t_list *tkn_list, t_tree *pivot)
{
	pivot->sub_list = hunt_last_pipe(tkn_list);
	if (pivot->sub_list)
	{
		pivot->tkn_type = pivot->sub_list->token.type;
		pivot->sub_list->next->prev = NULL;
		pivot->right = build_root(pivot->sub_list->next);
		pivot->sub_list->prev->next = NULL;
		pivot->left = build_root(tkn_list);
		pivot->sub_list->next = NULL;
		pivot->sub_list->prev = NULL;
	}
	else
	{

		pivot->redir = hunt_redir(&tkn_list);
		pivot->sub_list = tkn_list;
		if (!tkn_list)
		{
			printf("MEXILHÕES!!!!! TA NULO ESSA BAGAÇA\n");
		}
		if (pivot->redir)
			printf("REDIR: %s\n", pivot->redir->file);
		pivot->tkn_type = COMMAND;
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
			break ;
		}
		node = node->prev;
	}
	return (NULL);
}

t_redir	*hunt_redir(t_list	**tkn_list)
{
	t_list	*node;
	t_redir	*redir;
	t_list	*to_be_free;

	node = *tkn_list;
	redir = NULL;
	while (node)
	{
		if (node->token.type >= REDIRECT_INPUT
			&& node->token.type <= REDIRECT_OUTPUT_APPEND)
		{
			ft_lst_add_back(&redir,
				ft_lst_new(node->next->token.lexeme, node->token.type));
			if (node->prev)
				node->prev->next = node->next->next;
			if (node->next->next && node->prev)
				node->next->next->prev = node->prev;
			else if (node->next->next && node->prev == NULL)
			{
				node->next->next->prev = NULL;
				*tkn_list = node->next->next;
			}
			else if (node->prev == NULL && node->next->next == NULL)
			{
				*tkn_list = NULL;
			}
			to_be_free = node;
			node = node->next;
			free(to_be_free);
			to_be_free = NULL;
			to_be_free = node;
			node = node->next;
			free(to_be_free->token.lexeme);
			free(to_be_free);
			to_be_free = NULL;
		}
		else
			node = node->next;
	}
	return (redir);
}
