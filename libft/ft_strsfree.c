/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsfree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:39:19 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/06 13:40:27 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"

char **ft_strsfree(char **strs)
{
	int i;

	if (!strs)
		return (NULL);
	i = 0;
	while (strs[i])
		free(strs[i++]);
	free(strs);
	return (strs);
}
