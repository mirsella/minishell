/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_full_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 22:45:08 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/14 23:04:05 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_file_executable(char *path)
{
	struct stat	buf;

	if (stat(path, &buf) == -1)
	{
		if (errno == ENOENT)
			return (0);
		return (perror("stat"), -1);
	}
	if (S_ISREG(buf.st_mode) && (buf.st_mode & S_IXUSR))
		return (1);
	return (0);
}

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

	path = get_env_value(env, "PATH");
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
	if (is_file_executable(cmd))
		return (*full_path = ft_strdup(cmd), 0);
	tmp = search_path(env, cmd);
	if (!tmp)
		return (perror("malloc"), -1);
	if (!*tmp)
	{
		free(tmp);
		return (print_error(cmd, "command not found"), 1);
	}
	*full_path = tmp;
	return (0);
}
