/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:17:06 by dly               #+#    #+#             */
/*   Updated: 2023/02/28 11:18:54 by lgillard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

int	cmd_not_found(t_proc *proc)
{
	char	buffer[1024];
	int		ret;

	if (!proc->path && proc->type == COMMAND)
	{
		if (proc->exit_code == 0)
			proc->exit_code = 127;
		else if (proc->exit_code == -1)
			proc->exit_code = 0;
		if (proc->fd_in != STDIN_FILENO && proc->fd_out != STDOUT_FILENO)
		{
			ret = 1;
			while (ret)
			{
				ret = read(proc->fd_in, &buffer, 1024);
				write(proc->fd_out, &buffer, ret);
			}
		}
		return (1);
	}
	return (0);
}

int	process(t_proc *proc, t_list *env)
{
	t_proc	*tmp;

	tmp = proc;
	while (proc)
	{
		parse_line_to_proc(proc->line, proc, env);
		if (!cmd_not_found(proc) && (proc->path || proc->type == SUBSHELL))
		{
			assign_pipe(proc);
			if (proc->type == COMMAND && proc->fd_in != -1)
				child(tmp, proc, env);
			if (proc->type == SUBSHELL)
			{
				process(proc->procs, env);
				proc->exit_code = get_status_of_last_proc(proc->procs);
			}
		}
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break ;
		proc = proc->next;
	}
	wait_loop(tmp);
	if (proc)
		recursive_and_or(proc, env, 0);
	return (0);
}
