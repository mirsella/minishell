SRCS_MAIN = main.c signals.c close.c logging.c env_utils.c ft_lstproc.c
SRCS_PARSING = parse.c skipping.c pipeline_type.c check_unclosed_and_invalid_pipeline.c parse_line_to_proc.c set_full_path.c stat.c \
			   handle_expantion.c expander.c expand_wildcard.c wildcard_match.c get_dir_content.c parse_redirections.c handle_redirections.c heredoc.c
SRCS_EXECUTION = execute.c pipe.c processus.c
SRCS_BUILTIN = builtin.c echo.c unset.c export.c cd.c exit.c ft_env.c pwd.c
SRCS = $(addprefix srcs/, $(SRCS_MAIN)) \
	   $(addprefix srcs/parsing/, $(SRCS_PARSING)) \
	   $(addprefix srcs/execution/, $(SRCS_EXECUTION)) \
	   $(addprefix srcs/builtin/, $(SRCS_BUILTIN))
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

leak:
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=rl_leaks.txt ./minishell

.PHONY: all clean fclean re bonus
