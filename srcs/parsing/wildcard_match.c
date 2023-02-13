/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 11:25:45 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/13 22:48:18 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

struct s_chars
{
	char	*tmp;
	char	*str;
	char	*joined;
};

int	ismatching(const char *pattern, const char *string)
{
	if (!*pattern)
		return (!*string);
	if (*pattern == '*')
	{
		while (*pattern == '*')
			pattern++;
		while (*string)
		{
			if (ismatching(pattern, string))
				return (1);
			string++;
		}
		return (ismatching(pattern, string));
	}
	else if (*pattern != *string)
		return (0);
	return (ismatching(pattern + 1, string + 1));
}

char	*get_match(char *pattern, t_list *file)
{
	char	*str;

	str = NULL;
	if (*(char *)file->content == '.' && *pattern != '.')
		return (ft_strdup(""));
	if (!ismatching(pattern, file->content))
		return (ft_strdup(""));
	if (file->next)
	{
		str = ft_strjoin(file->content, " ");
		return (str);
	}
	str = ft_strdup(file->content);
	return (str);
}

char	*get_matching_files(char *pattern)
{
	struct s_chars	chars;
	t_list			*files;
	t_list			*tmp;

	files = get_lst_of_dir(".");
	if (!files)
		return (NULL);
	chars.str = ft_strdup("");
	if (!chars.str)
		return (NULL);
	tmp = files;
	while (tmp)
	{
		chars.tmp = get_match(pattern, tmp);
		if (!chars.tmp)
			return (NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		chars.str = chars.joined;
		tmp = tmp->next;
	}
	ft_lstclear(&files, free);
	if (!chars.str)
		return (NULL);
	return (chars.str);
}
