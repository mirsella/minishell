/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:01:01 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/24 11:09:53 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute(t_proc *procs, t_list *env)
{
	if (open_pipe(procs))
		return (-1);
	process(procs, env);
	return (0);
}
