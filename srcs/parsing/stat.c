/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 14:16:51 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/24 16:15:45 by mirsella         ###   ########.fr       */
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
		return (print_errorendl(path, "Not a regular file"), 0);
	if (!(st.st_mode & S_IXUSR))
		return (print_errorendl(path, "Not executable"), 0);
	return (1);
}

int	is_file_readable(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (print_errorendl(path, strerror(errno)), 0);
	if (!(st.st_mode & S_IRUSR))
		return (print_errorendl(path, "Read permission denied"), 0);
	return (1);
}

// shouldn't be a directory
int	is_file_writable(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		if (errno == ENOENT)
			return (1);
		else
			return (print_errorendl(path, strerror(errno)), 0);
	}
	if (S_ISDIR(st.st_mode))
		return (print_errorendl(path, "Is a directory"), 0);
	if (!(st.st_mode & S_IWUSR))
		return (print_errorendl(path, "Write permission denied"), 0);
	return (1);
}
