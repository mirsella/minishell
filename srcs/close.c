/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 22:49:48 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/07 23:57:32 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exit_shell(t_data *data)
{
	if (data->env)
		ft_lstclear(&data->env, free);
	if (data->procs)
		procs_free(&data->procs);
	rl_clear_history();
}
