/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 16:43:39 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/28 13:28:51 by lgillard         ###   ########.fr       */
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
			&& (*(char *)(tmp->content + ft_strlen(variable)) == '='
			|| *(char *)(tmp->content + ft_strlen(variable)) == 0))
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
			&& (*(char *)(tmp->content + ft_strlen(variable)) == '='
			|| *(char *)(tmp->content + ft_strlen(variable)) == 0))
		{
			free(tmp->content);
			tmp->content = newcontent;
		}
		tmp = tmp->next;
	}
	return (0);
}

int	add_env_var(t_list *env, char *variable, char *value)
{
	char	*tmp;
	t_list	*new;

	if (!value)
		value = "";
	tmp = calloc(ft_strlen(variable) + ft_strlen(value) + 2, sizeof(char));
	if (!tmp)
		return (perror("malloc"), -1);
	ft_strlcat(tmp, variable, ft_strlen(variable) + 1);
	if (*value)
	{
		ft_strlcat(tmp, "=", ft_strlen(variable) + 2);
		ft_strlcat(tmp, value, ft_strlen(variable) + ft_strlen(value) + 2);
	}
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

// first var is always "" enchor so we can start at the second item
int	remove_env_var(t_list *env, char *variable)
{
	t_list	*tmp;
	t_list	*prev;

	tmp = env->next;
	prev = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, variable, ft_strlen(variable)) == 0
				&& (*(char *)(tmp->content + ft_strlen(variable)) == '='
					|| *(char *)(tmp->content + ft_strlen(variable)) == 0))
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
