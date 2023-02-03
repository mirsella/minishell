/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 23:13:50 by mirsella          #+#    #+#             */
/*   Updated: 2022/11/13 15:02:44 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include "stdlib.h"
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	wordlen;
	char	*str;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
		return (ft_strdup(""));
	wordlen = 0;
	i = 0;
	while (s[start + wordlen] && wordlen < len)
		wordlen++;
	str = malloc((sizeof(char) * wordlen + 1));
	if (!str)
		return (NULL);
	while (s[start + i] && i < wordlen)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

// #include "stdio.h"
// int main()
// {
// 	char *str;
// 	char *s;
// 	str = ft_strdup("0123456789");
//	s = ft_substr(str, 9, 10);
// 	printf("'%s'", s);
// 	return (0);
// }
