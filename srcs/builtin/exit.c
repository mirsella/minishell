/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 17:38:04 by dly               #+#    #+#             */
/*   Updated: 2023/02/24 16:47:57 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_is_valid(const char *str)
{
	size_t	i;

	i = 0;
	i += ft_skip_spaces((char *)str);
	while (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (ft_isdigit(str[i]))
		i++;
	return (i == ft_strlen(str));
}

int	builtin_exit(t_proc *proc, t_list *env)
{
	unsigned char exit_code;

	exit_code = 0;
	ft_putstr_fd("exit\n", 2);
	if (proc->args->next)
	{
		if (!ft_is_valid(proc->args->next->content))
		{
			print_error(proc->path, proc->args->next->content);
			ft_putendl_fd(": numeric argument required", 2);
			procs_free(&proc);
			free_shell_data(env);
			exit(2);
		}
		if (proc->args->next->next)
		{
			print_errorendl(proc->path, "too many arguments");
			return (1);
		}
		exit_code += ft_atoi(proc->args->next->content);
	}
	free_shell_data(env);
	procs_free(&proc);
	exit(exit_code);
	return (0);
}
