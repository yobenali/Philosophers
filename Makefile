NAME	= philosophers
B_NAME	= philosophers_bonus

SRC	= philo/philo.c philo/utils.c philo/utils2.c
B_SRC = philo_bonus/philo_bonus.c philo_bonus/utils_bonus.c philo_bonus/utils2_bonus.c

CC = gcc

CFLAGS = -Wall -Wextra -Werror -pthread #-g -fsanitize=thread

$(NAME) :
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)
$(B_NAME) :
	$(CC) $(CFLAGS) $(B_SRC) -o $(B_NAME)
all	: $(NAME)
bonus : $(B_NAME)
clean :
	@rm -rf $(NAME)
	@rm -rf $(B_NAME)
fclean : clean
	@rm -rf $(NAME)
	@rm -rf $(B_NAME)

re : clean fclean all

.PHONEY : all clean fclean re
