/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:44:30 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/12 23:56:45 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>

struct s_chars
{
	char	*tmp;
	char	*str;
	char	*joined;
};

int	skip_wildcard(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] == '*')
		i++;
	return (i);
}

char	*expand_wildcards(char *line)
{
	char			*dircontent;
	struct s_chars	chars;
	int				i;

	// handle *.c things like that
	dircontent = get_dir_content();
	if (!dircontent)
		return (NULL);
	i = 0;
	chars.str = NULL;
	while (line[i])
	{
		if (line[i] == '*')
		{
			i += skip_wildcard(line + i);
			chars.tmp = ft_strdup(dircontent);
		}
		else
			chars.tmp = ft_strdup((char[]){line[i++], '\0'});
		if (!chars.tmp)
			return (perror("malloc"), NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		chars.str = chars.joined;
	}
	return (free(dircontent), chars.str);
}
