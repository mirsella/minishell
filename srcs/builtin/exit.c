/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 17:38:04 by dly               #+#    #+#             */
/*   Updated: 2023/02/24 16:13:16 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_is_valid(const char *str)
{
	int			i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_proc *proc, t_list *env)
{
	if (proc->args->next)
	{
		if (!ft_is_valid(proc->args->next->content))
		{
			print_error(proc->path, proc->args->next->content);
			ft_putendl_fd(": numeric argument required", 2);
			exit(2);
		}
		if (proc->args->next->next)
		{
			print_errorendl(proc->path, "too many arguments");
			exit(1);
		}
		proc->exit_code = ft_atoi(proc->args->next->content);
	}
	free_shell_data(env);
	procs_free(&proc);
	exit(proc->exit_code);
	return (0);
}
