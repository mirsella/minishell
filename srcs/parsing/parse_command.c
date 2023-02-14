/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 17:09:33 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/14 21:04:27 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

struct s_chars {
	char	*str;
	char	*tmp;
	char	*joined;
};

char	*get_next_token(char *line, int *index)
{
	char	*token;
	int		i;

	i = 0;
	while (line[i] && !ft_isspace(line[i]))
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			i += skip_quotes(line + i);
		}
		else
			i++;
	}
	token = ft_substr(line, 0, i);
	if (!token)
		return (perror("malloc"), NULL);
	*index += i;
	return (token);
}

// int	parse_arguments(char *line, t_proc *proc, t_list *env)
// {
// 	struct s_chars	chars;
// 	t_list			*tmp;
// 	int				i;
// 
// 	i = 0;
// 	while (line[i])
// 	{
// 		chars.tmp = get_next_token(line + i, &i);
// 		if (!chars.tmp)
// 			return (-1);
// 		printf("get_next_token: '%s'\n", chars.tmp);
// 		chars.str = expand_everything(chars.tmp, env);
// 		free(chars.tmp);
// 		if (!chars.str)
// 			return (-1);
// 		printf("expand_everything: '%s'\n", chars.str);
// 		tmp = ft_lstnew(chars.str);
// 		if (!tmp)
// 		{
// 			free(chars.str);
// 			return (perror("malloc"), -1);
// 		}
// 		ft_lstadd_back(&proc->args, tmp);
// 		i += ft_skip_spaces(line + i);
// 	}
// 	return (0);
// }

int	add_words_to_lst(char *line, t_proc *proc)
{
	char 	*tmp;
	// char	*expanded;
	t_list	*lst;
	int		i;
	int		j;

	i = 0;
	while (line[i])
	{
		j = 0;
		while (line[i + j] && !isspace(line[i + j]))
			j++;
		// printf("i=%d, j=%d, line: %s\n", i, j, line);
		tmp = ft_substr(line, i, j);
		if (!tmp)
			return (perror("malloc"), -1);
		// printf("expanding word '%s'\n", tmp);
		// expanded = expand_everything(tmp, env);
		// free (tmp);
		// if (!expanded)
		// 	return (perror("malloc"), -1);
		// printf("expanded '%s'\n", expanded);
		// lst = ft_lstnew(expanded);
		lst = ft_lstnew(tmp);
		if (!lst)
			return (perror("malloc"), -1);
		ft_lstadd_back(&proc->args, lst);
		i += j;
		i += ft_skip_spaces(line + i);
	}
	return (0);
}

int	add_word_to_lst(char *line, t_proc *proc, t_list *env)
{
	t_list	*tmp;
	char	*expanded;

	// printf("word '%s'\n", line);
	expanded = expand_everything(line, env);
	// printf("expanded word '%s'\n", expanded);
	if (!expanded)
		return (-1);
	tmp = ft_lstnew(expanded);
	if (!tmp)
		return (perror("malloc"), -1);
	ft_lstadd_back(&proc->args, tmp);
	return (0);
}

int	parse_arguments(char *line, t_proc *proc, t_list *env)
{
	char	*tmp;
	char	*expanded;
	int		i;

	while (*line)
	{
		// printf("checking word '%s'\n", line);
		i = 0;
		// if expand wildcard or var, can split into multiples args
		// if quotes, single args
		if (is_wildcard(line, env) && *line == '$')
		{
			tmp = expand_one(line, env, &i);
			if (!tmp)
				return (-1);
			// printf("addings words '%s'\n", line);
			if (add_words_to_lst(tmp, proc))
				return (free(tmp), -1);
		}
		else if (*line == '\'' || *line == '"')
		{
			tmp = get_next_token(line, &i);
			// printf("adding word '%s'\n", line);
			if (add_word_to_lst(tmp, proc, env))
				return (free(tmp), -1);
		}
		else
		{
			while (line[i] && !isspace(line[i]))
				i++;
			tmp = ft_substr(line, 0, i);
			if (!tmp)
				return (perror("malloc"), -1);
			expanded = expand_everything(tmp, env);
			free(tmp);
			if (!expanded)
				return (perror("malloc"), -1);
			// printf("adding word '%s' was '%s' passed %d\n", expanded, line, i);
			add_words_to_lst(expanded, proc);
			tmp = expanded;
		}
		free(tmp);
		line += i;
		line += ft_skip_spaces(line);
	}
	return (0);
}

int	parse_command(t_data *data, char *line, t_proc *proc)
{
	int		ret;
	char	*cmd;
	char	*full_path;

	ret = parse_arguments(line, proc, data->env);
	if (ret)
		return (ret);
	cmd = ft_strdup(proc->args->content);
	if (!cmd)
		return (perror("malloc"), -1);
	// printf("cmd: '%s'\n", cmd);
	full_path = NULL;
	ret = set_full_path(data->env, cmd, &full_path);
	free(cmd);
	if (ret)
		return (free(full_path), ret);
	// printf("full_path: '%s'\n", full_path);
	proc->path = full_path;
	return (0);
}


// int	parse_arguments(t_list *env, char *line, t_proc *proc)
// {
// 	t_list			*tmp;
// 	struct s_chars	chars;
// 	int				i;
// 
// 	i = 0;
// 	while (line[i])
// 	{
// 		chars.tmp = get_next_token(line + i, &i);
// 		if (!chars.tmp)
// 			return (-1);
// 		chars.str = expand_everything(env, chars.tmp);
// 		free(chars.tmp);
// 		if (!chars.str)
// 			return (-1);
// 		tmp = ft_lstnew(chars.str);
// 		if (!tmp)
// 		{
// 			free(chars.str);
// 			return (perror("malloc"), -1);
// 		}
// 		ft_lstadd_back(&proc->args, tmp);
// 		i += ft_skip_spaces(line + i);
// 	}
// 	return (0);
// }
// 
// int	parse_command(t_data *data, char *line, t_proc *proc)
// {
// 	struct s_chars	chars;
// 	int				i;
// 	int				ret;
// 
// 	chars.tmp = expand_vars(data->env, line);
// 	if (!chars.tmp)
// 		return (-1);
// 	chars.str = expand_wildcards(chars.tmp, data->env);
// 	free(chars.tmp);
// 	if (!chars.str)
// 		return (-1);
// 	i = ft_skip_spaces(chars.str);
// 	proc->path = get_next_token(chars.str, &i);
// 	if (!proc->path)
// 		return (free(chars.str), -1);
// 	ret = set_full_path(data->env, &proc->path);
// 	if (ret)
// 		return (free(chars.str), ret);
// 	i = ft_skip_spaces(chars.str);
// 	ret = parse_arguments(data->env, chars.str + i, proc);
// 	free(chars.str);
// 	if (ret)
// 		return (ret);
// 	return (0);
// }

