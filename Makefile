NAME = philo 

SRC_FILES = 
OBJ = $(SRC_FILES:.c=.o)

CC = gcc
CFLAGS = -g -Wall -Wextra -Werror


re: fclean all

.PHONY: all clean fclean re
