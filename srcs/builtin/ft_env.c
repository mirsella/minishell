/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 18:07:50 by dly               #+#    #+#             */
/*   Updated: 2023/02/21 20:16:46 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_env(t_proc *proc, t_list *env)
{
	while (env)
	{
		ft_putendl_fd(env->content, proc->fd_out);
		env = env->next;
	}
	return (0);
}
