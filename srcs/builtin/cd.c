/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 13:42:25 by dly               #+#    #+#             */
/*   Updated: 2023/02/28 15:29:55 by lgillard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>

static int	update_oldpwd(t_list *env)
{
	if (add_env_var(env, "OLDPWD", get_env_var(env, "PWD")) == -1)
		return (-1);
	return (0);
}

static int	go_to_home(t_list *env)
{
	char	*home;
	char	buf[PATH_MAX];

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
	if (!getcwd(buf, PATH_MAX))
		return (-1);
	if (add_env_var(env, "PWD", buf) == -1)
		return (-1);
	return (0);
}

static int	set_path_to_oldpwd(char **path, t_list *env)
{
	*path = get_env_var(env, "OLDPWD");
	if (!*path)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return (-1);
	}
	return (0);
}

static int	go_to_path(char *path, t_list *env)
{
	char	buf[PATH_MAX];

	if (ft_strcmp(path, "-") == 0)
	{
		if (set_path_to_oldpwd(&path, env) == -1)
			return (-1);
		ft_putendl_fd(path, 1);
	}
	path = ft_strdup(path);
	if (update_oldpwd(env) == -1)
		return (-1);
	if (!path)
		return (-1);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		return (perror(path), free(path), -1);
	}
	free(path);
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
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	if (go_to_path(proc->args->next->content, env) == -1)
		return (1);
	return (0);
}
