/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:56:04 by cshingai          #+#    #+#             */
/*   Updated: 2024/11/09 15:49:03 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	aux_exec_builting(char *command, char **argv, t_minishell *shell)
{
	int	status_command;

	status_command = 0;
	if (ft_strcmp(command, "pwd") == 0)
		status_command = pwd();
	else if (ft_strcmp(command, "cd") == 0)
		change_directory(&shell->envp_list, *argv);
	else if (ft_strcmp(command, "echo") == 0)
		status_command = echo(argv);
	else if (ft_strcmp(command, "env") == 0)
		status_command = env(shell->envp_list);
	else if (ft_strcmp(command, "exit") == 0)
		status_command = ft_exit(shell, *argv);
	else if (ft_strcmp(command, "export") == 0)
		status_command = export(*argv, &shell->envp_list);
	else if (ft_strcmp(command, "unset") == 0)
		status_command = unset(*argv, &shell->envp_list);
	return (status_command);
}

int	is_builtin(t_tree *tree)
{
	if (ft_strcmp(tree->sub_list->token.lexeme, "pwd")
		|| ft_strcmp(tree->sub_list->token.lexeme, "cd")
		|| ft_strcmp(tree->sub_list->token.lexeme, "echo")
		|| ft_strcmp(tree->sub_list->token.lexeme, "env")
		|| ft_strcmp(tree->sub_list->token.lexeme, "exit")
		|| ft_strcmp(tree->sub_list->token.lexeme, "export")
		|| ft_strcmp(tree->sub_list->token.lexeme, "unset"))
		return (1);
	else
		return (0);
}


int	execute_builtin(t_minishell *shell)
{
	t_builtin	builtin;
	int	status;
	int	i;

	builtin.command = NULL;
	builtin.argv = NULL;
	status = 0;
	i = 0;
	if (shell->tree->tkn_type == 0)
	{
		if(is_builtin(shell->tree))
		{
			get_args_builtin(shell->token_list, &builtin);
			status = aux_exec_builting(builtin.command, builtin.argv, shell);
		}
		else
			return (0);
	}
	while (builtin.argv[i])
		free(builtin.argv[i++]);
	free(builtin.argv);
	free(builtin.command);
	return (status);
}

void	get_args_builtin(t_list *sub_list, t_builtin *builtin)
{
	t_list	*node;
	int		i;
	char	**args;

	i = 0;
	node = sub_list;
	if (!sub_list)
		return ;
	builtin->command = ft_strdup(node->token.lexeme);
	while (node)
	{
		i++;
		node = node->next;
	}
	args = ft_calloc(i + 1, sizeof (char *));
	i = 0;
	node = sub_list;
	node = node->next;
	while (node)
	{
		args[i] = ft_strdup(node->token.lexeme);
		i++;
		node = node->next;
	}
	args[i] = NULL;
	builtin->argv = args;
}
