/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:17:06 by dly               #+#    #+#             */
/*   Updated: 2023/02/21 20:0 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "sys/wait.h" // waitpid

static void	child(t_proc *tmp, t_proc *proc, t_list *env)
{
	if (proc->next_pipeline == PIPE)
		assign_pipe_cmd(proc);
	proc->pid = fork();
	if (!proc->pid)
	{
		if (isbuiltin(proc->line))
			exit(0);
		if (double_dup2(proc->fd_in, proc->fd_out) == -1)
			exit(1);
		close_pipe(tmp);
		if (!access(proc->path, 0))
			execve(proc->path, ft_lst_to_tab(proc->args), ft_lst_to_tab(env));
		exit(-1);
	}
}

static void	wait_loop(t_proc *proc)
{
	close_pipe(proc);
	while (proc)
	{
		if (proc->type == COMMAND)
			waitpid(proc->pid, &proc->exit_code, 0);
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break ;
		proc = proc->next;
	}
}

static int	recursive_and_or(t_proc *tmp, t_proc *proc, t_list *env)
{
	if (proc->next_pipeline == AND)
	{
		if (proc->exit_code != 0)
			return (0);
		// while (tmp->next)
		// {
		// 	if (tmp->exit_code != 0)
		// 		return (0);
		// 	if (tmp->next_pipeline == AND || tmp->next_pipeline == OR)
		// 		break ;
		// 	tmp = tmp->next;
		// }
	}
	if (proc->next_pipeline == OR)
	{
		while (tmp)
		{
			if (tmp->exit_code == 0)
				return (0);
			if (tmp->next_pipeline == AND || tmp->next_pipeline == OR)
				break ;
			tmp = tmp->next;
		}
	}
	process(proc->next, env);
	return (0);
}

int	process(t_proc *proc, t_list *env)
{
	t_proc	*tmp;

	tmp = proc;
	while (proc)
	{
		if (proc->type == SUBSHELL)
		{
			assign_pipe_subshell(proc->procs, proc, env);
			process(proc->procs, env);
			proc = proc->next;
			if (!proc)
				return (0);
		}
		if (proc->type == COMMAND)
			child(tmp, proc, env);
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break ;
		proc = proc->next;
	}
	wait_loop(tmp);
	if (proc)
		recursive_and_or(tmp, proc, env);
	return (0);
}