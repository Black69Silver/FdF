/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_drawing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 21:20:17 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/05 00:32:51 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Define iniial zoom and shift values
// struct s_line	coord: Coordinates of the extreme points of the map
void	center_map_initial(struct s_draw *ctrl)
{
	struct s_line	coord;
	float			zoom_width;
	float			zoom_height;

	coord = (struct s_line){0, (*((*ctrl).array)).h - 1, 0, 0,
		(*((*ctrl).array)).w - 1, 0, 0};
	convert_3d(ctrl, &coord);
	zoom_width = (*ctrl).w_w / ft_max(ft_abs(&(coord).x1) - (coord).x0,
			ft_abs(&(coord).y1) - (coord).y0);
	limit_zoom(&zoom_width, ctrl);
	zoom_height = (*ctrl).w_h / ft_max(ft_abs(&(coord).x1) - (coord).x0,
			ft_abs(&(coord).y1) - (coord).y0);
	limit_zoom(&zoom_height, ctrl);
	if (zoom_height < zoom_width)
		(*ctrl).zoom = zoom_height;
	else
		(*ctrl).zoom = zoom_width;
	(*ctrl).shift_x = ((*ctrl).w_w
			- ((coord).x1 + (coord).x0) * (*ctrl).zoom) / 2;
	(*ctrl).shift_y = ((*ctrl).w_h
			- ((coord).y1 + (coord).y0) * (*ctrl).zoom) / 2;
}

// Center map on the image and envoke drawing function
void	center_map(struct s_draw *ctrl)
{
	int	vacant;

	if ((*ctrl).zoom == (*ctrl).zoom_new && (*ctrl).zoom == 1)
	{
		center_map_initial(ctrl);
	}
	else
	{
		vacant = (*ctrl).w_w / 2 - (*ctrl).shift_x;
		(*ctrl).shift_x = (*ctrl).w_w / 2
			- vacant * (*ctrl).zoom_new / (*ctrl).zoom;
		vacant = (*ctrl).w_h / 2 - (*ctrl).shift_y;
		(*ctrl).shift_y = (*ctrl).w_h / 2
			- vacant * (*ctrl).zoom_new / (*ctrl).zoom;
		(*ctrl).zoom = (*ctrl).zoom_new;
	}
	limit_translation(ctrl);
	draw_map(ctrl);
}

// Create an image/background on which drawing will happen and check for errors
void	initialize_image(struct s_draw *ctrl)
{
	(*ctrl).img = mlx_new_image((*ctrl).mlx, (*ctrl).w_w, (*ctrl).w_h);
	if (!(*ctrl).img
		|| (mlx_image_to_window((*ctrl).mlx, (*ctrl).img, 0, 0) < 0))
	{
		drawing_error(ctrl);
	}
}

// Set up the window/drawing environment, invoke functions enabling drawing,
// as well as key hooks to control the drawn image, and finallly close cleanly.
// LeakSanitizer sees a problems with 'mlx_init',
// but this is happening inside the library code.
// "Summary: 7 leaks, 512 B lost."
void	setup_window(struct s_draw *ctrl)
{
	(*ctrl).w_w = WIDTH;
	(*ctrl).w_h = HEIGHT;
	(*ctrl).mlx = mlx_init((*ctrl).w_w, (*ctrl).w_h,
			"filet de fer / iron wire net", true);
	if (!(*ctrl).mlx)
		drawing_error(ctrl);
	initialize_image(ctrl);
	(*ctrl).zoom = 1;
	(*ctrl).zoom_new = 1;
	(*ctrl).angle = ANGLE;
	center_map(ctrl);
	mlx_key_hook((*ctrl).mlx, &hook, ctrl);
	mlx_close_hook((*ctrl).mlx, &closefunc, ctrl);
	mlx_loop((*ctrl).mlx);
	mlx_delete_image((*ctrl).mlx, (*ctrl).img);
	mlx_terminate((*ctrl).mlx);
}
