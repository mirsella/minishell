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

static int recursive_and_or(t_proc *tmp, t_proc *proc, t_list *env)
{
	// (void)tmp;
	// printf("%d\n", proc->exit_code);
	t_proc *test;
	test = proc;
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
		if (proc->exit_code == 0)
			return (0);
		// while (tmp)
		// {
		// 	if (tmp->exit_code == 0)
		// 		return (0);
		// 	if (tmp->next_pipeline == AND || tmp->next_pipeline == OR)
		// 		break;
		// 	tmp = tmp->next;
		// }
	}
	while (tmp)
	{
		parse_line_to_proc(tmp->line, tmp, env);
		tmp = tmp->next;
	}
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
					process(proc->next, env);
					return (0);
					// break ;
				}
				proc = proc->next;
				if (!proc)
					return (0);
			}
			if (proc->type == COMMAND)
			{
				child(tmp, proc, env);
			}
		}
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break;
		proc = proc->next;
	}
	wait_loop(tmp);
	if (proc)
	{
		recursive_and_or(tmp, proc, env);
	}
	return (0);
}

/*	1. ! is at the moment assigned as a command, but it shouldn't
	2. > < >> << if failed, need to exit/stop exec and g_code_exit = 2
	3. \ need to ignore the env_var. ex: echo \$HOME     output = $HOME instead of /homes/......
	4. echo in /dev/full , no enough space to write in, check return of write()
	5. export david=d && echo $david	need to reparse after each && or || to get the env_var

 */