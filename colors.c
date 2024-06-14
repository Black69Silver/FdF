/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 00:14:15 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/04 21:54:56 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_current_digit(char *original, int i, int len)
{
	int		temp;

//	printf("original[%zu]: %c\n", len - i, original[len - i]);
	if (original[len - i] >= '0' && original[len - i] <= '9')
		temp = original[len - i] - '0';
	if (original[len - i] >= 'A' && original[len - i] <= 'Z')
		temp = original[len - i] - 'A' + 10;
	if (original[len - i] >= 'a' && original[len - i] <= 'z')
		temp = original[len - i] - 'a' + 10;
	return (temp);
}

static const int	g_hex = 16;

/* Converts a number received as ascii string in different bases:
 *	- hexadecimal (both upper case and lower case) to decimal,
 * Converts colors according to the following rules:
 * 	fdf input	=	MiniLibX	=	MLX42
 * 					0xAARRGGBB	=	0xRRGGBBAA
 * 	0x12345678	=	0x12345678	=	0x34567812
 * 	0x123456	=	0xFF123456	=	0x123456FF
 * 	0x1234		=	0x11223344	=	0x22334411
 * 	0x12		=	0x12FFFFFF	=	0xFFFFFF12
 * 	Missing channels are assumed to be have value FF
 */
//unsigned long	ft_hex_2_dec(char *original)
unsigned long	ft_hex_2_dec(struct s_draw *ctrl,
							char **ascii_number, char *original)
{
	int				i;
	int				len;
	unsigned long	power;
	unsigned long	converted;
	int				temp;

//	printf("\nascii string: %s\n", original);
	i = 1;
	len = ft_strlen(original);
//	printf("len: %i\n", len);
	power = 1;
	converted = 0;
	while (i <= len)
	{
//		printf("\ni: %i\n", i);
//		printf("power: %lu\n", power);
		temp = ft_current_digit(original, i, len);
		if (temp < 0 || temp > g_hex - 1)
//			return (-1);
			free_ascii(ctrl, ascii_number);
//		printf("temp: %i\n", temp);
		converted = converted + temp * power;
		if (len == 4)
			converted = converted + temp * power * (power * g_hex + power - 1);
//		printf("temp converted dec: %lu\n", converted);
//		printf("temp converted hex: %lX\n", converted);
		power = power * g_hex;
		i++;
	}
	if (len == 8 || len == 4)													// Shift the alpha channel to the end
	{
		temp = converted / 0x1000000;											// 16,777,216 = 0x1000000 to get the alpha channel
		converted = converted % 0x1000000;										// 16,777,216 = 0x1000000 to get the rest of the number
		converted = converted * 0x100 + temp;									// 256 = 0x100 to shift the number 2 positions eft and add the alpha channel to the end
	}
	else if (len == 6)															// Adding 'FF' for the alpha channel
	{
			converted = (converted + 1) * 0x100 - 1;
	}
	else if (len == 2)															// Adding 'FF' for the RR, GG, and BB channels
		{
			converted = converted + 0xFFFFFF00;									// 4294967040 = 0xFFFFFF00 and save 8 lines of code
		}	
//	printf("converted hex: %8lX\n", converted);
	return (converted);
}

/* Return a pointer of the first occurrence of 'c'
 * in the string pointed to by 'str', or, if not found, NULL.
 */
// char	*ft_strchr(char *str, int c)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i] && str[i] != c)
// 		i++;
// 	if (str[i] == c)
// 		return (str[i]);
// 	else
// 		return (NULL);
// }

/*			if (!is_valid(row))
				;

static const char	g_valid_char[] = "-0123456789AaBbCcDdEeFfXx, ";
*/
//		printf("Sir, are you in?\n");

/* Set in 'array' a color for each "number", either by extracting it
 * from 'ascii_number' or by assigning a default value.
 * Simultaneously, remove information about color from 'ascii_number'.
 */
//void	extract_colors(struct s_fdf *array, char **ascii_number, int i_row)
void	extract_colors(struct s_draw *ctrl, char **ascii_number, int i_row)
{
	int		j;
	char	*comma;

	j = 0;
//	while (j < (*array).width)
	while (j < (*((*ctrl).array)).w)
	{
//		printf("extract_colors start: ascii_number[%i]: %s\n", j, ascii_number[j]);
		comma = ft_strchr(ascii_number[j], ',');
//		printf("%p\n", comma);
		if (comma && comma[1] == '0' && (comma[2] == 'x' || comma[2] == 'X'))
		{
//			(*array).color[i_row][j] = ft_hex_2_dec(comma + 3);					// '+ 3' to skip the ',0x' or ',0X'
			(*((*ctrl).array)).clr[i_row][j]
				= ft_hex_2_dec(ctrl, ascii_number, comma + 3);
//			printf("@ extract_colors [%i][%i] color %lu\n", i_row, j, (*array).color[i_row][j]);
			*comma = '\0';														// To trim the string to the number only.
		}
		else
		{
//			printf("i_row: %i, j: %i, COLOR0: %i\n", i_row, j, COLOR0);
//			printf("color %lu\n", (*array).color[0][0]);
//					(*array).color[i_row][j] = COLOR0;
			(*((*ctrl).array)).clr[i_row][j] = COLOR0;
//			printf("color %lu\n", (*array).color[i_row][j]);
		}
//		printf("@ extract_colors end: ascii_number[%i]: %s\n", j, ascii_number[j]);
//		printf("@ extract_colors [%i][%i] color %lu\n", i_row, j, (*array).color[i_row][j]);
		j++;
	}
//	(*array).color[i_row][j] = '\0';											// This is not needed, as 'color' is an array of integers, not strings.
}
