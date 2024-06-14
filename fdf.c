/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 23:45:35 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/05 00:29:03 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Checks the program for leaks.
 */
void	leaks(void)																// comment out before submission
{
	system("leaks fdf");	
}

// Prints a memory allocation failure message.
int	print_mem_fail(void)
{
	write(2, mlx_strerror(MLX_MEMFAIL), ft_strlen(mlx_strerror(MLX_MEMFAIL)));
	write(2, "\n", 1);
	exit (FAILURE);
}

int	print_wrong_argc(void)
{
	write(2, "The program needs 1 file as input.\n", 35);
	exit (FAILURE);
}

/**
 * Checks if input file extension is '.fdf'.
 * @return	Exits the program if the file extension is not correct.
 */
int	check_extension(char *file_name)
{
	int	i;

	i = 0;
	while (file_name[i])
	{
		if (file_name[i] == '.' && file_name[i + 1] == 'f'
			&& file_name[i + 2] == 'd' && file_name[i + 3] == 'f')
		{
			// (*((*ctrl).array)).name = ft_strjoin("filet de fer / iron wire net of ", file_name);
			// printf("File name: %s\n", (*((*ctrl).array)).name);
			return (SUCCESS);
		}
		i++;
	}
	write(2, mlx_strerror(MLX_INVEXT), ft_strlen(mlx_strerror(MLX_INVEXT)));
	write(2, "\n", 1);
	exit (FAILURE);
}

/**
 * Takes a formated string with heights of map points
 * and draws the map in a window.
 * Vertical input is limited between -30,000 and +30,000.
 * Translation/shifting is limited to
 * 						50% outside the window width and 100% of window height.
 * Zooming is limited to the fewer pixels of the window width and height.
 * @param	struct s_draw 	*ctrl :	Drawing control structure
 * @return	Draws a filet de fer / iron wire net.
 */
int	main(int argc, char **argv)
{
	struct s_draw	*ctrl;

	atexit(leaks);																// comment out before submission
	if (argc != 2)
	{
//		printf("wrong number of arguments or file extension is not .fdf\n");
//		return (FAILURE);
		print_wrong_argc();
	}
//	printf("\nfd[0]: %i: %s\n", open(argv[0], O_RDONLY), argv[0]);
//	printf("fd[1]: %i: %s\n", open(argv[1], O_RDONLY), argv[1]);
	ctrl = ft_calloc(1, sizeof(struct s_draw));
	if (!ctrl)
	{
		print_mem_fail();
//		return (FAILURE);
	}
	(*ctrl).array = ft_calloc(1, sizeof(struct s_fdf));
	if (!(*ctrl).array)
	{
		fn_free((char **)&ctrl);
		print_mem_fail();
//		return (FAILURE);
	}
	check_extension(argv[1]);
//	printf("going to read_input\n");
	read_input(argv[1], ctrl);

// // Temp print input
// 	int	i;
// 	int	j;

// 	i = 0;
// 	while (i < (*array).h)
// 	{
// 		j = 0;
// 		while (j < (*array).w)
// 		{
// 			printf("%6d", (*array).z[i][j]);
// 			j++;
// 		}
// 		printf("\n");
// 		i++;
// 	}

	setup_window(ctrl);
	free_s_draw(ctrl);
	return (SUCCESS);
}
