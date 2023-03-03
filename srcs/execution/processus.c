/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:17:06 by dly               #+#    #+#             */
/*   Updated: 2023/03/03 13:44:52 by lgillard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	fill_procs(t_proc *proc, t_list *env)
{
	int		ret;

	ret = 0;
	while (proc)
	{
		ret = parse_line_to_proc(proc->line, proc, env);
		if (ret)
			return (ret);
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break ;
		proc = proc->next;
	}
	return (ret);
}

void	handle_proc(t_proc *proc, t_proc *tmp, t_list *env)
{
	if (proc->fd_in == -1 || proc->fd_out == -1)
		return ;
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
}

int	process(t_proc *proc, t_list *env)
{
	t_proc	*tmp;
	int		ret;

	tmp = proc;
	ret = fill_procs(proc, env);
	if (ret)
		return (ret);
	while (proc)
	{
		handle_proc(proc, tmp, env);
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break ;
		proc = proc->next;
	}
	wait_loop(tmp);
	if (proc)
		recursive_and_or(proc, env, 0);
	return (0);
}
