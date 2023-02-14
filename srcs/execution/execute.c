/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:01:01 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/14 12:21:57 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// dev function
void	print_procs(t_proc *procs, int layer)
{
	t_proc	*tmp;
	t_list	*args;
	char	*next_pipeline;

	tmp = procs;
	while (tmp)
	{
		if (tmp->next_pipeline == PIPE)
			next_pipeline = "PIPE";
		else if (tmp->next_pipeline == OR)
			next_pipeline = "OR";
		else if (tmp->next_pipeline == AND)
			next_pipeline = "AND";
		else
			next_pipeline = "INVALID";
		if (tmp->type == SUBSHELL)
		{
			printf("%*cSUBSHELL: next_pipeline: %s, fd_in: %d, fd_out: %d\n", layer, ' ',
				next_pipeline, tmp->fd_in, tmp->fd_out);
			print_procs(tmp->procs, layer + 4);
		}
		else
		{
			printf("%*cCOMMAND: %s, next_pipeline: %s, fd_in: %d, fd_out: %d,\n", layer, ' ',
				tmp->path, next_pipeline, tmp->fd_in, tmp->fd_out);
			if (tmp->args)
			{
				args = tmp->args;
				while (args)
				{
					printf("%*cARG: %s\n", layer + 2, ' ',
						(char *)args->content);
					args = args->next;
				}
			}
		}
		tmp = tmp->next;
	}
}

int	execute(t_data *data)
{
	print_procs(data->procs, 0);
	return (0);
}
