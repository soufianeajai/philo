NAME=philo

CFLAGS= -Wall -Wextra -Werror -pthread  #-fsanitize=thread

SRS= main.c parser.c initialize.c dinner.c supervise.c

OBJ = $(SRS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	cc  $(CFLAGS) $(OBJ) -o $(NAME)

fclean: clean
	rm -f $(NAME)

clean:
	rm -f $(OBJ)

re: fclean all

.PHONY: all clean fclean re
