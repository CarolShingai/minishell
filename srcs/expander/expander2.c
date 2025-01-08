/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 17:31:39 by cshingai          #+#    #+#             */
/*   Updated: 2025/01/06 20:52:48 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expander_heredoc(char **line, t_minishell *shell)
{
	char	*temp;

	temp = *line;
	*line = check_lexeme(*line, shell, 0);
	free(temp);
}

char	**handle_state(char *str, t_minishell *shell, char **result)
{
	char	*itoa_status;
	char	*tmp_result;

	if (*result == NULL)
	{
		*result = ft_calloc(1, sizeof (char));
		(*result)[0] = '\0';
	}
	if (*str == '?')
	{
		itoa_status = ft_itoa(shell->status);
		tmp_result = *result;
		*result = ft_strjoin(*result, itoa_status);
		free(tmp_result);
		free(itoa_status);
	}
	return (result);
}

void	handle_quotes(char c, int *quotes, char **result)
{
	if (*quotes == 0)
	{
		if (c == '\"')
			(*quotes) = 1;
		else if (c == '\'')
			(*quotes) = 2;
	}
	else if ((*quotes == 1 && c == '\"') || (*quotes == 2 && c == '\''))
		(*quotes) = 0;
	else if ((*quotes == 1 && c == '\'') || (*quotes == 2 && c == '\"'))
		expander_word(c, result);
}
