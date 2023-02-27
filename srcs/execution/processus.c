/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:17:06 by dly               #+#    #+#             */
/*   Updated: 2023/02/27 10:40:07 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>

void	close_pipe1(t_proc *proc);
void	assign_pipe(t_proc *proc);

static void	child(t_proc *tmp, t_proc *proc, t_list *env)
{
	if (proc->next_pipeline == PIPE)
		assign_pipe_cmd(proc);
	if (isbuiltin(proc->path) && (!tmp->next || tmp->next_pipeline == AND
			|| tmp->next_pipeline == OR))
	{
		proc->exit_code = exec_builtin(proc, env);
		g_exit_code = proc->exit_code;
		return ;
	}
//	printf("path : %s  \n pipe[0]: %d && pipe[1]: %d \n fd_in: %d  && fd_out: %d\n\n", proc->path, proc->pipes[0]
//			,proc->pipes[1], proc->fd_in, proc->fd_out);
	printf("forking line %s\n", proc->line);
	proc->pid = fork();
	if (!proc->pid)
	{
		printf("before double dup2 %d %d\n", proc->fd_in, proc->fd_out);
		if (double_dup2(proc->fd_in, proc->fd_out) == -1)
		{
			printf("double_dup2: %s\n", strerror(errno));
			exit(1);
		}
		printf("after double dup2\n");
		if (isbuiltin(proc->path))
		{
			proc->exit_code = exec_builtin(proc, env);
			exit(proc->exit_code);
		}
		// if (tmp)
		// {
		// 	while (tmp->prev)
		// 		tmp = tmp->prev;
		// }
		// close_pipe1(tmp);
		printf("execve line %s\n", proc->line);
		if (!access(proc->path, 0))
			execve(proc->path, ft_lst_to_tab(proc->args), ft_lst_to_tab(env));
		exit(-1);
	}
}

static void	wait_loop(t_proc *tmp, t_proc *proc, t_list *env)
{
	close_pipe(tmp);
	while (tmp)
	{
		if (tmp->type == COMMAND && tmp->path)
			waitpid(tmp->pid, &tmp->exit_code, 0);
		g_exit_code = tmp->exit_code;
		if (tmp->next_pipeline == AND || tmp->next_pipeline == OR)
			break ;
		tmp = tmp->next;
	}
	if (proc)
		recursive_and_or(proc, env, 0);
}

int	cmd_not_found(t_proc *proc)
{
	if (!proc->path && proc->type == COMMAND)
	{
		proc->exit_code = 127;
		g_exit_code = 127;
		return (1);
	}
	return (0);
}

int	process(t_proc *proc, t_list *env)
{
	t_proc	*tmp;

	tmp = proc;
	if (proc)
		printf("process type %d line %s\n", proc->type, proc->line);
	else
		printf("proc is NULL\n");
	while (proc)
	{
		parse_line_to_proc(proc->line, proc, env);
		printf("filled proc type %d line %s and path %s\n", proc->type, proc->line, proc->path);
		if (proc->type == SUBSHELL)
			printf("  procs: %p\n", proc->procs);
		// printf("path : %s  line: %s \n pipe[0]: %d && pipe[1]: %d \n fd_in: %d  && fd_out: %d\n\n"
		// 	,proc->path, proc->line, proc->pipes[0] ,proc->pipes[1], proc->fd_in, proc->fd_out);
		if (!cmd_not_found(proc) && (proc->path || proc->type == SUBSHELL))
		{
			assign_pipe(proc);
			if (proc->type == COMMAND && proc->fd_in != -1)
			{
				printf("calling child %s\n", proc->line);
				child(tmp, proc, env);
			}
			if (proc->type == SUBSHELL)
			{
				assign_pipe_subshell(proc->procs, proc, env);
				printf("calling process recursively bc of SUBSHELL %s and procs %p\n", proc->line, proc->procs);
				process(proc->procs, env);
				break ;
				// if (proc->next_pipeline == AND || proc->next_pipeline == OR)
				// 	return (recursive_and_or(proc, env, 0), 0);
			}
		}
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break ;
		proc = proc->next;
	}
	wait_loop(tmp, proc, env);
	return (0);
}
