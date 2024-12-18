/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsouza-r <lsouza-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:21:54 by lsouza-r          #+#    #+#             */
/*   Updated: 2024/12/14 21:18:29 by lsouza-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	hunt_heredoc(t_list *tkn_list)
{
	t_list	*current;
	int		tag;

	tag = 0;
	current = tkn_list;
	while (current)
	{
		if (current->token.type == REDIRECT_HEREDOC)
		{
			current = current->next;
			create_heredoc(current, tag);
			tag++;
		}
		current->next;
	}
}

void	create_heredoc(t_list *delimiter, int tag)
{
	char	*file_path;
	char	*curr;
	int		fd;
	int		fd_stdin;
	char	*line;

	curr = ft_itoa(tag);
	file_path = ft_strjoin("/tmp/heredoc", curr);
	free(curr);
	fd = open(file_path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	while (1)
	{
		line = readline(">");
		if (ft_strncmp(line, delimiter->token.lexeme, ft_strlen(delimiter->token.lexeme) - 1) != 0)
			ft_putstr_fd(line, fd);
		else
			break ;
		free(line);
	}
	
	

	
}