NAME=philo

CFLAGS= -Wall -Wextra -Werror -pthread #-g -fsanitize=thread

SRS= main.c parser.c initialize.c dinner.c

OBJ = $(SRS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	cc  $(CFLAGS) $(OBJ) -o $(NAME)

fclean: clean
	rm -f $(NAME)

clean:
	rm -f $(OBJ)

re: fclean all

debug: $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME) -g3

.PHONY: all clean fclean re
