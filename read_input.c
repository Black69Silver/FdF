/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 00:53:12 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/04 23:32:53 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
		exit(FAILURE);
	j = 0;
	while (row[j])
	{
		if (row[j] == '\n')
			row[j] = ' ';
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
void	fill_array(struct s_draw *ctrl, int fd)
{
	int		i_row;
	char	**ascii_numbers;
	int		j;

	i_row = 0;
	while (i_row < (*((*ctrl).array)).h)
	{
		ascii_numbers = get_ascii_line(fd);
		if (!ascii_numbers)
			free_ascii(ctrl, ascii_numbers);
		extract_colors(ctrl, ascii_numbers, i_row);
		j = 0;
		while (j < (*((*ctrl).array)).w)
		{
			(*((*ctrl).array)).z[i_row][j] = fdf_atoi(ctrl, ascii_numbers, j);
			fn_free(&ascii_numbers[j]);
			j++;
		}
		fn_free((char **)&ascii_numbers);
		i_row++;
	}
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
		return (FAILURE);
	row[0] = '1';
	while (row)
	{
		fn_free(&row);
		row = get_next_line(fd);
		if (row)
			file_columns = word_counter(row, ' ');
		if (file_rows != 0 && file_columns != (*((*ctrl).array)).w)
			free_ascii(ctrl, 0);
		else
			(*((*ctrl).array)).w = file_columns;
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
int	get_dimensions(char *file_name, struct s_draw *ctrl)
{
	int		fd;
	int		file_rows;

	fd = open(file_name, O_RDONLY);
	if (fd < 0)
	{
		write(2, "File was invalid / does not exist.\n", 35);
		return (FAILURE);
	}
	file_rows = 0;
	(*((*ctrl).array)).h = file_rows;
	file_rows = scan_input(ctrl, fd, file_rows);
	if (file_rows == FAILURE || close(fd) < 0)
		return (FAILURE);
	(*((*ctrl).array)).h = file_rows - 1;
	if ((*((*ctrl).array)).h == 0 && (*((*ctrl).array)).w == 0)
	{
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
void	read_input(char *file_name, struct s_draw *ctrl)
{
	int		fd;

	if (get_dimensions(file_name, ctrl))
		free_s_draw_and_exit(ctrl);
	(*((*ctrl).array)).z
		= initialize_int_array((*((*ctrl).array)).h, (*((*ctrl).array)).w);
	if (!(*((*ctrl).array)).z)
		free_s_draw_and_exit(ctrl);
	(*((*ctrl).array)).clr
		= initialize_lu_array((*((*ctrl).array)).h, (*((*ctrl).array)).w);
	if (!(*((*ctrl).array)).clr)
		free_s_draw_and_exit(ctrl);
	fd = open(file_name, O_RDONLY);
	if (fd < 0)
		free_s_draw_and_exit(ctrl);
	fill_array(ctrl, fd);
	if (close(fd) < 0)
		free_s_draw_and_exit(ctrl);
}
