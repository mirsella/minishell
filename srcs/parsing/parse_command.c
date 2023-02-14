/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 17:09:33 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/14 15:34:32 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// char	*get_next_token(char *line, int *index)
// {
// 	char	*token;
// 	int		i;
// 
// 	i = 0;
// 	while (line[i] && !ft_isspace(line[i]))
// 	{
// 		if (line[i] == '\'' || line[i] == '\"')
// 			i += skip_quotes(line + i);
// 		else if (line[i] == '(')
// 			i += skip_parenthesis(line + i);
// 		else
// 			i++;
// 	}
// 	token = ft_substr(line, 0, i);
// 	if (!token)
// 		return (perror("malloc"), NULL);
// 	*index += i;
// 	return (token);
// }
// 
// t_list	*new_lst_expand(t_list *env, char *str)
// {
// 	char	*tmp;
// 	t_list	*lst;
// 
// 	tmp = expand_everything(env, str);
// 	if (!tmp)
// 		return (NULL);
// 	lst = ft_lstnew(tmp);
// 	if (!lst)
// 	{
// 		free(tmp);
// 		return (perror("malloc"), NULL);
// 	}
// 	return (lst);
// }
// 
// int	parse_arguments(t_data *data, char *line, t_proc *proc)
// {
// 	int		i;
// 	char	*tmp;
// 	t_list	*lst;
// 
// 	i = 0;
// 	while (line[i])
// 	{
// 		i += ft_skip_spaces(line + i);
// 		if (!line[i])
// 			break ;
// 		tmp = get_next_token(line + i, &i);
// 		if (!tmp)
// 			return (-1);
// 		lst = new_lst_expand(data->env, tmp);
// 		free(tmp);
// 		if (!lst)
// 			return (-1);
// 		ft_lstadd_back(&proc->args, lst);
// 	}
// 	return (0);
// }

// int	parse_command(t_data *data, char *line, t_proc *proc)
// {
// 	char	*tmp;
// 	int		i;
// 	int		ret;
// 
// 	i = 0;
// 	line = expand_wildcards(line, data->env);
// 	if (!line)
// 		return (-1);
// 	tmp = get_next_token(line, &i);
// 	if (!tmp)
// 		return (-1);
// 	proc->path = expand_everything(data->env, tmp);
// 	free(tmp);
// 	if (!proc->path)
// 		return (-1);
// 	ret = get_full_path(data->env, &proc->path);
// 	if (ret)
// 		return (free(line), ret);
// 	if (!proc->path)
// 		return (free(line), -1);
// 	if (parse_arguments(data, line + i, proc) == -1)
// 		return (free(line), -1);
// 	return (free(line), 0);
// }

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
		if (line[i] == '\'' || line[i] == '\"')
			i += skip_quotes(line + i);
		else
			i++;
	}
	token = ft_substr(line, 0, i);
	if (!token)
		return (perror("malloc"), NULL);
	*index += i;
	return (token);
}

int	parse_arguments(t_list *env, char *line, t_proc *proc)
{
	t_list			*tmp;
	struct s_chars	chars;
	int				i;

	i = 0;
	while (line[i])
	{
		chars.tmp = get_next_token(line + i, &i);
		if (!chars.tmp)
			return (-1);
		chars.str = expand_everything(env, chars.tmp);
		free(chars.tmp);
		if (!chars.str)
			return (-1);
		tmp = ft_lstnew(chars.str);
		if (!tmp)
		{
			free(chars.str);
			return (perror("malloc"), -1);
		}
		ft_lstadd_back(&proc->args, tmp);
		i += ft_skip_spaces(line + i);
	}
	return (0);
}

int	parse_command(t_data *data, char *line, t_proc *proc)
{
	struct s_chars	chars;
	int				i;
	int				ret;

	chars.tmp = expand_vars(data->env, line);
	if (!chars.tmp)
		return (-1);
	chars.str = expand_wildcards(chars.tmp, data->env);
	free(chars.tmp);
	if (!chars.str)
		return (-1);
	i = ft_skip_spaces(chars.str);
	proc->path = get_next_token(chars.str, &i);
	if (!proc->path)
		return (free(chars.str), -1);
	ret = set_full_path(data->env, &proc->path);
	if (ret)
		return (free(chars.str), ret);
	i = ft_skip_spaces(chars.str);
	ret = parse_arguments(data->env, chars.str + i, proc);
	free(chars.str);
	if (ret)
		return (ret);
	return (0);
}
