/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 18:07:50 by dly               #+#    #+#             */
/*   Updated: 2023/02/24 16:16:41 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_env(t_proc *proc, t_list *env)
{
	while (env)
	{
		if (ft_putendl_fd(env->content, proc->fd_out) == -1)
		{
			print_errorendl("env: write", strerror(errno));
			return (125);
		}
		env = env->next;
	}
	return (0);
}
