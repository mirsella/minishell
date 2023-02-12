SRCS_MAIN = main.c signals.c close.c logging.c env.c
SRCS_PARSING = parse.c skipping.c pipeline_type.c prompt_loop_utils.c parse_redirections.c handle_redirections.c parse_command.c get_full_path.c \
			   expand_everything.c expand.c expand_wildcard.c wildcard.c 
SRCS_PROCS = ft_lst.c
SRCS = $(addprefix srcs/, $(SRCS_MAIN)) \
	   $(addprefix srcs/parsing/, $(SRCS_PARSING)) \
	   $(addprefix srcs/procs/, $(SRCS_PROCS))
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -g3
# CFLAGS += -fsanitize=address

LIBFT = libft/libft.a
NAME = minishell

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(LIBFT):
	make -C libft

bonus: all

.PHONY: all clean fclean re bonus
