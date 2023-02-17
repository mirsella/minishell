/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 14:16:51 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/17 16:01:32 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <string.h>
#include <unistd.h>

int	is_file_executable(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	if (!S_ISREG(st.st_mode))
		return (print_error(path, "not a regular file"), 0);
	if (!(st.st_mode & S_IXUSR))
		return (print_error(path, "not executable"), 0);
	return (1);
}

int	is_file_readable(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (print_error(path, strerror(errno)), 0);
	if (!S_ISREG(st.st_mode))
		return (print_error(path, "not a regular file"), 0);
	if (!(st.st_mode & S_IRUSR))
		return (print_error(path, "read permission denied"), 0);
	return (1);
}

int	is_file_writable(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		if (errno == ENOENT)
			return (1);
		else
			return (print_error(path, strerror(errno)), 0);
	}
	if (!S_ISREG(st.st_mode))
		return (print_error(path, "not a regular file"), 0);
	if (!(st.st_mode & S_IWUSR))
		return (print_error(path, "write permission denied"), 0);
	return (1);
}
