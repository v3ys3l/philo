NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread -pthread

SRC = main.c philo.c utils.c
OBJ = $(SRC:.c=.o)

HEADER = philo.h utils.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
