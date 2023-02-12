/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 16:14:35 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/12 21:51:20 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// first two readdir are to skip . and ..
DIR		*open_dir(char *path)
{
	DIR		*dir;

	dir = opendir(path);
	if (dir == NULL)
		return (perror("opendir"), NULL);
	(void)readdir(dir);
	(void)readdir(dir);
	return (dir);
}

t_list	*get_lst_of_dir(char *path)
{
	t_list			*files;
	t_list			*tmp;
	struct dirent	*dirent;
	DIR				*dir;

	files = NULL;
	dir = open_dir(path);
	if (!dir)
		return (NULL);
	while (1)
	{
		errno = 0;
		dirent = readdir(dir);
		if (!dirent && errno)
			return (perror("readdir"), NULL);
		if (!dirent)
			break ;
		tmp = ft_lstnew(dirent->d_name);
		if (!tmp)
			return (perror("malloc"), NULL);
		ft_lstadd_back(&files, tmp);
	}
	if (closedir(dir))
		return (perror("closedir"), NULL);
	return (files);
}

char	*get_dir_content(void)
{
	t_list			*files;
	t_list			*tmp;
	int				size;
	char			*str;

	files = get_lst_of_dir(".");
	if (!files)
		return (NULL);
	size = 0;
	tmp = files;
	while (tmp)
	{
		size += ft_strlen(tmp->content);
		tmp = tmp->next;
	}
	str = calloc(sizeof(char), size + 1);
	if (!str)
		return (perror("malloc"), NULL);
	tmp = files;
	while (tmp)
	{
		ft_strlcat(str, tmp->content, size + 1);
		tmp = tmp->next;
	}
	return (str);
}
