/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstproc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 22:52:18 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/27 22:40:28 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

int	create_and_push_proc(t_proc **first, t_proc **last_proc, t_proc **proc)
{
	t_proc	*new;

	new = new_proc();
	if (!new)
		return (-1);
	new->prev = *last_proc;
	*proc = new;
	if (!*first)
		*first = new;
	if (*last_proc && (*last_proc)->type == COMMAND)
		(*last_proc)->next = new;
	else if (*last_proc
		&& (*last_proc)->type == SUBSHELL && (*last_proc)->procs)
		(*last_proc)->next = new;
	else if (*last_proc
		&& (*last_proc)->type == SUBSHELL && !(*last_proc)->procs)
		(*last_proc)->procs = new;
	*last_proc = new;
	return (0);
}

t_proc	*new_proc(void)
{
	t_proc	*new;

	new = calloc(sizeof(t_proc), 1);
	if (!new)
		return (perror("malloc"), NULL);
	new->type = COMMAND;
	new->fd_in = STDIN_FILENO;
	new->fd_out = STDOUT_FILENO;
	new->pipes[0] = -1;
	new->pipes[1] = -1;
	new->from_pipe = NULL;
	new->next = NULL;
	new->prev = NULL;
	new->args = NULL;
	new->path = NULL;
	new->line = NULL;
	new->procs = NULL;
	new->pid = -1;
	new->exit_code = -1;
	return (new);
}

void	proc_free(t_proc *proc)
{
	if (proc->line)
		free(proc->line);
	if (proc->args)
		ft_lstclear(&proc->args, free);
	if (proc->path)
		free(proc->path);
	if (proc->fd_in > 2)
		close(proc->fd_in);
	if (proc->fd_out > 2)
		close(proc->fd_out);
	if (proc->pipes[0] > 2)
		close(proc->pipes[0]);
	if (proc->pipes[1] > 2)
		close(proc->pipes[1]);
	free(proc);
}

int	get_status_of_last_proc(t_proc *proc)
{
	while (proc->next)
		proc = proc->next;
	return (proc->exit_code);
}

void	procs_free(t_proc **proc)
{
	t_proc	*tmp;

	if (!proc || !*proc)
		return ;
	while ((*proc)->prev)
		*proc = (*proc)->prev;
	while (*proc)
	{
		tmp = *proc;
		*proc = (*proc)->next;
		if (tmp->procs)
		{
			tmp->procs->prev = NULL;
			procs_free(&tmp->procs);
		}
		proc_free(tmp);
	}
	*proc = NULL;
}
