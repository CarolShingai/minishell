/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:01:39 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/06 20:52:24 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expander(t_list *sub_list, t_minishell *shell)
{
	t_list	*node;
	char	*temp;

	node = sub_list;
	while (node)
	{
		temp = node->token.lexeme;
		node->token.lexeme = check_lexeme(node->token.lexeme, shell, 0);
		free(temp);
		node = node->next;
	}
}

char	*check_lexeme(char *str, t_minishell *shell, int is_delimit)
{
	int		i;
	int		quotes;
	char	*result;

	i = 0;
	result = NULL;
	quotes = 0;
	while (str[i] != '\0')
	{
		if (is_delimit == 0 && (str[i] == '$'
				&& str[i + 1] != '\0' && (ft_isalpha(str[i + 1])
					|| str[i + 1] == '_' || str[i + 1] == '?')))
		{
			if (quotes == 0 || quotes == 1)
				i = expander_var(str, i, shell, &result);
			else
				expander_word(str[i], &result);
		}
		else if (str[i] == '\"' || str[i] == '\'')
			handle_quotes(str[i], &quotes, &result);
		else
			expander_word(str[i], &result);
		i++;
	}
	return (result);
}

int	expander_var(char *str, int i, t_minishell *shell, char **result)
{
	int		start;
	int		end;
	char	*var;

	start = ++i;
	if (*result == NULL)
	{
		*result = ft_calloc(1, sizeof (char));
		(*result)[0] = '\0';
	}
	if (str[i - 1] == '$' && str[i] == '?')
		i++;
	else
	{
		while ((ft_isalnum(str[i]) || str[i] == '_') && str[i] != '\0')
			i = i + 1;
	}
	end = i - 1;
	var = ft_substr(str, start, end - start + 1);
	handle_variable(var, shell, result);
	free(var);
	return (i - 1);
}

void	handle_variable(char *var, t_minishell *shell, char **result)
{
	char	*value;
	char	*temp;

	if (key_exist(var, shell->envp_list) == 1)
	{
		value = ft_strdup(ft_getenv(var, shell->envp_list));
		temp = *result;
		*result = ft_strjoin(*result, value);
		free(temp);
		free(value);
	}
	else if (ft_strcmp(var, "?") == 0)
		handle_state(var, shell, result);
}

void	expander_word(char c, char **result)
{
	char	*temp1;
	char	*temp2;

	if (*result == NULL)
	{
		*result = ft_calloc(2, sizeof (char));
		(*result)[0] = c;
		(*result)[1] = '\0';
	}
	else
	{
		temp1 = ft_calloc(2, sizeof (char));
		temp1[0] = c;
		temp1[1] = '\0';
		temp2 = *result;
		*result = ft_strjoin(*result, temp1);
		free(temp1);
		free(temp2);
	}
}
