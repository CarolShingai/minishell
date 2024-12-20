/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:27:57 by cshingai          #+#    #+#             */
/*   Updated: 2024/12/13 18:18:02 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_signals(void)
{
	signal(SIGINT, sig_handler_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_for_command(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}

void	sig_handler_sigint(int signal)
{
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", STDIN_FILENO);
	rl_redisplay();
	control_sign(signal + 128);
}

// void	sig_handler_heredoc(int signal)
// {}

int	control_sign(int new_signal)
{
	return (new_signal);
}

// void	update_status(int sign, t_minishell *shell)
// {
// 	shell->status = g_signal;
// }
