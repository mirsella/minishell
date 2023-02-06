/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 22:35:33 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/06 13:40:56 by lgillard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

int					ft_floor(double x);
int					ft_ceil(double x);
int					ft_round(double x);
int					ft_isascending(int *tab, int size);
int					ft_isdescending(int *tab, int size);
int					ft_atoi(const char *str);
long long			ft_atoll(const char *str);
int					ft_isalnum(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isprint(int c);
int					ft_lstsize(t_list *lst);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_tolower(int c);
int					ft_toupper(int c);
int					ft_putchar(char c);
int					ft_putstr(char *s);
int					ft_putnbr(long long n);
int					ft_putnbr_base(long long n, char *base);
long long			ft_llmax(long long a, long long b);
unsigned long long	ft_ullmax(unsigned long long a, unsigned long long b);
long long			ft_llmin(long long a, long long b);
unsigned long long	ft_ullmin(unsigned long long a, unsigned long long b);
int					ft_nbrlen(long long n);
int					ft_nbrlen_base(long long n, int base);
int					ft_ullnbrlen(unsigned long long n);
int					ft_ullnbrlen_base(unsigned long long n, int base);
int					ft_abs(int n);
long long			ft_llabs(long long n);
int					ft_printf(const char *format, ...);
char 				**ft_strsfree(char **strs);
char				**ft_split(char const *s, char c);
char				*ft_itoa(int n);
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s1);
char				**ft_strstrdup(char **strs);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strnstr(const char *haystack, const char *needle,
						size_t len);
char				*ft_strrchr(const char *s, int c);
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_get_next_line(int fd, int freee);
size_t				ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t				ft_strlen(const char *s);
t_list				*ft_lstlast(t_list *lst);
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
t_list				*ft_list_find(t_list *begin_list, void *data_ref,
						int (*cmp)(void *, void *));
t_list				*ft_lstnew(void *content);
t_list				*ft_list_at(t_list *begin_list, unsigned int nbr);
void				ft_intsort(int *tab, int size);
void				*ft_calloc(size_t count, size_t size);
void				*ft_memccpy(void *dest, const void *src, int c, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memset(void *b, int c, size_t len);
void				ft_bzero(void *s, size_t n);
void				ft_lstadd_back(t_list **alst, t_list *newel);
void				ft_lstadd_front(t_list **alst, t_list *newel);
void				ft_lstclear(t_list **lst, void (*del)(void*));
void				ft_lstdelone(t_list *lst, void (*del)(void*));
void				ft_lstiter(t_list *lst, void (*f)(void *));
void				ft_putchar_fd(char c, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));

#endif
