/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstproc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 22:52:18 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/13 18:12:54 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

t_proc	*create_and_push_proc(t_data *data, t_proc *last_proc)
{
	t_proc	*new;

	new = new_proc();
	if (!new)
		return (NULL);
	if (!last_proc)
		data->procs = new;
	else if (last_proc->type == COMMAND
		|| (last_proc->type == SUBSHELL && last_proc->procs))
		last_proc->next = new;
	else if (last_proc->type == SUBSHELL && !last_proc->procs)
		last_proc->procs = new;
	return (new);
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
	new->next = NULL;
	new->args = NULL;
	new->procs = NULL;
	new->pid = -1;
	new->exit_status = -1;
	return (new);
}

t_proc	*get_last_proc(t_proc *procs)
{
	t_proc	*tmp;

	tmp = procs;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

// useless ?
void	push_back_proc(t_proc *procs, t_proc *proc)
{
	t_proc	*tmp;

	tmp = procs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = proc;
}

void	procs_free(t_proc **proc)
{
	t_proc	*tmp;

	if (!proc || !*proc)
		return ;
	while (*proc)
	{
		tmp = *proc;
		*proc = (*proc)->next;
		if (tmp->args)
			ft_lstclear(&tmp->args, free);
		if (tmp->path)
			free(tmp->path);
		if (tmp->procs)
			procs_free(&tmp->procs);
		if (tmp->fd_in > 2)
			close(tmp->fd_in);
		if (tmp->fd_out > 2)
			close(tmp->fd_out);
		free(tmp);
	}
	*proc = NULL;
}
