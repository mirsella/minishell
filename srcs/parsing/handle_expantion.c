/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expantion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:57:59 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/14 19:38:29 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <ctype.h>

struct s_chars
{
	char	*str;
	char	*tmp;
	char	*joined;
};

char	*expand_one(char *line, t_list *env, int *index)
{
	char	*tmp;

	// printf("expanding one before '%s' %d\n", line, *index);
	if (is_wildcard(line, env))
		tmp = expand_wildcard(line, index, env);
	else if (*line == '\'')
		tmp = expand_single_quote(line, index);
	else if (*line == '"')
		tmp = expand_double_quote(line, index, env);
	else if (*line == '$')
		tmp = expand_var(env, line, index);
	else
	{
		tmp = ft_strdup((char []){*line, 0});
		(*index)++;
	}
	// printf("expanding one after '%s' %d\n", tmp, *index);
	if (!tmp)
		return (NULL);
	return (tmp);
}

char	*expand_wildcard_and_var(char *line, t_list *env, int *index)
{
	char	*tmp;
	int		i;

	i = 0;
	if (is_wildcard(line, env))
		tmp = expand_wildcard(line + *index, index, env);
	else if (*line == '$')
		tmp = expand_var(env, line, index);
	else if (*line == '\'' || *line == '"')
	{
		tmp = ft_strndup(line, skip_quotes(line));
		(*index) += skip_quotes(line);
	}
	else
	{
		while (line[i] && !isspace(i))
			i++;
		tmp = ft_substr(line, 0, i);
		(*index) += i;
	}
	if (!tmp)
		return (NULL);
	return (tmp);
}

char	*expand_everything(char *line, t_list *env)
{
	int				i;
	struct s_chars	chars;

	i = 0;
	chars.str = ft_strdup("");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (line[i])
	{
		chars.tmp = expand_one(line + i, env, &i);
		if (!chars.tmp)
			return (free(chars.str), NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		chars.str = chars.joined;
	}
	return (chars.str);
}

// char	*expand_vars(t_list *env, char *line)
// {
// 	int				i;
// 	struct s_chars	chars;
// 
// 	i = 0;
// 	chars.str = ft_strdup("");
// 	if (!chars.str)
// 		return (perror("malloc"), NULL);
// 	while (line[i])
// 	{
// 		if (line[i] == '\'' || line[i] == '"')
// 		{
// 			chars.tmp = ft_strndup(line + i, skip_quotes(line + i));
// 			i += skip_quotes(line + i);
// 		}
// 		else if (line[i] == '$')
// 			chars.tmp = expand_var(env, line + i, &i);
// 		else
// 			chars.tmp = ft_strdup((char []){line[i++], 0});
// 		if (!chars.tmp)
// 			return (free(chars.tmp), NULL);
// 		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
// 		chars.str = chars.joined;
// 	}
// 	return (chars.str);
// }

// char	*expand_everything(t_list *env, char *line)
// {
// 	int				i;
// 	struct s_chars	chars;
// 
// 	i = 0;
// 	chars.str = ft_strdup("");
// 	if (!chars.str)
// 		return (perror("malloc"), NULL);
// 	while (line[i])
// 	{
// 		if (line[i] == '\'')
// 			chars.tmp = expand_single_quote(line + i, &i);
// 		else if (line[i] == '"')
// 			chars.tmp = expand_double_quote(env, line + i, &i);
// 		else if (line[i] == '$')
// 			chars.tmp = expand_var(env, line + i, &i);
// 		else
// 			chars.tmp = ft_strdup((char []){line[i++], 0});
// 		if (!chars.tmp)
// 			return (free(chars.tmp), NULL);
// 		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
// 		chars.str = chars.joined;
// 	}
// 	return (chars.str);
// }
