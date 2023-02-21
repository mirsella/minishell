/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:51:18 by dly               #+#    #+#             */
/*   Updated: 2023/02/21 20:52:08 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	double_dup2(int in, int out)
{
	if (dup2(in, STDIN_FILENO) == -1)
		return (-1);
	if (dup2(out, STDOUT_FILENO) == -1)
		return (-1);
	return (0);
}

int	open_pipe(t_proc *proc)
{
	while (proc)
	{
		if (proc->type == SUBSHELL)
			open_pipe(proc->procs);
		if (proc->next_pipeline == PIPE)
		{
			if (pipe(proc->pipes) == -1)
				return (-1);
		}
		proc = proc->next;
	}
	return (0);
}

void	close_pipe(t_proc *proc)
{
	while (proc)
	{
		if (proc->type == SUBSHELL)
			close_pipe(proc->procs);
		if (proc->next_pipeline == PIPE)
		{
			close(proc->pipes[0]);
			close(proc->pipes[1]);
		}
		proc = proc->next;
	}
}

void	assign_pipe_cmd(t_proc *proc)
{
	if (proc->fd_out == STDOUT_FILENO)
	{
		proc->fd_out = proc->pipes[1];
		if (proc->next && proc->next->fd_in == STDIN_FILENO)
			proc->next->fd_in = proc->pipes[0];
	}
}

void	assign_pipe_subshell(t_proc *tmp, t_proc *proc, t_list *env)
{
	if (proc->next_pipeline == PIPE)
	{
		if (proc->next->fd_in == STDIN_FILENO)
			proc->next->fd_in = proc->pipes[0];
		while (proc->procs)
		{
			if (proc->procs->next_pipeline != PIPE)
			{
				if (proc->procs->fd_out == STDOUT_FILENO)
					proc->procs->fd_out = proc->pipes[1];
			}
			proc->procs = proc->procs->next;
		}
		process(tmp, env);
	}
}
