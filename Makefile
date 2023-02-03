SRCS = $(addprefix srcs/,)
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -g3 -fsanitize=address

LIBFT = libft/libft.a
NAME = minishell

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(LIBFT):
	make -C libft

bonus: all

.PHONY: all clean fclean re bonus
