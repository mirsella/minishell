/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 16:13:17 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/27 19:00:41 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	sig_handler(int signo, siginfo_t *info, void *context)
// {
// 	(void)context;
// 	(void)info;
// 	if (signo == SIGINT)
// 	{
// 		ft_putchar('\n');
// 		rl_on_new_line();
// 		rl_replace_line("", STDIN_FILENO);
// 		rl_redisplay();
// 		g_exit_code = 128 + SIGINT;
// 	}
// 	else if (signo == SIGQUIT)
// 	{
// 		rl_redisplay();
// 		g_exit_code = 128 + SIGQUIT;
// 	}
// }
// 
// void	call_sigaction(void)
// {
// 	struct sigaction	sa;
// 
// 	sa.sa_flags = SA_SIGINFO;
// 	sa.sa_sigaction = sig_handler;
// 	sigemptyset(&sa.sa_mask);
// 	sigaction(SIGINT, &sa, NULL);
// 	// sigaction(SIGQUIT, &sa, NULL);
// }

void	sigint_handler(int signo)
{
	(void)signo;
	ft_putchar('\n');
	rl_on_new_line();
	rl_replace_line("", STDIN_FILENO);
	rl_redisplay();
	g_exit_code = 128 + SIGINT;
}
