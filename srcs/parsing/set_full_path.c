/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_full_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 22:45:08 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/17 18:05:11 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*test_paths(char **paths, char *cmd)
{
	char	*tmp;
	char	*path;

	while (*paths)
	{
		path = ft_strjoin(*paths, "/");
		if (!path)
			return (perror("malloc"), NULL);
		tmp = ft_strjoin(path, cmd);
		free(path);
		if (is_file_executable(tmp))
			return (tmp);
		free(tmp);
		paths++;
	}
	return (ft_strdup(""));
}

char	*search_path(t_list *env, char *cmd)
{
	char	*path;
	char	**paths;
	char	*tmp;

	path = get_env_var(env, "PATH");
	if (!path)
		return (ft_strdup(""));
	paths = ft_split(path, ":");
	if (!paths)
		return (perror("malloc"), NULL);
	tmp = test_paths(paths, cmd);
	ft_free_tab(paths);
	if (!tmp)
		return (perror("malloc"), NULL);
	return (tmp);
}

int	set_full_path(t_list *env, char *cmd, char **full_path)
{
	char	*tmp;

	if (isbuiltin(cmd))
		return (*full_path = ft_strdup(cmd), 0);
	if (access(cmd, F_OK) == 0)
	{
		if (is_file_executable(cmd))
			return (*full_path = ft_strdup(cmd), 0);
		g_exit_code = 126;
		return (1);
	}
	tmp = search_path(env, cmd);
	if (!tmp)
		return (perror("malloc"), -1);
	if (!*tmp)
	{
		free(tmp);
		g_exit_code = 127;
		return (print_error(cmd, "command not found"), 1);
	}
	*full_path = tmp;
	return (0);
}
