/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:17:06 by dly               #+#    #+#             */
/*   Updated: 2023/02/27 22:14:27 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	child(t_proc *tmp, t_proc *proc, t_list *env);
void	wait_loop(t_proc *tmp, t_proc *proc, t_list *env);
void	assign_pipe(t_proc *proc);

int	cmd_not_found(t_proc *proc)
{
	if (!proc->path && proc->type == COMMAND)
	{
		if (proc->exit_code == 0)
			proc->exit_code = 127;
		else if (proc->exit_code == -1)
			proc->exit_code = 0;
		return (1);
	}
	return (0);
}

int	get_status_of_last_proc(t_proc *proc)
{
	while (proc->next)
		proc = proc->next;
	return (proc->exit_code);
}

int	process(t_proc *proc, t_list *env)
{
	t_proc	*tmp;

	tmp = proc;
	// if (proc)
	// 	printf("in process type %d line %s\n", proc->type, proc->line);
	// else
	// 	printf("proc is NULL\n");
	while (proc)
	{
		parse_line_to_proc(proc->line, proc, env);
		// printf("parselinetoproc filled proc type %d line %s and path %s\n", proc->type, proc->line, proc->path);
		// if (proc->type == SUBSHELL)
		// 	printf("  procs: %p\n", proc->procs);
		// printf("path : %s  line: %s \n pipe[0]: %d && pipe[1]: %d \n fd_in: %d  && fd_out: %d\n\n"
		// 	,proc->path, proc->line, proc->pipes[0] ,proc->pipes[1], proc->fd_in, proc->fd_out);
		if (!cmd_not_found(proc) && (proc->path || proc->type == SUBSHELL))
		{
			assign_pipe(proc);
			if (proc->type == COMMAND && proc->fd_in != -1)
			{
				// printf("calling child %s\n", proc->line);
				child(tmp, proc, env);
			}
			if (proc->type == SUBSHELL)
			{
				// printf("calling process recursively bc of SUBSHELL %s and procs %p\n", proc->line, proc->procs);
				process(proc->procs, env);
				proc->exit_code = get_status_of_last_proc(proc->procs);
			}
		}
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break ;
		proc = proc->next;
	}
	wait_loop(tmp, proc, env);
	if (proc)
		recursive_and_or(proc, env, 0);
	return (0);
}
