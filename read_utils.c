/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 00:53:12 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/04 22:51:25 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/* Error handling function */
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
 * @param	struct s_fdf *array: to be freed in case of nbr beyond int limits.
 * @return	nbr: The signed integer value of the evaluated number/substring.
 */
int	fdf_atoi(struct s_draw *ctrl, char **ascii_numbers, int j)
{
	signed long long	nbr;
	int					sign;
	int					i;

	nbr = 0;
	sign = 1;
	i = 0;
	if (ascii_numbers[j][i] == '-' || ascii_numbers[j][i] == '+')
	{
		if (ascii_numbers[j][i] == '-')
			sign = -sign;
		i++;
	}
	while (ascii_numbers[j][i])
	{
		if (ascii_numbers[j][i] < '0' || ascii_numbers[j][i] > '9')
			free_ascii(ctrl, ascii_numbers);
		nbr = nbr * 10 + (ascii_numbers[j][i] - '0');
		if (nbr * sign < Z_MIN || nbr * sign > Z_MAX)
			free_ascii(ctrl, ascii_numbers);
		i++;
	}
	return (nbr * sign);
}

// Initializes a 2D array of long unsigned integers with given height and width.
unsigned long	**initialize_lu_array(int height, int width)
{
	unsigned long	**array;
	int				i_y;

	array = ft_calloc(height, sizeof(unsigned long *));
	if (!array)
	{
		return (NULL);
	}
	i_y = 0;
	while (i_y < height)
	{
		array[i_y] = ft_calloc(width, sizeof(unsigned long));
		if (!array[i_y])
		{
			free_lu_array(&array, i_y);
			return (NULL);
		}
		i_y++;
	}
	return (array);
}

// Initializes a 2D array of integers with given height and width.
int	**initialize_int_array(int height, int width)
{
	int		**array;
	int		i_y;

	array = ft_calloc(height, sizeof(int *));
	if (!array)
	{
		return (NULL);
	}
	i_y = 0;
	while (i_y < height)
	{
		array[i_y] = ft_calloc(width, sizeof(int));
		if (!array[i_y])
		{
			free_int_array(&array, i_y);
			return (NULL);
		}
		i_y++;
	}
	return (array);
}
