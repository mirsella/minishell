/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 22:56:25 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/20 23:19:08 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	isbuiltin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	else
		return (0);
}

// int	exec_builtin(t_proc *proc)
// {
// 	if (ft_strcmp(proc->path, "echo") == 0)
// 		return (builtin_echo(proc));
// 	else if (ft_strcmp(proc->path, "cd") == 0)
// 		return (builtin_cd(proc));
// 	else if (ft_strcmp(proc->path, "pwd") == 0)
// 		return (builtin_pwd(proc));
// 	else if (ft_strcmp(proc->path, "export") == 0)
// 		return (builtin_export(proc));
// 	else if (ft_strcmp(proc->path, "unset") == 0)
// 		return (builtin_unset(proc, env));
// 	else if (ft_strcmp(proc->path, "env") == 0)
// 		return (builtin_env(proc, env));
// 	else if (ft_strcmp(proc->path, "exit") == 0)
// 		return (builtin_exit(proc));
// 	else
// 		return (0);
// }
