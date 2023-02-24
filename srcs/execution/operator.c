/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 20:59:21 by dly               #+#    #+#             */
/*   Updated: 2023/02/24 21:02:15 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_true(t_proc *proc, int is_subshell)
{
	while (proc)
	{
		if (proc->next_pipeline == AND)
		{
			if (proc->exit_code != 0)
				return (0);
			if (is_subshell != 2 && proc->next && proc->next->exit_code != 0)
				return (0);
		}
		if (proc->next_pipeline == OR)
		{
			if (proc->exit_code != 0 && is_subshell == 2)
				return (0);
			if (proc->exit_code == 0)
				return (1);
			if (is_subshell != 2 && proc->next && proc->next->exit_code == 0)
				return (1);
		}
		if (!is_subshell)
			break ;
		proc = proc->next;
	}
	return (1);
}

int	recursive_and_or(t_proc *proc, t_list *env, int need_open)
{
	t_proc	*tmp;
	int		is_subshell;

	tmp = proc;
	is_subshell = 2;
	if (proc->prev && proc->prev->type != SUBSHELL)
	{
		tmp = proc->prev;
		is_subshell = 0;
	}
	if (proc->procs)
	{
		tmp = proc->procs;
		is_subshell = 1;
	}
	if (!is_true(tmp, is_subshell) && proc->next_pipeline == AND)
		return (0);
	if (is_true(tmp, is_subshell) && proc->next_pipeline == OR)
		return (0);
	if (need_open)
		open_pipe(proc);
	process(proc->next, env);
	return (0);
}
