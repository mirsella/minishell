/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expantion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:57:59 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/14 15:32:47 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

struct s_chars
{
	char	*str;
	char	*tmp;
	char	*joined;
};

// char	*expand_individual(char *line, int *i, t_list *env)
// {
// 	char	*tmp;
// 
// 	if (*line == '\'')
// 		tmp = expand_single_quote(line, i);
// 	else if (*line == '"')
// 		tmp = expand_double_quote(env, line, i);
// 	else if (*line == '$')
// 		tmp = expand_var(env, line, i);
// 	else
// 	{
// 		tmp = ft_strdup((char [2]){*line, 0});
// 		(*i)++;
// 	}
// 	if (!tmp)
// 		return (perror("malloc"), NULL);
// 	return (tmp);
// }

char	*expand_vars(t_list *env, char *line)
{
	int				i;
	struct s_chars	chars;

	i = 0;
	chars.str = ft_strdup("");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			chars.tmp = ft_strndup(line + i, skip_quotes(line + i));
			i += skip_quotes(line + i);
		}
		else if (line[i] == '$')
			chars.tmp = expand_var(env, line + i, &i);
		else
			chars.tmp = ft_strdup((char []){line[i++], 0});
		if (!chars.tmp)
			return (free(chars.tmp), NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		chars.str = chars.joined;
	}
	return (chars.str);
}

// char	*expand_everything(t_list *env, char *line)
// {
// 	int		i;
// 	char	*str;
// 	char	*tmp;
// 	char	*expanded;
// 
// 	i = 0;
// 	str = ft_strdup("");
// 	if (!str)
// 		return (perror("malloc"), NULL);
// 	while (line[i])
// 	{
// 		expanded = expand_individual(line + i, &i, env);
// 		if (!expanded)
// 			return (free(str), free(line), NULL);
// 		tmp = ft_strjoin_free(str, expanded);
// 		str = tmp;
// 	}
// 	return (str);
// }

char	*expand_everything(t_list *env, char *line)
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
			chars.tmp = expand_double_quote(env, line + i, &i);
		else if (line[i] == '$')
			chars.tmp = expand_var(env, line + i, &i);
		else
			chars.tmp = ft_strdup((char []){line[i++], 0});
		if (!chars.tmp)
			return (free(chars.tmp), NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		chars.str = chars.joined;
	}
	return (chars.str);
}
