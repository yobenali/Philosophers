NAME = philosophers

SRC = philo/philo.c philo/utils.c philo/utils2.c

CC = gcc

CFLAGS = -Wall -Wextra -Werror -pthread #-g -fsanitize=thread

$(NAME) :
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

all	: $(NAME)

clean :
	@rm -rf $(NAME)

fclean : clean
	@rm -rf $(NAME)

re : clean fclean all

.PHONEY : all clean fclean re
