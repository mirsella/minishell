/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 17:38:04 by dly               #+#    #+#             */
/*   Updated: 2023/02/23 18:28:56 by dly              ###   ########.fr       */
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
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(proc->path, 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(proc->args->next->content, 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd("numeric argument required\n", 2);
			exit(2);
		}
		if (proc->args->next->next)
		{
			print_error(proc->path, "too many arguments");
			exit(1);
		}
		proc->exit_code = ft_atoi(proc->args->next->content);
	}
	free_shell_data(env);
	// procs_free(&proc);
	exit(proc->exit_code);
	return (0);
}
