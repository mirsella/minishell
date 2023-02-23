/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 18:07:50 by dly               #+#    #+#             */
/*   Updated: 2023/02/23 18:28:23 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_write_fd(char *str, int fd)
{
	if (write(fd, str, ft_strlen(str)) == -1)
		return (-1);
	if (write(fd, "\n", 1) == -1)
		return (-1);
	return (0);
}

int	builtin_env(t_proc *proc, t_list *env)
{
	while (env)
	{
		if (ft_write_fd(env->content, proc->fd_out) == -1)
		{
			print_error("write", "No space left on device");
			proc->exit_code = 125;
			g_exit_code = 125;	
			return (125);
		}
		env = env->next;
	}
	return (0);
}
