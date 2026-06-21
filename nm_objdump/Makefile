CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

SRC_NM = main.c nm_64.c nm_32.c symbol_type.c endian.c
OBJ_NM = $(SRC_NM:.c=.o)
NAME_NM = hnm

SRC_OBJDUMP = objdump_main.c objdump_64.c objdump_32.c objdump_utils.c endian.c
OBJ_OBJDUMP = $(SRC_OBJDUMP:.c=.o)
NAME_OBJDUMP = hobjdump

.PHONY: all hnm hobjdump clean fclean re

all: $(NAME_NM) $(NAME_OBJDUMP)

hnm: $(OBJ_NM)
	$(CC) $(CFLAGS) $(OBJ_NM) -o $(NAME_NM)

hobjdump: $(OBJ_OBJDUMP)
	$(CC) $(CFLAGS) $(OBJ_OBJDUMP) -o $(NAME_OBJDUMP)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_NM) $(OBJ_OBJDUMP)

fclean: clean
	rm -f $(NAME_NM) $(NAME_OBJDUMP)

re: fclean all
