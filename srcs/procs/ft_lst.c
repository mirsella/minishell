/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 22:52:18 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/08 19:20:36 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_proc	*new_proc_command(char *args, char *path,
		t_next_pipeline next_pipeline, int pipes[2])
{
	t_proc	*new;

	new = calloc(sizeof(t_proc), 1);
	if (!new)
		return (perror("malloc"), NULL);
	new->args = args;
	new->path = path;
	new->next_pipeline = next_pipeline;
	new->pipes[0] = pipes[0];
	new->pipes[1] = pipes[1];
	new->next = NULL;
	return (new);
}

t_proc	*new_proc_subshell(t_proc *procs,
		t_next_pipeline next_pipeline, int pipes[2])
{
	t_proc	*new;

	new = calloc(sizeof(t_proc), 1);
	if (!new)
		return (perror("malloc"), NULL);
	new->procs = procs;
	new->next_pipeline = next_pipeline;
	new->pipes[0] = pipes[0];
	new->pipes[1] = pipes[1];
	new->next = NULL;
	return (new);
}

void	push_back_proc(t_proc *procs, t_proc *new)
{
	t_proc	*tmp;

	tmp = procs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
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
		free(tmp);
	}
	*proc = NULL;
}
