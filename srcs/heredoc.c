/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:21:54 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/08 17:52:27 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	hunt_heredoc(t_list *tkn_list, t_minishell *shell)
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
			create_heredoc(current, tag, shell);
			tag++;
		}
		if (g_signal)
		{
			shell->status = g_signal;
			break ;
		}
		current = current->next;
	}
}

void	create_heredoc(t_list *delimiter, int tag, t_minishell *shell)
{
	char	*file_path;
	int		fd;

	file_path = create_file_path(tag);
	fd = open(file_path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	process_heredoc_input(delimiter, shell, fd, file_path);
	close(fd);
	free(file_path);
}

char	*create_file_path(int tag)
{
	char	*file_path;
	char	*curr;

	curr = ft_itoa(tag);
	file_path = ft_strjoin("/tmp/heredoc", curr);
	free(curr);
	return (file_path);
}

void	process_heredoc_input(t_list *delimiter,
		t_minishell *shell, int fd, char *file_path)
{
	char	*line;
	char	*expanded_delimiter;

	expanded_delimiter = check_lexeme(delimiter->token.lexeme, shell, 1);
	printf("expanded_delimiter: %s\n", expanded_delimiter);
	while (1)
	{
		signal(SIGINT, sig_handler_heredoc);
		line = readline("> ");
		if (line == NULL)
			break ;
		if (ft_strcmp(line, expanded_delimiter) != 0)
		{
			expander_heredoc(&line, shell);
			ft_putendl_fd(line, fd);
		}
		else
		{
			free(line);
			break ;
		}
		free(line);
	}
	free(delimiter->token.lexeme);
	delimiter->token.lexeme = ft_strdup(file_path);
	free(expanded_delimiter);
}
