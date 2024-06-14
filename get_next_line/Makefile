# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/04 00:17:04 by ggeorgie          #+#    #+#              #
#    Updated: 2024/05/23 22:43:42 by ggeorgie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libget_next_line.a
CC = cc
LIB = ar rcs
CFLAGS = -Wall -Werror -Wextra -g

#SOURCES = test.c \
		get_next_line_bonus.c \ 	# Alternative SOURCES to test get_next_line
SOURCES = get_next_line_bonus.c \
		get_next_line_utils_bonus.c

OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)

# Compile each source file to an binary object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
$(NAME): $(OBJECTS)
	$(LIB) $(NAME) $(OBJECTS)

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

test: re
	./get_next_line
