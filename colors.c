/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 00:53:12 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/04 23:08:59 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/* Converts colors according to the following rules:
 * 	fdf input	=	MiniLibX	=	MLX42
 * 					0xAARRGGBB	=	0xRRGGBBAA
 * 	0x12345678	=	0x12345678	=	0x34567812
 * 	0x123456	=	0xFF123456	=	0x123456FF
 * 	0x1234		=	0x11223344	=	0x22334411
 * 	0x12		=	0x12FFFFFF	=	0xFFFFFF12
 * Missing channels are assumed to be have value FF
 * i.e.
 * For 4- and 8-digit hex numbers, the alpha channel is shifted to the end.
 * For 6-digit hex numbers, FF is added for alpha channel at the end.
 * For 2-digit hex nbrs, FF is added for RR, GG, BB channels at the beginning.
 * <https://www.rapidtables.com/convert/number/decimal-to-hex.html>
 */
int	harmonize_color_length(int len, unsigned long converted)
{
	int	temp;

	if (len == 8 || len == 4)
	{
		temp = converted / 0x1000000;
		converted = converted % 0x1000000;
		converted = converted * 0x100 + temp;
	}
	else if (len == 6)
		converted = (converted + 1) * 0x100 - 1;
	else if (len == 2)
		converted = converted + 0xFFFFFF00;
	return (converted);
}

/* Converts a current digit from ASCII character to a decimal integer.
 */
int	ft_current_digit(char *original, int i, int len)
{
	int	temp;

	if (original[len - i] >= '0' && original[len - i] <= '9')
		temp = original[len - i] - '0';
	if (original[len - i] >= 'A' && original[len - i] <= 'F')
		temp = original[len - i] - 'A' + 10;
	if (original[len - i] >= 'a' && original[len - i] <= 'f')
		temp = original[len - i] - 'a' + 10;
	return (temp);
}

static const int	g_hex = 16;

/* Converts a number received as ascii string in different bases:
 *	- hexadecimal (both upper case and lower case) to decimal.
 */
unsigned long	ft_hex_2_dec(struct s_draw *ctrl,
							char **ascii_number, char *original)
{
	int				i;
	int				len;
	unsigned long	power;
	unsigned long	converted;
	int				temp;

	i = 1;
	len = ft_strlen(original);
	power = 1;
	converted = 0;
	while (i <= len)
	{
		temp = ft_current_digit(original, i, len);
		if (temp < 0 || temp > g_hex - 1)
			free_ascii(ctrl, ascii_number);
		converted = converted + temp * power;
		if (len == 4)
			converted = converted + temp * power * (power * g_hex + power - 1);
		power = power * g_hex;
		i++;
	}
	converted = harmonize_color_length(len, converted);
	return (converted);
}

/* Set in 'array' a color for each "number", either by extracting it
 * from 'ascii_number' or by assigning a default value.
 * Simultaneously, remove information about color from 'ascii_number'.
 */
void	extract_colors(struct s_draw *ctrl, char **ascii_number, int i_row)
{
	int		j;
	char	*comma;

	j = 0;
	while (j < (*((*ctrl).array)).w)
	{
		comma = ft_strchr(ascii_number[j], ',');
		if (comma && comma[1] == '0' && (comma[2] == 'x' || comma[2] == 'X'))
		{
			(*((*ctrl).array)).clr[i_row][j]
				= ft_hex_2_dec(ctrl, ascii_number, comma + 3);
			*comma = '\0';
		}
		else
		{
			(*((*ctrl).array)).clr[i_row][j] = COLOR0;
		}
		j++;
	}
}
