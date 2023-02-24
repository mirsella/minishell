/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 16:23:07 by dly               #+#    #+#             */
/*   Updated: 2023/02/24 16:16:11 by mirsella         ###   ########.fr       */
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

int	putnewline(int fd)
{
	if (ft_putchar_fd('\n', fd) == -1)
		return (print_errorendl("echo: write error", strerror(errno)));
	return (0);
}

int	builtin_echo(t_proc *proc)
{
	int		option;
	t_list	*tmp;

	option = 0;
	tmp = proc->args->next;
	if (!tmp)
		return (putnewline(proc->fd_out));
	while (tmp && is_flag_n(tmp->content))
	{
		option = 1;
		tmp = tmp->next;
	}
	while (tmp)
	{
		if (ft_putstr_fd(tmp->content, proc->fd_out) == -1)
			return (print_errorendl("echo: write error", strerror(errno)));
		tmp = tmp->next;
		if (tmp)
			if (ft_putchar_fd(' ', proc->fd_out) == -1)
				return (print_errorendl("echo: write error", strerror(errno)));
	}
	if (!option)
		return (putnewline(proc->fd_out));
	return (0);
}
