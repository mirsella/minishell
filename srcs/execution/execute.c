/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:01:01 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/27 18:59:37 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_and_exit_child(t_proc *proc, t_list *env, int exit_code)
{
	ft_lstclear(&env, free);
	procs_free(&proc);
	exit(exit_code);
}

int	execute(t_proc *procs, t_list *env)
{
	if (open_pipe(procs))
		return (-1);
	signal(SIGQUIT, SIG_DFL);
	process(procs, env);
	return (0);
}
