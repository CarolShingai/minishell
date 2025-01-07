/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:26:31 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/06 20:57:45 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_list	*token_create_node(char *lexeme, int token_type)
{
	t_list	*new_token;

	new_token = malloc(sizeof(t_list));
	new_token->token.lexeme = lexeme;
	new_token->token.type = token_type;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

void	token_add_to_list(t_list **token_list, char *lexeme, int token_type)
{
	t_list	*new_token;
	t_list	*last_node;

	last_node = *token_list;
	new_token = token_create_node(lexeme, token_type);
	while (last_node && last_node->next)
		last_node = last_node->next;
	if (last_node)
	{
		last_node->next = new_token;
		last_node->next->prev = last_node;
	}
	else
		*token_list = new_token;
}

void	free_list(t_list **token_list)
{
	t_list	*aux1;
	t_list	*aux2;

	if (token_list)
	{
		aux1 = *token_list;
		while (aux1)
		{
			aux2 = aux1->next;
			if (aux1->token.lexeme)
				free(aux1->token.lexeme);
			free(aux1);
			aux1 = aux2;
		}
		*token_list = NULL;
	}
}

void	*free_tree(t_tree **tree)
{
	if (*tree == NULL)
		return (NULL);
	if ((*tree)->right)
		free_tree(&(*tree)->right);
	if ((*tree)->left)
		free_tree(&(*tree)->left);
	if ((*tree)->sub_list)
		free_list(&(*tree)->sub_list);
	if ((*tree)->redir)
		free_redir(&(*tree)->redir);
	free(*tree);
	*tree = NULL;
	return (NULL);
}

t_list	*get_last_token(t_list	*tkn_list)
{
	t_list	*tmp;
	int		i;

	i = 0;
	if (!tkn_list)
		return (NULL);
	tmp = tkn_list;
	while (tmp->next != NULL)
	{
		tmp->pos = i;
		tmp = tmp->next;
		i++;
	}
	tmp->pos = i;
	return (tmp);
}
