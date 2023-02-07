/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 22:52:18 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/07 23:40:01 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_proc	*new_proc(char *args, t_list *env)
{
	t_proc	*new;

	new = malloc(sizeof(t_proc));
	if (!new)
		return (perror("malloc"), NULL);
	new->args = args;
	new->env = env;
	new->next = NULL;
	return (new);
}

void	add_proc(t_proc *proc, char *args, t_list *env)
{
	t_proc	*tmp;

	tmp = proc;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_proc(args, env);
}

void	procs_free(t_proc **proc)
{
	t_proc	*tmp;

	while (*proc)
	{
		tmp = *proc;
		*proc = (*proc)->next;
		free(tmp->args);
		free(tmp->args);
		free(tmp);
	}
	*proc = NULL;
}
