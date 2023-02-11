/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 16:43:39 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/11 21:22:43 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(t_list *env, char *variable)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, variable, ft_strlen(variable)) == 0)
		{
			if (*(char *)(tmp->content + ft_strlen(variable)) == '=')
				return (tmp->content + ft_strlen(variable) + 1);
		}
		tmp = tmp->next;
	}
	return (NULL);
}
