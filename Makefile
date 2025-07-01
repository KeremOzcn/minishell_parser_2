CC = cc
CFLAGS = -Wall -Wextra -Werror -fPIC
NAME = minishell
SRC = main.c ./parser/lexer1.c ./parser/lexer2.c ./parser/parser1.c ./parser/parser2.c ./parser/parse_pushback.c \
	./parser/split_token1.c ./parser/split_token2.c ./parser/check_error.c ./parser/free.c ./parser/linked_list.c
OBJ = $(SRC:.c=.o)
LIBFT = libft/libft.a
READLINE = readline

all: $(READLINE) $(LIBFT) $(NAME)

$(READLINE):
	curl -O https://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz
	tar -xvf readline-8.2.tar.gz
	cd readline-8.2 && ./configure --prefix=${PWD}/readline
	cd readline-8.2 && make install

$(LIBFT):
	make -C libft

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFT) -L${PWD}/readline/lib  -I${PWD}/readline/include/ -lreadline -ltermcap -I.

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I${PWD}/readline/include/

clean:
	rm -f $(OBJ)
	@make clean -C libft

fclean: clean
	rm -f $(NAME)
	@make fclean -C libft
	@rm -rf readline-8.2 readline-8.2.tar.gz

re: fclean all

.PHONY: all clean fclean re