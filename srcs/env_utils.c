/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 16:43:39 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/20 23:18:20 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_var(t_list *env, char *variable)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, variable, ft_strlen(variable)) == 0
			&& *(char *)(tmp->content + ft_strlen(variable)) == '=')
		{
			return (tmp->content + ft_strlen(variable) + 1);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

int	replace_env_var(t_list *env, char *variable, char *newcontent)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, variable, ft_strlen(variable)) == 0
			&& *(char *)(tmp->content + ft_strlen(variable)) == '=')
		{
			free(tmp->content);
			tmp->content = newcontent;
		}
		tmp = tmp->next;
	}
	return (0);
}

int	set_env_var(t_list *env, char *variable, char *value)
{
	char	*tmp;
	t_list	*new;

	if (!variable || !value)
		return (perror("malloc"), -1);
	tmp = calloc(ft_strlen(variable) + ft_strlen(value) + 2, sizeof(char));
	if (!tmp)
		return (perror("malloc"), -1);
	ft_strlcat(tmp, variable, ft_strlen(variable) + 1);
	ft_strlcat(tmp, "=", ft_strlen(variable) + 2);
	ft_strlcat(tmp, value, ft_strlen(variable) + ft_strlen(value) + 2);
	if (get_env_var(env, variable))
		replace_env_var(env, variable, tmp);
	else
	{
		new = ft_lstnew(tmp);
		if (!new)
			return (perror("malloc"), -1);
		ft_lstadd_back(&env, new);
	}
	return (0);
}

int	set_exit_code_to_env(t_list *env)
{
	char	*exitcode;

	exitcode = ft_itoa(g_exit_code);
	if (set_env_var(env, "?", exitcode))
		return (free(exitcode), -1);
	free(exitcode);
	return (0);
}

// first var is always ? so we can start at the second item
int	remove_env_var(t_list *env, char *variable)
{
	t_list	*tmp;
	t_list	*prev;

	tmp = env->next;
	prev = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, variable, ft_strlen(variable)) == 0
			&& *(char *)(tmp->content + ft_strlen(variable)) == '=')
		{
			prev->next = tmp->next;
			ft_lstdelone(tmp, free);
			return (0);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (0);
}
