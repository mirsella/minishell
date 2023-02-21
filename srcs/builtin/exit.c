/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 17:38:04 by dly               #+#    #+#             */
/*   Updated: 2023/02/21 20:13:48 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	builtin_exit(t_proc *proc, t_list *env)
{
	int	exit_int;

	exit_int = 0;
	if (proc->args->next)
		exit_int = ft_atoi(proc->args->next->content);
	free_shell_data(env);
	procs_free(&proc);
	exit(exit_int);
}
