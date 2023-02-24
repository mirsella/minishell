/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 22:49:48 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/24 16:12:59 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_shell_data(t_list *env)
{
	if (env)
		ft_lstclear(&env, free);
	rl_clear_history();
}

void	exit_shell(t_list *env)
{
	free_shell_data(env);
	exit(g_exit_code);
}

void	exit_shell_error(t_list *env, char *msg)
{
	print_errorendl(msg, 0);
	free_shell_data(env);
	exit(g_exit_code);
}
