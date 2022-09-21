B_NAME	= philo_bonus

B_SRC = philo_bonus.c utils_bonus.c utils2_bonus.c

CC = gcc

CFLAGS = -Wall -Wextra -Werror -pthread #-g -fsanitize=thread

$(B_NAME) :
	$(CC) $(CFLAGS) $(B_SRC) -o $(B_NAME)
all	: $(B_NAME)
bonus : all
clean :
	@rm -rf $(B_NAME)
fclean : clean
	@rm -rf $(B_NAME)
re : clean fclean all

.PHONEY : all clean fclean re
