/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_full_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 22:45:08 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/28 14:46:17 by lgillard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <string.h>

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
		if (is_file_executable(tmp, 0))
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
		if (ft_strcmp(cmd, "..") == 0 || ft_strcmp(cmd, ".") == 0)
			return (print_errorendl(cmd, "command not found"));
		if (is_file_executable(cmd, 1))
			return (*full_path = ft_strdup(cmd), 0);
		return (126);
	}
	tmp = search_path(env, cmd);
	if (!tmp)
		return (perror("malloc"), -1);
	if (!*tmp)
	{
		free(tmp);
		return (print_errorendl(cmd, "command not found"), 127);
	}
	*full_path = tmp;
	return (0);
}
