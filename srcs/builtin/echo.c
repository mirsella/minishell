/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 16:23:07 by dly               #+#    #+#             */
/*   Updated: 2023/02/17 17:15:13 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	echo(t_proc *proc)
{
	int	option;

	option = 0;
	proc->args = proc->args->next;
	if (!proc->args)
		return (ft_putstr_fd("\n", proc->fd_out), 0);
	if (!ft_strcmp(proc->args->content, "-n"))
	{
		option = 1;
		proc->args = proc->args->next;
	}
	while (proc->args)
	{
		ft_putstr_fd(proc->args->content, proc->fd_out);
		proc->args = proc->args->next;
		if (proc->args)
			write(proc->fd_out, " ", 1);
	}
	if (!option)
		write(proc->fd_out, "\n", 1);
	return (0);
}
