/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:18:11 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/07 23:55:39 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"
#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

t_list	*ft_lstnew_strs(int size, char **strs)
{
	t_list	*l;
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = 0;
	l = 0;
	while (i < size)
	{
		l = ft_lstnew(strs[i]);
		if (!l)
		{
			ft_lstclear(&tmp, free);
			return (NULL);
		}
		l->next = tmp;
		tmp = l;
		i++;
	}
	return (l);
}
