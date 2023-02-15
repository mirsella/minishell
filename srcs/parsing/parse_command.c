/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 17:09:33 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/15 18:17:04 by mirsella         ###   ########.fr       */
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

int	add_word_to_lst(char *line, t_proc *proc, t_list *env)
{
	t_list	*tmp;
	char	*expanded;

	expanded = expand_everything(line, env);
	if (!expanded)
		return (-1);
	tmp = ft_lstnew(expanded);
	if (!tmp)
		return (perror("malloc"), -1);
	ft_lstadd_back(&proc->args, tmp);
	return (0);
}

int	add_next_token_to_lst_as_tokens(
		char *line, t_list **args, t_list *env, int *index)
{
	struct s_chars	chars;
	t_list			*tmplst;
	int				i;

	i = 0;
	chars.tmp = get_next_token(line, index);
	if (!chars.tmp)
		return (perror("malloc"), -1);
	chars.str = expand_everything(chars.tmp, env);
	free(chars.tmp);
	if (!chars.str)
		return (perror("malloc"), -1);
	while (chars.str[i])
	{
		chars.tmp = get_next_token(chars.str + i, &i);
		if (!chars.tmp)
			return (perror("malloc"), -1);
		tmplst = ft_lstnew(chars.tmp);
		if (!tmplst)
			return (perror("malloc"), -1);
		ft_lstadd_back(args, tmplst);
		i += ft_skip_spaces(chars.str + i);
	}
	free(chars.str);
	return (0);
}

int	parse_arguments(char *line, t_proc *proc, t_list *env)
{
	char	*tmp;
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			tmp = get_next_token(line + i, &i);
			if (!tmp)
				return (perror("malloc"), -1);
			if (add_word_to_lst(tmp, proc, env))
				return (free(tmp), -1);
			free(tmp);
		}
		else
		{
			if (add_next_token_to_lst_as_tokens(line + i, &proc->args, env, &i))
				return (-1);
		}
		i += ft_skip_spaces(line + i);
	}
	return (0);
}

int	parse_command(char *line, t_proc *proc, t_list *env)
{
	int		ret;
	char	*cmd;
	char	*full_path;

	ret = parse_arguments(line, proc, env);
	if (ret)
		return (ret);
	if (!proc->args)
		return (0);
	cmd = ft_strdup(proc->args->content);
	if (!cmd)
		return (perror("malloc"), -1);
	full_path = NULL;
	ret = set_full_path(env, cmd, &full_path);
	free(cmd);
	if (ret)
		return (free(full_path), ret);
	proc->path = full_path;
	return (0);
}
