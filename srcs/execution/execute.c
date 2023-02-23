/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:01:01 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/24 00:12:19 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// dev function
int	print_procs(t_proc *procs, t_list *env, int layer)
{
	t_proc	*tmp;
	t_list	*args;
	// char	*next_pipeline;
	int		ret;

	tmp = procs;
	while (tmp)
	{
		// if (tmp->next_pipeline == PIPE)
		// 	next_pipeline = "PIPE";
		// else if (tmp->next_pipeline == OR)
		// 	next_pipeline = "OR";
		// else if (tmp->next_pipeline == AND)
		// 	next_pipeline = "AND";
		// else
		// 	next_pipeline = "INVALID";
		ret = parse_line_to_proc(tmp->line, tmp, env);
		if (ret)
			return (ret);
		if (tmp->type == SUBSHELL)
		{
			// printf("%*cSUBSHELL: next_pipeline: %s, fd_in: %d, fd_out: %d\n", layer, ' ',
				// next_pipeline, tmp->fd_in, tmp->fd_out);
			ret = print_procs(tmp->procs, env, layer + 4);
			if (ret)
				return (ret);
		}
		else
		{
			// printf("%*cCOMMAND: '%s', next_pipeline: %s, fd_in: %d, fd_out: %d,\n", layer, ' ',
				// tmp->path, next_pipeline, tmp->fd_in, tmp->fd_out);
			if (tmp->args)
			{
				args = tmp->args;
				while (args)
				{
					// printf("%*cARG: '%s'\n", layer + 2, ' ',
						// (char *)args->content);
					args = args->next;
				}
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

int	execute(t_proc *procs, t_list *env)
{
	print_procs(procs, env, 0);
	if (open_pipe(procs))
		return (-1);
	printf("before process\n");
	process(procs, env);
	printf("after process\n");
	return (0);
}
