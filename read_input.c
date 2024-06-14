/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 00:53:12 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/05 00:42:31 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	free_ascii(struct s_draw *ctrl, char **ascii_numbers)
{
	write(2, "Error: illegal input\n", 21);
	fn_free_char_ptr(&ascii_numbers);
	free_s_draw_and_exit(ctrl);
}

/**
 * Converts an ASCII string to an integer and checks if it exceeds the limits.
 * @param	char *ascii_numbers: The evaluated number/substring.
 * @param	int	sign: Sign of the evaluated number/substring.
 * @param	int	i: Index of the current character.
 * @param	struct s_fdf *array: to be freed in case of number beyond int limits.
 * @return	nbr: The signed integer value of the evaluated number/substring.
 */
//int	fdf_atoi(struct s_fdf *array, char *ascii_number)
int	fdf_atoi(struct s_draw *ctrl, char **ascii_numbers, int j)
{
	signed long long	nbr;
	int					sign;
	int					i;

	nbr = 0;
	sign = 1;
	i = 0;
	// if (ascii_number[i] == '-' || ascii_number[i] == '+')						// Maybe this can be a separate function, so that the function fits within 25 lines?
	// {
	// 	if (ascii_number[i] == '-')
	if (ascii_numbers[j][i] == '-' || ascii_numbers[j][i] == '+')
	{
		if (ascii_numbers[j][i] == '-')
			sign = -sign;
		i++;
	}
//	printf("fdf_atoi[%i]: %s, sign: %i\n", i, ascii_number, sign);
	// if (((nbr * 10 + (ascii_number[i] - '0')) * sign) < INT_MIN
	// 	|| ((nbr * 10 + (ascii_number[i] - '0')) * sign) > INT_MAX)
	if (((nbr * 10 + (ascii_numbers[j][i] - '0')) * sign) < INT_MIN
		|| ((nbr * 10 + (ascii_numbers[j][i] - '0')) * sign) > INT_MAX)
	{
		free_ascii(ctrl, ascii_numbers);
		printf("number out of integer limits\n");
		exit (EXIT_FAILURE);
	}
	else
	{
//		while (ascii_number[i])
		while (ascii_numbers[j][i])
		{
//			if (ascii_number[i] < '0' || ascii_number[i] > '9')
			if (ascii_numbers[j][i] < '0' || ascii_numbers[j][i] > '9')
			{
				// printf("non-numeric character in the number\n");
				// exit (EXIT_FAILURE);
				free_ascii(ctrl, ascii_numbers);
			}
//			printf("fdf_atoi[%i]: %c, nbr: %lli\n", i, ascii_number[i], nbr);
//			nbr = nbr * 10 + (ascii_number[i] - '0');
			nbr = nbr * 10 + (ascii_numbers[j][i] - '0');
//			printf("integer[%i]: %lli\n", i, nbr);
			i++;
		}
	}
	return (nbr * sign);
}

/**
 * Helper function for 'fill_array'.
 * @param	struct	s_fdf *array: a structure to be filled with input data.
 * @param	int		fd: file descriptor for the input file.
 * @param	char	*row: a line from the input file.
 * @param	char	**ascii_numbers: an array, where each line represents a
 * number/word/sub-string and the number of lines is equal to '(*array).w'.
 * @param	int		j: index for the position within a 'row' string.
 */
char	**get_ascii_line(int fd)
{
	char	*row;
	int		j;
	char	**ascii_numbers;

	row = get_next_line(fd);
	if (!row)
	{
		/* error-handling code */
		// free_s_fdf(array);
		exit(FAILURE);
	}
//	printf("row [%i] to split: '%6s'\n", i_row, row);
	j = 0;
	while (row[j])																// swap '\n' to '\0' to define the end of the loop, but the program seems to work without this as well
	{
//		printf("number[%i]: '%c'\n", i_column, row[i_column]);
		if (row[j] == '\n')
		{
			row[j] = ' ';
		}
//		printf("number[%i]: '%c'\n", col, row[col]);
		j++;
	}
	ascii_numbers = ft_split(row, ' ');
	if (!ascii_numbers)
		exit (FAILURE);
	fn_free(&row);
	return (ascii_numbers);
}

/**
 * Get the input, split it by lines, then by words/sub-strings,
 * convert the latter to numbers and save them in 'array' struct
 * @param	struct s_draw 	*ctrl :	Drawing control structure
 * @param	int		fd: file descriptor for the input file.
 * @param	int		i_row: a counter for the 'row's coming from input file
 * 							and for 'array' rows.
 * @param	char	**ascii_numbers: an array, where each line represents a
 * number/word/sub-string and the number of lines is equal to '(*array).w'.
 * @param	int		j: a counter for lines in 'ascii_numbers' rows
 * 						& integers in '(*array).z' columns.
 */
//void	fill_array(struct s_fdf *array, int fd)
void	fill_array(struct s_draw *ctrl, int fd)
{
//	char	*row;
	int		i_row;
	char	**ascii_numbers;
	int		j;

	i_row = 0;
	while (i_row < (*((*ctrl).array)).h)										// There's no need of '=' for a dummy line with '0's, as it is already allocated and handled at the end of the function
	{
		ascii_numbers = get_ascii_line(fd);
		if (!ascii_numbers)
		{
			/* error-handling code */
			// write(2, mlx_strerror(MLX_MEMFAIL), ft_strlen(mlx_strerror(MLX_MEMFAIL)));
			// write(2, "\n", 1);
			printf("failed to allocate memory\n");
			// free_s_fdf(array);
			// exit (EXIT_FAILURE);
			free_ascii(ctrl, ascii_numbers);
		}
// // Print input
// 		printf("row[%i] of ascii_numbers: ", i_row);
// 		j = 0;
// 		while (j < (*array).width)
// 		{
// 			printf("%6s", ascii_numbers[j]);
// 			j++;
// 		}
// 		printf("\n");
		
		extract_colors(ctrl, ascii_numbers, i_row);
		
// // Print input
// 		printf("row[%i] of colors: ", i_row);
// 		j = 0;
// 		while (j < (*array).width)
// 		{
// 			printf("\t%lu \t", (*array).color[i_row][j]);
// 			j++;
// 		}
// 		printf("\n");

		j = 0;
//		printf("\t\t\tz[row: %i]\n", i_row);
		while (j < (*((*ctrl).array)).w)
		{
//			printf("pre-atoi int[col: %i]: %s\n", j, ascii_numbers[j]);
//			(*array).z[i_row][j] = fdf_atoi(array, ascii_numbers[j]);
			(*((*ctrl).array)).z[i_row][j] = fdf_atoi(ctrl, ascii_numbers, j);
			// if ((*array).z[i_row][j] > Z_MAX || (*array).z[i_row][j] < Z_MIN)
			// {
			// 	printf("%i makes the program tired!\n", (*array).z[i_row][j]);
			// 	exit (EXIT_FAILURE);
			// }
//			printf("\t\t\t\tinteger[row: %i][col: %i]: %i\n", i_row, j, (*array).z[i_row][j]);
			fn_free(&ascii_numbers[j]);
			j++;
		}
//		(*array).z[i_row][j] = '\0';											// This is not needed, as 'z' is an array of integers, not strings.
//		fn_free_char_ptr(ascii_numbers);										// Causes double free
//		printf("about to 'fn_free((char *)ascii_numbers);'\n");
		fn_free((char **)&ascii_numbers);
		i_row++;
	}
//	(*array).z[i_row] = NULL;													// This is not needed, as 'z' is an array of integers, not strings.
//	(*array).color[i_row] = NULL;												// This is not needed, as 'color' is an array of integers, not strings.
}

// Initializes a 2D array of long unsigned integers with given height and width.
unsigned long	**initialize_lu_array(int height, int width)
{
	unsigned long	**array;
	int				i_y;

//	printf("height: %i, width: %i\n", height, width);
//	array = malloc((height) * sizeof(unsigned long *));
	array = ft_calloc(height, sizeof(unsigned long *));							// No need of '+ 1' for the NULL-terminator, because it is an array of integers, not strings.
	if (!array)
	{
		/* error-handling code */
		// write(2, mlx_strerror(MLX_MEMFAIL), ft_strlen(mlx_strerror(MLX_MEMFAIL)));
		// write(2, "\n", 1);
		return (NULL);
	}
	i_y = 0;
	while (i_y < height)
	{
//		printf("%i. height: %i, width: %i\n", i_y, height, width);
//		array[i_y] = malloc((width) * sizeof(unsigned long));
		array[i_y] = ft_calloc(width, sizeof(unsigned long));					// No need of '+ 1' for the NULL-terminator, because it is an array of integers, not strings.
		if (!array[i_y])
		{
			/* error-handling code */
			// write(2, mlx_strerror(MLX_MEMFAIL), ft_strlen(mlx_strerror(MLX_MEMFAIL)));
			// write(2, "\n", 1);
			free_lu_array(&array, i_y);
			return (NULL);
		}
//		printf("array[%i][0] %i\n", i_y, array[i_y][0]);
		i_y++;
	}
	return (array);
}

// Initializes a 2D array of integers with given height and width.
int	**initialize_int_array(int height, int width)
{
	int		**array;
	int		i_y;

//	printf("height: %i, width: %i\n", height, width);
//	array = malloc((height) * sizeof(int *));
	array = ft_calloc(height, sizeof(int *));									// No need of '+ 1' for the NULL-terminator, because it is an array of integers, not strings.
	if (!array)
	{
		/* error-handling code */
		// write(2, mlx_strerror(MLX_MEMFAIL), ft_strlen(mlx_strerror(MLX_MEMFAIL)));
		// write(2, "\n", 1);
		return (NULL);
	}
	i_y = 0;
	while (i_y < height)
	{
//		printf("%i. height: %i, width: %i\n", i_y, height, width);
//		array[i_y] = malloc((width) * sizeof(int));
		array[i_y] = ft_calloc(width, sizeof(int));								// No need of '+ 1' for the NULL-terminator, because it is an array of integers, not strings.
		if (!array[i_y])
		{
			/* error-handling code */
			// write(2, mlx_strerror(MLX_MEMFAIL), ft_strlen(mlx_strerror(MLX_MEMFAIL)));
			// write(2, "\n", 1);
//			free_array(&array);													// Maybe try this instead of the next line, but first modify 'free_array' to accept a pointer to a pointer. Also, adjust other functions which use 'free_array'.
			free_int_array(&array, i_y);
			return (NULL);
		}
//		printf("array[%i][0] %i\n", i_y, array[i_y][0]);
		i_y++;
	}
	return (array);	
}

/**
 * Helper function for 'get_dimensions'.
 * @param	char	*row: holds the string read by 'get_next_line'.
 * @param	int		file_columns: counter of the number of columns.
 * @return	SUCCESS/FAILURE of funciton execution.
 */
int	scan_input(struct s_draw *ctrl, int fd, int file_rows)
{
	char	*row;
	int		file_columns;

	row = ft_calloc(2, sizeof(char));
	if (!row)
	{
		/* error-handling code */
		printf("failed to allocate memory\n");
		// write(2, mlx_strerror(MLX_MEMFAIL), ft_strlen(mlx_strerror(MLX_MEMFAIL)));
		// write(2, "\n", 1);
		// return (1);
		return (FAILURE);
	}
	row[0] = '1';
//	printf("going from get_dimensions to get_next_line\n");
	while (row)
	{
		fn_free(&row);															// Why do we need this here? Where is the memory for the next row allocated? Maybe next line should be 'malloc'? -> No, it is allocated in 'get_next_line'
		row = get_next_line(fd);
//		if (row && row[0] != '\n')												// 'row[0] != '\n'' doesn't seem to be of much use
		if (row)
		{
//			printf("row[%i]: '%s'\n", file_rows, row);
			file_columns = word_counter(row, ' ');
		}
		if (file_rows != 0 && file_columns != (*((*ctrl).array)).w)
		{
			/* error-handling code */
			printf("the number of columns in the file is not consistent\n");
			printf("file_columns: %i, (*array).width: %i @ row[%i]\n", file_columns, (*((*ctrl).array)).w, file_rows);
			// write(2, mlx_strerror(MLX_INVIMG), ft_strlen(mlx_strerror(MLX_INVIMG)));
			// write(2, "\n", 1);
			// // return (1);
			free_ascii(ctrl, 0);
		}
		else
		{
			(*((*ctrl).array)).w = file_columns;
		}		
		file_rows++;
	}
	fn_free(&row);
	return (file_rows);
}

// <https://manpages.ubuntu.com/manpages/mantic/en/man2/open.2.html>
// <https://manpages.ubuntu.com/manpages/noble/en/man2/close.2.html>
/**
 * Read all lines in the file to figure how many they are.
 * Simultaneously, using 'word_counter',
 * figure out how many numbers/words/sub-strings are in each line.
 * @param	char	*file_name: The input file to be read from.
 * @param	struct s_fdf	*array: a structure to be filled with input data.
 * @param	int		fd: file descriptor for the input file.
 * @param	int		file_rows: counter of the number of rows.
 * @param	int		file_columns: counter of the number of columns.
 * @return	SUCCESS/FAILURE of funciton execution.
 */
//int	get_dimensions(char *file_name, struct s_fdf *array)
int	get_dimensions(char *file_name, struct s_draw *ctrl)
{
	int		fd;
	int		file_rows;
//	int		file_columns;
//	char	*row;

//	printf("in get_y\n");
	fd = open(file_name, O_RDONLY);
	if (fd < 0)
	{
		// printf("cannot open the file with input data\n");
		write(2, "File was invalid / does not exist.\n", 35);
		return (FAILURE);
	}
	printf("\nfd: %i: %s\n", fd, file_name);
	file_rows = 0;
//	(*array).height = file_rows;
	(*((*ctrl).array)).h = file_rows;
//	file_columns = 0;															// This can be omitted
	file_rows = scan_input(ctrl, fd, file_rows);
//	if (close(fd) < 0)
	if (file_rows == FAILURE || close(fd) < 0)
	{
		// free everything that has been allocated so far
		printf("cannot close the file with input data\n");
		// return (1);
		return (FAILURE);
	}
	(*((*ctrl).array)).h = file_rows - 1;
	printf("file rows: %i, columns: %i\n", (*((*ctrl).array)).h, (*((*ctrl).array)).w);
	if ((*((*ctrl).array)).h == 0 && (*((*ctrl).array)).w == 0)
	{
		/* error-handling code */
		write(2, mlx_strerror(MLX_INVIMG), ft_strlen(mlx_strerror(MLX_INVIMG)));
		write(2, "\n", 1);
		write(2, "Error: empty file\n", 18);
		return (FAILURE);
	}
	return (SUCCESS);
}

/**
 * Read the input file, get the dimensions of the map,
 * allocate memory for the map, and fill it with the numbers from the file.
 * @param	char	*file_name: The input file to be read from.
 * @param	struct	s_fdf *array: a structure to be filled with input data.
 * @param	int		fd: file descriptor for the input file.
 */
//void	read_input(char *file_name, struct s_fdf *array)
void	read_input(char *file_name, struct s_draw *ctrl)
{
	int		fd;

//	printf("in read_input\n");
//	if (get_dimensions(file_name, array))
	if (get_dimensions(file_name, ctrl))
	{
//		/* error-handling code */;
//		exit (EXIT_FAILURE);
		free_s_draw_and_exit(ctrl);
	}
//	printf("file rows: %i, columns: %i\n", (*array).height, (*array).width);
//	(*array).z = initialize_int_array((*array).height, (*array).width);
	(*((*ctrl).array)).z
		= initialize_int_array((*((*ctrl).array)).h, (*((*ctrl).array)).w);
	if (!(*((*ctrl).array)).z)
		free_s_draw_and_exit(ctrl);
//	printf("z[0][0] %i\n", (*array).z[0][0]);
//	(*array).color = initialize_lu_array((*array).height, (*array).width);		// This should suffice as the return value of 'initialize_lu_array' is 'unsigned long	**'.
	(*((*ctrl).array)).clr
		= initialize_lu_array((*((*ctrl).array)).h, (*((*ctrl).array)).w);
	if (!(*((*ctrl).array)).clr)
		free_s_draw_and_exit(ctrl);
//	printf("color[0][0] %lu\n", (*array).color[0][0]);
	fd = open(file_name, O_RDONLY);
	if (fd < 0)
	{
		/* error-handling code */
		free_s_draw_and_exit(ctrl);
	}
//	printf("going from read_input to fill_array\n");
//	fill_array(array, fd);
	fill_array(ctrl, fd);
	if (close(fd) < 0)
	{
		/* error-handling code */
		// free everything that has been allocated so far
		// printf("cannot close the file with input data\n");
		// return (1);
		free_s_draw_and_exit(ctrl);
	}
//	printf("leaving read_input back to fdf.c\n");
}
