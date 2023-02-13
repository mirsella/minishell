/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir_content.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 16:14:35 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/13 11:26:23 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// first two readdir are to skip . and ..
DIR	*open_dir(char *path)
{
	DIR		*dir;

	dir = opendir(path);
	if (dir == NULL)
		return (perror("opendir"), NULL);
	(void)readdir(dir);
	(void)readdir(dir);
	return (dir);
}

t_list	*ft_lstadd_back_dup(t_list **lst, char *content)
{
	t_list	*new;
	char	*dup;

	dup = ft_strdup(content);
	if (!dup)
		return (perror("malloc"), NULL);
	new = ft_lstnew(dup);
	if (!new)
		return (perror("malloc"), NULL);
	if (!*lst)
		*lst = new;
	else
		ft_lstlast(*lst)->next = new;
	return (new);
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
		tmp = ft_lstadd_back_dup(&files, dirent->d_name);
		if (!tmp)
			return (perror("malloc"), NULL);
	}
	if (closedir(dir))
		return (perror("closedir"), NULL);
	ft_lstsort(&files, strcmp);
	return (files);
}
