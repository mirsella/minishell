/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 22:49:48 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/08 22:24:58 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_shell_data(t_data *data)
{
	if (data->env)
		ft_lstclear(&data->env, free);
	if (data->procs)
		procs_free(&data->procs);
	rl_clear_history();
}

void	exit_shell(t_data *data)
{
	free_shell_data(data);
	exit(g_exit_code);
}

void	exit_shell_error(t_data *data, char *msg)
{
	perror(msg);
	free_shell_data(data);
	exit(g_exit_code);
}
