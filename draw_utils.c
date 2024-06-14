/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 20:17:41 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/05 00:49:58 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Limits map translation to reasonable values
void	limit_translation(struct s_draw *ctrl)
{
	if ((*ctrl).shift_x < -(int)(*ctrl).w_w * 0.5)
	{
		(*ctrl).shift_x = -(int)(*ctrl).w_w * 0.5;
	}
	if ((*ctrl).shift_x > (int)(*ctrl).w_w * 1.5)
	{
		(*ctrl).shift_x = (*ctrl).w_w * 1.5;
	}
	if ((*ctrl).shift_y < -(int)(*ctrl).w_h)
	{
		(*ctrl).shift_y = -(*ctrl).w_h;
	}
	if ((*ctrl).shift_y > (int)(*ctrl).w_h)
	{
		(*ctrl).shift_y = (*ctrl).w_h;
	}
}

// Limits map zooming to reasonable values
void	limit_zoom(float *zoom, struct s_draw *ctrl)
{
	if (*zoom < 1.2)
	{
		*zoom = 1.2;
	}
	if (*zoom > (*ctrl).w_w || *zoom > (*ctrl).w_h)
	{
		*zoom = ft_min((*ctrl).w_w, (*ctrl).w_h);
	}
}

// Draws a border around the image and crosshairs in the center
void	draw_border(mlx_image_t *img)
{
	int		i_x;
	int		i_y;

	i_x = -1;
	while (i_x++ < WIDTH - 1)
		mlx_put_pixel(img, i_x, 0, COLORP);
	i_y = -1;
	while (i_y++ < HEIGHT - 1)
		mlx_put_pixel(img, 0, i_y, COLOR0);
	i_x = -1;
	while (i_x++ < WIDTH - 1)
		mlx_put_pixel(img, i_x, HEIGHT - 1, COLORN);
	i_y = -1;
	while (i_y++ < HEIGHT - 1)
		mlx_put_pixel(img, WIDTH - 1, i_y, COLOR0);
	i_x = -1;
	while (i_x++ < WIDTH - 1)
		mlx_put_pixel(img, i_x, HEIGHT / 2, 0X55555555);
	i_y = -1;
	while (i_y++ < HEIGHT - 1)
		mlx_put_pixel(img, WIDTH / 2, i_y, 0X55555555);
}

/* MLX error handling function */
void	drawing_error(struct s_draw *ctrl)
{
	write(2, mlx_strerror(mlx_errno), ft_strlen(mlx_strerror(mlx_errno)));
	write(2, "\n", 1);
	free_s_draw_and_exit(ctrl);
}

/* Handles clicking on the cross on the window’s frame */
void	closefunc(void *param)
{
	struct s_draw	*ctrl;

	ctrl = param;
	free_s_draw_and_exit(ctrl);
}
