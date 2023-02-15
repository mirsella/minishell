/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstproc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 22:52:18 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/15 19:33:38 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

int	create_and_push_proc(t_proc **last_proc, t_proc **proc)
{
	t_proc	*new;

	new = new_proc();
	if (!new)
		return (-1);
	if (!*last_proc)
		*last_proc = new;
	else
	{
		(*last_proc)->next = new;
		*last_proc = new;
	}
	// else if ((*last_proc)->type == COMMAND
	// 	|| ((*last_proc)->type == SUBSHELL && (*last_proc)->procs))
	// 	(*last_proc)->next = new;
	// else if ((*last_proc)->type == SUBSHELL && !(*last_proc)->procs)
	// 	(*last_proc)->procs = new;
	*proc = new;
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
	new->next = NULL;
	new->args = NULL;
	new->pid = -1;
	new->exit_code = -1;
	return (new);
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
		if (tmp->fd_in > 2)
			close(tmp->fd_in);
		if (tmp->fd_out > 2)
			close(tmp->fd_out);
		free(tmp);
	}
	*proc = NULL;
}
