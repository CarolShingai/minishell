/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:49:25 by cshingai          #+#    #+#             */
/*   Updated: 2024/10/09 17:06:42 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
	In this first part of the main I have created the prompt. The prompt is
	created by the function readline. There are many functions, that still need
	to explore and probably will need to use, from the library of readline. But
	so far addhistory is the one who will bring history when press arrow up.
*/


static void	print_list(t_list *list)
{
	t_list *aux;

	aux = list;
	while (aux)
	{
		printf("lexema: %s type: %d\n", aux->token.lexeme, aux->token.type);
		aux = aux->next;
	}
}

int	main(void)
{
	char	*prompt;
	t_minishell	base;

	while (1)
	{
		prompt = readline("minihell: ");
		if (prompt == NULL)
			break;
		base.token_list = tokenizer(prompt);
		// push_prompt_to_list(&prompt);
		print_list(base.token_list);
		if (strcmp(prompt, "exit") == 0)
		{
			free(prompt);
			free_list(&base.token_list);
			exit(1);
		}
		add_history(prompt);
		free_list(&base.token_list);
	}
}
