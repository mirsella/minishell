/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 13:42:25 by dly               #+#    #+#             */
/*   Updated: 2023/02/27 18:04:35 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	update_oldpwd(t_list *env)
{
	char	buf[PATH_MAX];

	if (!getcwd(buf, PATH_MAX))
		return (-1);
	if (add_env_var(env, "OLDPWD", buf) == -1)
		return (-1);
	return (0);
}

static int	update_pwd(t_list *env)
{
	char	buf[PATH_MAX];

	if (!getcwd(buf, PATH_MAX))
		return (-1);
	if (add_env_var(env, "PWD", buf) == -1)
		return (-1);
	return (0);
}

static int	go_to_home(t_list *env)
{
	char	*home;

	if (update_oldpwd(env) == -1)
		return (-1);
	home = get_env_var(env, "HOME");
	if (!home)
	{
		g_exit_code = 1;
		return (print_errorendl("cd", "HOME no set"), 1);
	}
	if (chdir(home) == -1)
		return (-1);
	if (update_pwd(env) == -1)
		return (-1);
	return (0);
}

static int	go_to_path(char *path, t_list *env)
{
	char	buf[PATH_MAX];

	if (update_oldpwd(env) == -1)
		return (-1);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		return (perror(path), -1);
	}
	if (!getcwd(buf, PATH_MAX))
		return (-1);
	if (add_env_var(env, "PWD", buf) == -1)
		return (-1);
	return (0);
}

int	builtin_cd(t_proc *proc, t_list *env)
{
	if (!proc->args->next)
		return (go_to_home(env));
	if (proc->args->next->next)
	{
		proc->exit_code = 1;
		return (ft_putstr_fd("cd: too many arguments\n", 2), -1);
	}
	if (go_to_path(proc->args->next->content, env) == -1)
	{
		proc->exit_code = 1;
		return (-1);
	}
	return (0);
}
