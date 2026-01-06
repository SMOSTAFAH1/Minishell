NAME = minishell

SRCS = main.c utils.c utils2.c env.c lexer.c expand.c expand2.c \
	parser.c parser2.c builtins.c builtin_exit.c builtin_cd.c builtin_export_lexer.c \
	executor.c executor2.c exec_helpers.c exec_errors.c redir.c

OBJS = $(addprefix objs/,$(SRCS:.c=.o))

CC = cc
CFLAGS = -Wall -Wextra -Werror -I.
LDFLAGS = -lreadline

all: objs $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

objs:
	mkdir -p objs

objs/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf objs

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re