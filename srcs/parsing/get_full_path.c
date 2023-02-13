/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_full_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 22:45:08 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/13 22:50:48 by mirsella         ###   ########.fr       */
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

char	*search_PATH(t_list *env, char *cmd)
{
	char	*PATH;
	char	**paths;
	char	*tmp;

	PATH = get_env_value(env, "PATH");
	if (!PATH)
		return (ft_strdup(""));
	paths = ft_split(PATH, ":");
	if (!paths)
		return (perror("malloc"), NULL);
	tmp = test_paths(paths, cmd);
	ft_free_tab(paths);
	if (!tmp)
		return (perror("malloc"), NULL);
	return (tmp);
}

int	get_full_path(t_list *env, char **cmd)
{
	char	*tmp;

	if (isbuiltin(*cmd))
		return (0);
	if (is_file_executable(*cmd))
		return (0);
	tmp = search_PATH(env, *cmd);
	if (!tmp)
		return (perror("malloc"), -1);
	if (!*tmp)
	{
		free(tmp);
		return (print_error(*cmd, "command not found"), 1);
	}
	free(*cmd);
	*cmd = tmp;
	return (0);
}
