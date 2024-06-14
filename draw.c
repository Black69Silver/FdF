/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 13:59:13 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/02 20:55:03 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* If necessary to install MLX42:
	- git clone https://github.com/codam-coding-college/MLX42.git MLX42
	- cd MLX42
	- cmake -B build
	- cmake --build build -j4
	- adjust LIBMLX path in Makefile
	<https://github.com/codam-coding-college/MLX42>
	## Maybe necessary dependencies
	- [CMake: >= 3.18.0](https://cmake.org/download/)
	- [GLFW: >= 3.3.6](https://github.com/glfw/glfw)
				& <https://www.glfw.org/docs/3.3/compile.html>
	- 'brew install glfw'
	
	Control commands from keyboard:
	- ESC / MLX_KEY_ESCAPE: close the window and quit the program in a cleanly.
	- - / MLX_KEY_MINUS / MLX_KEY_KP_SUBTRACT: zoom out.
	- + / MLX_KEY_EQUAL / MLX_KEY_KP_ADD: zoom in.

	- MLX_KEY_UP: move the image up.
	- MLX_KEY_LEFT: move the image left.
	- MLX_KEY_RIGHT: move the image right.
	- MLX_KEY_DOWN: move the image down.
	
	- E / MLX_KEY_E: while looking from the side, reduce viewing angle.
	- D / MLX_KEY_D: while looking from the side, increase viewing angle.
	
	~ A / MLX_KEY_A: scale up 'z' coordinate.
	~ Z / MLX_KEY_Z: scale down 'z' coordinate.
 */

#include "fdf.h"

/** 
 * Lines with height of at least 0 are drawn red. 
 * Lines with height of at most 0 are drawn blue.
*/
void	colorize_vertically(struct s_line *coord)
{
	if ((*coord).clr == COLOR0 && (((*coord).z0 >= 0 && (*coord).z1 > 0)
			|| ((*coord).z0 > 0 && (*coord).z1 >= 0)))
	{
		(*coord).clr = COLORP;
	}
	else if ((*coord).clr == COLOR0 && (((*coord).z0 <= 0 && (*coord).z1 < 0)
			|| ((*coord).z0 < 0 && (*coord).z1 <= 0)))
	{
		(*coord).clr = COLORN;
	}
}

/* Converts 3D coordinates to 2D isometric projection.
 *	sin(angle) = cos(angle) = 0.7071 @ 45 degrees = .7853981634 radians
 */
void	convert_3d(struct s_draw *ctrl, struct s_line *coord)
{
	struct s_line	temp;

	(temp).x0 = (*coord).x0 * (*ctrl).zoom;
	(temp).y0 = (*coord).y0 * (*ctrl).zoom;
	(temp).x1 = (*coord).x1 * (*ctrl).zoom;
	(temp).y1 = (*coord).y1 * (*ctrl).zoom;
	(*coord).x0 = ((temp).x0 - (temp).y0) * cos((*ctrl).angle);
	(*coord).y0 = ((temp).x0 + (temp).y0) * sin((*ctrl).angle) - (*coord).z0;
	(*coord).x1 = ((temp).x1 - (temp).y1) * cos((*ctrl).angle);
	(*coord).y1 = ((temp).x1 + (temp).y1) * sin((*ctrl).angle) - (*coord).z1;
}

// Draws a line between two points using Bresenham's line algorithm
// <https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#All_cases>
// struct s_line *coord : Coordinates of the line to be drawn
void	draw_line(struct s_draw *ctrl, struct s_line *coord)
{
	float	max;

	if (!coord)
		drawing_error(ctrl);
	convert_3d(ctrl, coord);
	(*coord).x0 = (*coord).x0 + (*ctrl).shift_x;
	(*coord).y0 = (*coord).y0 + (*ctrl).shift_y;
	(*coord).x1 = (*coord).x1 + (*ctrl).shift_x;
	(*coord).y1 = (*coord).y1 + (*ctrl).shift_y;
	(*ctrl).step_x = (*coord).x1 - (*coord).x0;
	(*ctrl).step_y = (*coord).y1 - (*coord).y0;
	max = ft_max(ft_abs(&(*ctrl).step_x), ft_abs(&(*ctrl).step_y));
	(*ctrl).step_x = (*ctrl).step_x / max;
	(*ctrl).step_y = (*ctrl).step_y / max;
	colorize_vertically(coord);
	while ((int)((*coord).x0 - (*coord).x1) || (int)((*coord).y0 - (*coord).y1))
	{
		if ((*coord).x0 >= 0 && (*coord).x0 < WIDTH
			&& (*coord).y0 >= 0 && (*coord).y0 < HEIGHT)
			mlx_put_pixel((*ctrl).img, (*coord).x0, (*coord).y0, (*coord).clr);
		(*coord).x0 = (*coord).x0 + (*ctrl).step_x;
		(*coord).y0 = (*coord).y0 + (*ctrl).step_y;
	}
}

// Sends coordinates of current point and the next points
// - to the right and down - to 'draw_line'
void	draw_map(struct s_draw *ctrl)
{
	int	i_x;
	int	i_y;

	i_y = -1;
	while (i_y++ <= (*((*ctrl).array)).h)
	{
		i_x = -1;
		while (i_x++ <= (*((*ctrl).array)).w)
		{
			if (i_x < (*((*ctrl).array)).w - 1 && i_y < (*((*ctrl).array)).h)
			{
				draw_line(ctrl, &(struct s_line){i_x, i_y,
					(*((*ctrl).array)).z[i_y][i_x],
					(*((*ctrl).array)).clr[i_y][i_x],
					i_x + 1, i_y, (*((*ctrl).array)).z[i_y][i_x + 1]});
			}
			if (i_y < (*((*ctrl).array)).h - 1 && i_x < (*((*ctrl).array)).w)
			{
				draw_line(ctrl, &(struct s_line){i_x, i_y,
					(*((*ctrl).array)).z[i_y][i_x],
					(*((*ctrl).array)).clr[i_y][i_x],
					i_x, i_y + 1, (*((*ctrl).array)).z[i_y + 1][i_x]});
			}
		}
	}
}
