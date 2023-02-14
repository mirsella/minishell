/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:44:30 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/14 21:06:08 by mirsella         ###   ########.fr       */
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

int	is_wildcard(char *line, t_list *env)
{
	int		i;
	char	*tmp;

	i = 0;
	while (line[i] && !isspace(line[i]) && line[i] != '*')
	{
		if (line[i] == '\'' || line[i] == '"')
			i += skip_quotes(line + i);
		else if (line[i] == '$')
		{
			tmp = expand_var(env, line + i, &i);
			if (!tmp)
				return (0);
			if (ft_strchr(tmp, '*'))
				return (free(tmp), 1);
			free(tmp);
		}
		else
			i++;
	}
	if (line[i] == '*')
		return (1);
	return (0);
}

// char	*get_pattern(char *line, int *index, t_list *env)
// {
// 	int				i;
// 	struct s_chars	chars;
// 
// 	i = 0;
// 	chars.str = NULL;
// 	while (line[i] && !isspace(line[i]) && !ismeta(line[i]))
// 	{
// 		chars.joined = get_next_token(line + i, &i);
// 		if (!chars.joined)
// 			return (free(chars.str), NULL);
// 		chars.tmp = expand_everything(chars.joined, env);
// 		free(chars.joined);
// 		if (!chars.tmp)
// 			return (free(chars.str), NULL);
// 		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
// 		if (!chars.joined)
// 			return (NULL);
// 		chars.str = chars.joined;
// 	}
// 	*index += i;
// 	return (chars.str);
// }

// char	*get_pattern(char *line, int *index, t_list *env)
// {
// 	struct s_chars	chars;
// 
// 	chars.tmp = get_next_token(line, index);
// 	if (!chars.tmp)
// 		return (NULL);
// 	chars.str = expand_everything(chars.tmp, env);
// 	free(chars.tmp);
// 	if (!chars.str)
// 		return (NULL);
// 	return (chars.str);
// }

char	*get_pattern(char *line, int *index, t_list *env)
{
	int				i;
	struct s_chars	chars;

	i = 0;
	chars.str = ft_strdup("");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (line[i])
	{
		if (line[i] == '\'')
			chars.tmp = expand_single_quote(line + i, &i);
		else if (line[i] == '"')
			chars.tmp = expand_double_quote(line + i, &i, env);
		else if (line[i] == '$')
			chars.tmp = expand_var(env, line + i, &i);
		else
			chars.tmp = ft_strdup((char []){line[i++], 0});
		if (!chars.tmp)
			return (free(chars.tmp), NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		chars.str = chars.joined;
	}
	*index += i;
	return (chars.str);
}

char	*expand_wildcard(char *line, int *index, t_list *env)
{
	char	*str;
	char	*pattern;
	int		i;

	i = 0;
	pattern = get_pattern(line, &i, env);
	if (!pattern)
		return (NULL);
	*index += i;
	str = get_matching_files(pattern);
	if (!str)
		return (free(pattern), NULL);
	free(pattern);

	if (!*str)
		return (free(str), ft_strndup(line, i));
	return (str);
}

// char	*expand_wildcards(char *line, t_list *env)
// {
// 	struct s_chars	chars;
// 	int				i;
// 
// 	i = 0;
// 	chars.str = ft_strdup("");
// 	while (line[i])
// 	{
// 		if (is_wildcard(line + i))
// 			chars.tmp = expand_wildcard(line + i, &i, env);
// 		else if (line[i] == '\'' || line[i] == '"')
// 		{
// 			chars.tmp = ft_strndup(line + i, skip_quotes(line + i));
// 			i += skip_quotes(line + i);
// 		}
// 		else
// 			chars.tmp = ft_strdup((char []){line[i++], '\0'});
// 		if (!chars.tmp)
// 			return (perror("malloc"), NULL);
// 		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
// 		if (!chars.joined)
// 			return (perror("malloc"), NULL);
// 		chars.str = chars.joined;
// 	}
// 	return (chars.str);
// }
