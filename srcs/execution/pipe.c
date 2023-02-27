/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:51:18 by dly               #+#    #+#             */
/*   Updated: 2023/02/27 13:30:33 by mirsella         ###   ########.fr       */
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
		// printf("[0]:%d [1]:%d\n", proc->pipes[0],proc->pipes[0]);
		proc = proc->next;
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

void	assign_pipe_subshell(t_proc *procs, t_proc *proc, t_list *env)
{
	t_proc *tmp;

	(void)procs;
	(void)env;
	if (proc->next_pipeline == PIPE)
	{
		if (proc->next->procs)
			proc->next->procs->fd_in = proc->pipes[0];
		if (proc->next->fd_in == STDIN_FILENO)
			proc->next->fd_in = proc->pipes[0];
		tmp = proc->procs;
		while (tmp)
		{
			if (tmp->next_pipeline != PIPE)
			{
				if (tmp->fd_out == STDOUT_FILENO)
					tmp->fd_out = proc->pipes[1];
			}
			tmp = proc->procs->next;
		}
		// process(procs, env);
	}
}
