/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 21:39:32 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/08 21:02:39 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_redir(t_tree	*tree, t_minishell *shell)
{
	t_redir	*node;
	char	*expanded_file;

	node = tree->redir;
	expanded_file = NULL;
	while (node)
	{
		expanded_file = check_lexeme(node->file, shell, 0);
		if (node->rd_type == REDIRECT_OUTPUT
			|| node->rd_type == REDIRECT_OUTPUT_APPEND)
		{
			if (handle_output_append(node, expanded_file, shell) == 1)
				return (1);
		}
		else if (node->rd_type == REDIRECT_INPUT
			|| node->rd_type == REDIRECT_HEREDOC)
		{
			if (handle_input_heredoc(node, expanded_file, shell) == 1)
				return (1);
		}
		free(expanded_file);
		expanded_file = NULL;
		node = node->next;
	}
	return (0);
}

int	handle_output_append(t_redir *redir, char *file, t_minishell *shell)
{
	int	fd;

	if (redir->rd_type == REDIRECT_OUTPUT)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(file);
		free(file);
		return (1);
	}
	ft_lstadd_back(&(shell->fd_list),
		ft_lstnew((void *)((long)fd)));
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_input_heredoc(t_redir *redir, char *file, t_minishell *shell)
{
	int	fd;

	if (redir->rd_type == REDIRECT_INPUT)
	{
		fd = open(file, O_RDONLY);
		if (fd == -1)
		{
			perror(file);
			free(file);
			return (1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else
	{
		fd = open(file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	ft_lstadd_back(&(shell->fd_list),
		ft_lstnew((void *)((long)fd)));
	return (0);
}
