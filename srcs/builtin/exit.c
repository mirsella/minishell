/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 17:38:04 by dly               #+#    #+#             */
/*   Updated: 2023/02/28 16:53:24 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// check if number at str fits in long long
int	check_if_numeric(char *str, int neg)
{
	int	i;

	i = 0;
	while (ft_isdigit(str[i]))
		i++;
	if (i > 19)
		return (0);
	if (i == 19)
	{
		if (neg % 2 == 0)
		{
			if (ft_strcmp(str, "9223372036854775807") > 0)
				return (0);
		}
		else
		{
			if (ft_strcmp(str, "9223372036854775808") > 0)
				return (0);
		}
	}
	return (1);
}

static int	ft_is_valid(const char *str)
{
	size_t	i;
	int		neg;

	i = 0;
	i += ft_skip_spaces((char *)str);
	neg = 0;
	while (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg++;
		i++;
	}
	if (!check_if_numeric((char *)str + i, neg))
		return (0);
	if (!str[i])
		return (0);
	while (ft_isdigit(str[i]))
		i++;
	return (i == ft_strlen(str));
}

int	builtin_exit(t_proc *proc, t_list *env)
{
	unsigned char	exit_code;

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
		exit_code = ft_atoll(proc->args->next->content);
	}
	free_shell_data(env);
	procs_free(&proc);
	exit(exit_code);
	return (0);
}
