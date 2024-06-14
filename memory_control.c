/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 03:37:03 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/02 22:33:03 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Frees the memory allocated for a double pointer and the pointers it points
 * to. The triple pointer is needed for modification of the original pointer.
 * @param	char	***ptr: a pointer to pointers.
 */
void	fn_free_char_ptr(char ***ptr)
{
	int	i;

	if (*ptr)
	{
		i = 0;
		while (*ptr[i])
		{
			fn_free(&(*ptr)[i]);
			i++;
		}
		fn_free(*ptr);
	}
}

/**
 * Frees each sub-array of array and the array itself
 * @param	int	***array: a pointer to a 2D array of long unsigned integers,
 * 						which is to be freed.
 * @param	int	rows: the number of rows in the array.
 * @param	int	i: the index of the sub-array to be freed.
 */
void	free_lu_array(unsigned long ***array, int rows)
{
	int	i;

	i = 0;
	if (*array && **array && rows > 0)
	{
		while (i < rows)
		{
			fn_free((char **)array[i]);
			i++;
		}
		fn_free((char **)&array);
	}
}

/**
 * Frees each sub-array of array and the array itself
 * @param	int	***array: a pointer to a 2D array of integers to be freed.
 * @param	int	rows: the number of rows in the array.
 * @param	int	i: the index of the sub-array to be freed.
 */
void	free_int_array(int ***array, int rows)
{
	int	i;

	i = 0;
	if (*array && **array && rows > 0)
	{
		while (i < rows)
		{
			fn_free((char **)array[i]);
			i++;
		}
		fn_free((char **)&array);
	}
}

/**
 * Frees an 's_draw' type of structure, including all its elements,
										particularly 's_fdf' type of structure.
 */
void	free_s_draw(struct s_draw *an_s_draw)
{
	int	i;

	if ((*an_s_draw).img && (*an_s_draw).mlx)
		mlx_delete_image((*an_s_draw).mlx, (*an_s_draw).img);
	if ((*an_s_draw).mlx)
		mlx_terminate((*an_s_draw).mlx);
	if (an_s_draw)
	{
		if ((*an_s_draw).array)
		{
			i = 0;
			while (i < (*(*an_s_draw).array).h)
			{
				if ((*(*an_s_draw).array).z[i])
					fn_free((char **)&(*(*an_s_draw).array).z[i]);
				if ((*(*an_s_draw).array).clr[i])
					fn_free((char **)&(*(*an_s_draw).array).clr[i]);
				i++;
			}
			fn_free((char **)&(*(*an_s_draw).array).z);
			fn_free((char **)&(*(*an_s_draw).array).clr);
			fn_free((char **)&(*an_s_draw).array);
		}
		fn_free((char **)&an_s_draw);
	}
}

// Frees an 's_draw' type of struct and exits the program with FAILURE status.
void	free_s_draw_and_exit(struct s_draw *an_s_draw)
{
	free_s_draw(an_s_draw);
	exit (FAILURE);
}
