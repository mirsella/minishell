/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 22:52:18 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/10 16:01:57 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
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
	return (new);
}

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

	while (*proc)
	{
		tmp = *proc;
		*proc = (*proc)->next;
		if (tmp->args)
			free(tmp->args);
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
