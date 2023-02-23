/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 16:23:07 by dly               #+#    #+#             */
/*   Updated: 2023/02/23 18:16:45 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_flag_n(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	else
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(t_proc *proc)
{
	int		option;
	t_list	*tmp;

	option = 0;
	tmp = proc->args->next;
	if (!tmp)
		return (ft_putstr_fd("\n", proc->fd_out), 0);
	while (tmp && is_flag_n(tmp->content))
	{
		option = 1;
		tmp = tmp->next;
	}
	while (tmp)
	{
		if (write(proc->fd_out, tmp->content, ft_strlen(tmp->content)) == -1)
		{
			proc->exit_code = 1;
			g_exit_code = 1;
		}
		tmp = tmp->next;
		if (tmp)
			write(proc->fd_out, " ", 1);
	}
	if (!option)
		write(proc->fd_out, "\n", 1);
	return (0);
}
