/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:17:06 by dly               #+#    #+#             */
/*   Updated: 2023/02/24 19:52:30 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void child(t_proc *tmp, t_proc *proc, t_list *env)
{
	if (proc->next_pipeline == PIPE)
		assign_pipe_cmd(proc);
	if (isbuiltin(proc->path) && (!tmp->next || tmp->next_pipeline == AND || tmp->next_pipeline == OR))
	{
		proc->exit_code = exec_builtin(proc, env);
		g_exit_code = proc->exit_code;
		return ;
	}
	proc->pid = fork();
	if (!proc->pid)
	{
		if (double_dup2(proc->fd_in, proc->fd_out) == -1)
			exit(1);
		if (isbuiltin(proc->path))
		{
			proc->exit_code = exec_builtin(proc, env);
			exit(proc->exit_code);
		}
		close_pipe(tmp);
		if (!access(proc->path, 0))
			execve(proc->path, ft_lst_to_tab(proc->args), ft_lst_to_tab(env));
		exit(-1);
	}
}

static void wait_loop(t_proc *proc)
{
	close_pipe(proc);
	while (proc)
	{
		if (proc->type == COMMAND && proc->path)
			waitpid(proc->pid, &proc->exit_code, 0);
		g_exit_code = proc->exit_code;
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break;
		proc = proc->next;
	}
}

static int	is_true(t_proc *proc ,int is_subshell)
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

static int recursive_and_or(t_proc *proc, t_list *env, int need_open)
{
	t_proc *tmp;
	int		is_subshell;

	tmp = proc;
	is_subshell = 2;
	if (proc->prev)
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

int process(t_proc *proc, t_list *env)
{
	t_proc *tmp;

	tmp = proc;
	while (proc)
	{
		parse_line_to_proc(proc->line, proc, env);
		if (!proc->path && proc->type == COMMAND)
		{
			proc->exit_code = 127;
			g_exit_code = 127;
		}
		else if (proc->path || proc->type == SUBSHELL)
		{
			if (proc->type == SUBSHELL)
			{
				assign_pipe_subshell(proc->procs, proc, env);
				process(proc->procs, env);
				if (proc->next_pipeline == AND || proc->next_pipeline == OR)
				{
					recursive_and_or(proc, env, 0);
					return (0);
				}
				proc = proc->next;
				if (!proc)
					return (0);
			}
			if (proc->type == COMMAND && proc->fd_in != -1 && proc->fd_out != -1)
				child(tmp, proc, env);
		}
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break;
		proc = proc->next;
	}
	wait_loop(tmp);
	if (proc)
		recursive_and_or(proc, env, 1);
	return (0);
}
