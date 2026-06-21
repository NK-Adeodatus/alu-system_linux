CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
SRC = main.c nm_64.c nm_32.c symbol_type.c endian.c
OBJ = $(SRC:.c=.o)
NAME = hnm

.PHONY: hnm clean fclean re

hnm: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean hnm
