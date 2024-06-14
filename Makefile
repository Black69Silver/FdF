# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/04 00:17:04 by ggeorgie          #+#    #+#              #
#    Updated: 2024/06/05 20:39:32 by ggeorgie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -Ofast: <https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html>
# m: Math library
# glfw: Graphics Library Framework
# Cocoa: Apple's native API
# OpenGL: Graphics API
# IOKit: Input/Output Kit

NAME = fdf
CC = cc
CFLAGS = -Wall -Werror -Wextra -Wunreachable-code -Ofast
MLX_PATH = ./MLX42
MLX_INC = $(MLX_PATH)/include
MLX_LIB = $(MLX_PATH)/build/libmlx42.a
GNL_PATH = get_next_line
GNL_LIB = $(GNL_PATH)/get_next_line.a
LIB = -Lget_next_line \
	-lget_next_line \
	-lm \
	-lglfw -framework Cocoa -framework OpenGL -framework IOKit

SOURCES = fdf.c \
		read_input.c \
		read_utils.c \
		ft_split.c \
		colors.c \
		initialize_drawing.c \
		draw.c \
		interactions.c \
		draw_utils.c \
		memory_control.c \
		utils.c

OBJECTS = $(SOURCES:.c=.o) 

all: $(NAME)

# Check if the MLX42 submodule is initialized and up-to-date
$(MLX_PATH)/CMakeLists.txt:
	@echo "Initializing MLX42 submodule..."
	@git submodule update --init --recursive

# Compile the MLX42 library
$(MLX_LIB): $(MLX_PATH)/CMakeLists.txt
	@cmake $(MLX_PATH) -B $(MLX_PATH)/build && cmake --build $(MLX_PATH)/build -j4

# Compile the get_next_line dependency archive
$(GNL_LIB):
	@make -C $(GNL_PATH)

# Compile each source file to an binary object file
# In order to avoid relinking, use '-c' flag. 
#	* '$<' represents the source file.
#	* '$@' represents the target/object file.
#	* so that only the necessary files are (re-)compiled.
%.o: %.c
	$(CC) $(CFLAGS) -I $(MLX_INC) -I $(GNL_PATH) -c $< -o $@

$(NAME): $(MLX_LIB) $(GNL_LIB) $(OBJECTS)
	@$(CC) $(CFLAGS) -I $(MLX_INC) $(OBJECTS) $(LIB) -o $(NAME) $(MLX_LIB)

# Delete '.o' object and other intermediate files
clean:
	rm -f $(OBJECTS)
	rm -rf $(MLX_PATH)/build
	make clean -C $(GNL_PATH)

# clean + Delete executable and archive files
fclean: clean
	rm -f $(NAME)
	make fclean -C $(GNL_PATH)

re: fclean all

test: $(NAME)
#test: re

#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/simple.fdf
	./fdf ~/Gprojects/24_FdF/Extras/test_maps/simple_col.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/10-2.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/10-70.fdf
	./fdf ~/Gprojects/24_FdF/Extras/test_maps/20-60.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/42.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/50-4.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/100-6.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/basictest.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/elem-col.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/elem.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/elem2.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/flat.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/mars.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/one_line_break.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/one_line.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/one_space.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/pentenegpos.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/pnp_flat.fdf
	./fdf ~/Gprojects/24_FdF/Extras/test_maps/pylone.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/pyra.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/pyramide.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/t1.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/t2.fdf

# Maps with errors
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/pyr.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/empty.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/invalid_values.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/unequal_lines.fdf
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/no_read.fdf

# Huge maps
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/elem-fract.fdf					// This is a 500 * 500 color map and will take a long time to render.
#	./fdf ~/Gprojects/24_FdF/Extras/test_maps/julia.fdf							// This is a 500 * 500 color map and will take a long time to render.

#	valgrind --leak-check=full ./fdf ~/Gprojects/24_FdF/Extras/test_maps/basictest.fdf - not working, as it shows the memory leaks from the MLX42 library

# prevent collisions with files with the same name in the directory
.PHONY: all clean fclean re test
