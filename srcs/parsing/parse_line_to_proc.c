/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_to_proc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 17:09:33 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/16 22:52:23 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

struct s_chars {
	char	*str;
	char	*tmp;
	char	*joined;
};

int	add_word_to_lst(char *word, t_proc *proc)
{
	t_list	*tmp;
	char	*str;

	str = ft_strdup(word);
	if (!str)
		return (perror("malloc"), -1);
	tmp = ft_lstnew(str);
	if (!tmp)
		return (perror("malloc"), -1);
	ft_lstadd_back(&proc->args, tmp);
	return (0);
}

int	add_words_to_lst(char *words, t_proc *proc)
{
	char	*tmp;
	int		i;

	i = 0;
	while (words[i])
	{
		tmp = get_next_token(words + i, &i);
		if (!tmp)
			return (perror("malloc"), -1);
		if (add_word_to_lst(tmp, proc))
			return (free(tmp), -1);
		free(tmp);
		i += ft_skip_spaces(words + i);
	}
	return (0);
}

// a="foo bar" replace $a with 'foo' 'bar'
int	parse_arguments(char *line, t_proc *proc, t_list *env)
{
	struct s_chars	chars;
	int		i;

	line = expand_vars(line, env);
	if (!line)
		return (perror("malloc"), -1);
	i = 0;
	printf("expanded var '%s'\n", line);
	while (line[i])
	{
		chars.tmp = get_next_token(line + i, &i);
		if (!chars.tmp)
			return (free(line), perror("malloc"), -1);
		chars.str = expand_everything(chars.tmp, env);
		if (!chars.str)
			return (free(chars.tmp), free(line), perror("malloc"), -1);
		if (is_wildcard(chars.tmp, env))
			add_words_to_lst(chars.str, proc);
		else
			add_word_to_lst(chars.str, proc);
		free(chars.tmp);
		free(chars.str);
		i += ft_skip_spaces(line + i);
	}
	free(line);
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
		return (ret);
	proc->path = full_path;
	return (0);
}

int	parse_line_to_proc(char *line, t_proc *proc, t_list *env)
{
	int	ret;

	ret = 0;
	if (proc->type == SUBSHELL)
		ret = parse_redirections(proc->line, proc, env);
	else
	{
		ret = parse_redirections(line, proc, env);
		if (ret)
			return (ret);
		ret = parse_command(proc->line, proc, env);
	}
	return (ret);
}
