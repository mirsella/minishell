/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 17:55:10 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/20 23:20:51 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_unset(t_proc *proc, t_list *env)
{
	t_list	*tmp;

	tmp = proc->args;
	while (tmp)
	{
		remove_env_var(env, tmp->content);
		tmp = tmp->next;
	}
	return (0);
}
