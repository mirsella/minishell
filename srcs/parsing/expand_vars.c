/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 21:42:47 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/16 22:39:05 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

struct s_chars
{
	char	*str;
	char	*tmp;
	char	*joined;
};

char	*quote_word(char *word, int	isnotlast)
{
	char	*tmp;

	tmp = calloc(ft_strlen(word) + 3 + isnotlast, sizeof(char));
	if (!tmp)
		return (perror("malloc"), NULL);
	tmp[0] = '\'';
	ft_strlcpy(tmp + 1, word, ft_strlen(word) + 1);
	tmp[ft_strlen(word) + 1] = '\'';
	if (isnotlast)
		tmp[ft_strlen(word) + 2] = ' ';
	return (tmp);
}

char	*expand_var_to_quotes(t_list *env, char *line, int *index)
{
	struct s_chars	chars;
	char			**words;
	int				i;

	i = 0;
	line = expand_var(env, line, index);
	if (!line)
		return (NULL);
	words = ft_split(line, "\t\n\v\f\r ");
	if (!words)
		return (perror("malloc"), NULL);
	chars.str = ft_strdup("");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (words[i])
	{
		chars.tmp = quote_word(words[i], words[i + 1] != NULL);
		if (!chars.tmp)
			return (free(chars.str), NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		chars.str = chars.joined;
		i++;
	}
	ft_free_tab(words);
	return (chars.str);
}

char	*expand_vars(char *line, t_list *env)
{
	int				i;
	struct s_chars	chars;

	i = 0;
	chars.str = ft_strdup("");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (line[i])
	{
		if (line[i] == '$')
			chars.tmp = expand_var_to_quotes(env, line + i, &i);
		else if (line[i] == '\'' || line[i] == '"')
		{
			chars.tmp = ft_substr(line + i, 0, skip_quotes(line + i));
			i += skip_quotes(line + i);
		}
		else
			chars.tmp = ft_strdup((char []){line[i++], 0});
		if (!chars.tmp)
			return (free(chars.str), NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		chars.str = chars.joined;
	}
	return (chars.str);
}


