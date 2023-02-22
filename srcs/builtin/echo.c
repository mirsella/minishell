/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 16:23:07 by dly               #+#    #+#             */
/*   Updated: 2023/02/22 16:19:57 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_echo(t_proc *proc)
{
	int		option;
	t_list	*tmp;

	option = 0;
	tmp = proc->args->next;
	if (!tmp)
		return (ft_putstr_fd("\n", proc->fd_out), 0);
	if (!ft_strcmp(tmp->content, "-n"))
	{
		option = 1;
		tmp = tmp->next;
	}
	while (tmp)
	{
		ft_putstr_fd(tmp->content, proc->fd_out);
		tmp = tmp->next;
		if (tmp)
			write(proc->fd_out, " ", 1);
	}
	if (!option)
		write(proc->fd_out, "\n", 1);
	return (0);
}
