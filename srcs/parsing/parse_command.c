/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 17:09:33 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/13 19:23:55 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_next_token(char *line, int *index)
{
	char	*token;
	int		i;

	i = 0;
	while (line[i] && !ft_isspace(line[i]))
	{
		if (line[i] == '\'' || line[i] == '\"')
			i += skip_quotes(line + i);
		else if (line[i] == '(')
			i += skip_parenthesis(line + i);
		else
			i++;
	}
	token = ft_substr(line, 0, i);
	if (!token)
		return (perror("malloc"), NULL);
	*index += i;
	return (token);
}

t_list	*new_lst_expand(t_list *env, char *str)
{
	char	*tmp;
	t_list	*lst;

	tmp = expand_everything(env, str);
	if (!tmp)
		return (NULL);
	lst = ft_lstnew(tmp);
	if (!lst)
	{
		free(tmp);
		return (perror("malloc"), NULL);
	}
	return (lst);
}

// $nonexistantvar doesn't count as a variable, but "$nonexistantvar" does
int	isemptyvar(char *str, t_list *env)
{
	char	*tmp;
	int		i;

	i = 0;
	if (*str != '$')
		return (0);
	tmp = expand_var(env, str, &i);
	if (!tmp)
		return (-1);
	if (*tmp == '\0')
	{
		free(tmp);
		return (1);
	}
	free(tmp);
	return (0);
}

int	parse_arguments(t_data *data, char *line, t_proc *proc)
{
	int		i;
	char	*tmp;
	t_list	*lst;

	i = 0;
	while (line[i])
	{
		i += ft_skip_spaces(line + i);
		if (!line[i])
			break ;
		tmp = get_next_token(line + i, &i);
		if (!tmp)
			return (-1);
		if (isemptyvar(tmp, data->env) == 1)
		{
			free(tmp);
			continue ;
		}
		lst = new_lst_expand(data->env, tmp);
		free(tmp);
		if (!lst)
			return (-1);
		ft_lstadd_back(&proc->args, lst);
	}
	return (0);
}

int	parse_command(t_data *data, char *line, t_proc *proc)
{
	char	*cmd;
	char	*tmp;
	int		i;

	i = 0;
	line = expand_wildcards(line, data->env);
	if (!line)
		return (-1);
	tmp = get_next_token(line, &i);
	if (!tmp)
		return (-1);
	cmd = expand_everything(data->env, tmp);
	if (!cmd)
		return (free(tmp), -1);
	free(tmp);
	proc->path = get_full_path(data->env, cmd);
	free(cmd);
	if (!proc->path)
		return (free(line), perror("malloc"), -1);
	if (parse_arguments(data, line + i, proc) == -1)
		return (free(line), -1);
	return (free(line), 0);
}
