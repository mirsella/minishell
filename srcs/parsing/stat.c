/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 14:16:51 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/27 17:30:42 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <string.h>
#include <unistd.h>

char	*get_filetype(mode_t mode)
{
	if (S_ISREG(mode))
		return ("is a regular file");
	else if (S_ISDIR(mode))
		return ("is a directory");
	else if (S_ISCHR(mode))
		return ("is a character device");
	else if (S_ISBLK(mode))
		return ("is a block device");
	else if (S_ISFIFO(mode))
		return ("is a FIFO");
	else if (S_ISLNK(mode))
		return ("is a symbolic link");
	else if (S_ISSOCK(mode))
		return ("is a socket");
	else
		return ("is unknown");
}

int	is_file_executable(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	if (!S_ISREG(st.st_mode))
		return (print_errorendl(path, get_filetype(st.st_mode)), 0);
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
