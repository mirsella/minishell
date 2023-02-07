/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strfindword.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 21:16:25 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/07 21:26:45 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strfindword(char *s, const char *word)
{
	int	i;
	int	findi;

	if (word[0] == '\0')
		return (s);
	i = 0;
	while (s[i] != '\0')
	{
		findi = 0;
		while (s[i + findi] == word[findi])
		{
			if (word[findi + 1] == '\0')
				return (&s[i]);
			findi++;
		}
		i++;
	}
	return (NULL);
}
