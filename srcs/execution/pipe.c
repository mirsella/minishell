/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:51:18 by dly               #+#    #+#             */
/*   Updated: 2023/02/27 22:31:42 by mirsella         ###   ########.fr       */
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
			if (open_pipe(proc->procs))
				return (-1);
		if (proc->next_pipeline == PIPE)
		{
			if (pipe(proc->pipes) == -1)
				return (-1);
		}
		proc = proc->next;
	}
	return (0);
}

void	close_pipe1(t_proc *proc)
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

void	close_pipe(t_proc *proc)
{
	while (proc)
	{
		if (proc->next_pipeline == PIPE)
		{
			close(proc->pipes[0]);
			close(proc->pipes[1]);
		}
		if (proc->from_pipe)
		{
			close(proc->from_pipe[0]);
			close(proc->from_pipe[1]);
		}
		proc = proc->next;
	}
}

void	assign_pipe_2(t_proc *proc)
{
	t_proc	*tmp;

	if (proc->next->type == COMMAND && proc->next->fd_in == STDIN_FILENO)
	{
		proc->next->from_pipe = proc->pipes;
		proc->next->fd_in = proc->pipes[0];
	}
	if (proc->next->type == SUBSHELL)
	{
		tmp = proc->next->procs;
		if (tmp->procs)
		{
			while (tmp->procs)
				tmp = tmp->procs;
		}
		if (tmp->fd_in == STDIN_FILENO)
			tmp->fd_in = proc->pipes[0];
		tmp->from_pipe = proc->pipes;
	}
}

void	assign_pipe(t_proc *proc)
{
	t_proc	*tmp;

	if (proc->next_pipeline == PIPE)
	{
		if (proc->type == COMMAND && proc->fd_out == STDOUT_FILENO)
		{
			proc->fd_out = proc->pipes[1];
		}
		if (proc->type == SUBSHELL)
		{
			tmp = proc->procs;
			while (tmp)
			{
				if (tmp->next_pipeline != PIPE && tmp->fd_out == STDOUT_FILENO)
						tmp->fd_out = proc->pipes[1];
				tmp = tmp->next;
			}
		}
		assign_pipe_2(proc);
	}
}
