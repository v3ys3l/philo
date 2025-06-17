NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = main.c philo.c utils.c init_data.c monitor.c philo_life.c
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

.PHONY: all clean fclean re