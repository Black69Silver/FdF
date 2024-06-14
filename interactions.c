/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 20:22:11 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/02 20:33:44 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Changes the size of the map by generating a 'zoom_new' value.
void	zoom_map(struct s_draw *ctrl, mlx_key_data_t keydata)
{
	float	increment;

	increment = 1.025;
	if (keydata.key == MLX_KEY_KP_ADD)
	{
		(*ctrl).zoom_new = (*ctrl).zoom * increment;
	}
	else if (keydata.key == MLX_KEY_KP_SUBTRACT)
	{
		(*ctrl).zoom_new = (*ctrl).zoom / increment;
	}
	limit_zoom(&(*ctrl).zoom_new, ctrl);
}

// Translates the map by changing 'shift' values.
void	translate_map(struct s_draw *ctrl, mlx_key_data_t keydata)
{
	float	increment;

	increment = 10;
	if (keydata.key == MLX_KEY_UP)
	{
		(*ctrl).shift_y = (*ctrl).shift_y - increment;
	}
	else if (keydata.key == MLX_KEY_LEFT)
	{
		(*ctrl).shift_x = (*ctrl).shift_x - increment;
	}
	else if (keydata.key == MLX_KEY_RIGHT)
	{
		(*ctrl).shift_x = (*ctrl).shift_x + increment;
	}
	else if (keydata.key == MLX_KEY_DOWN)
	{
		(*ctrl).shift_y = (*ctrl).shift_y + increment;
	}
	limit_translation(ctrl);
}

// Rotates the map be changing the 'angle'.
void	rotate_map(struct s_draw *ctrl, mlx_key_data_t keydata)
{
	float	increment;

	increment = .025;
	if (keydata.key == MLX_KEY_E)
	{
		(*ctrl).angle = (*ctrl).angle - increment;
	}
	else if (keydata.key == MLX_KEY_D)
	{
		(*ctrl).angle = (*ctrl).angle + increment;
	}
}

// Manages user interactions, which do not require recentering the map.
void	hook_to_draw(mlx_key_data_t keydata, void *param)
{
	struct s_draw	*ctrl;

	ctrl = param;
	if (keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_LEFT
		|| keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_DOWN)
	{
		translate_map(ctrl, keydata);
	}
	if (keydata.key == MLX_KEY_E || keydata.key == MLX_KEY_D)
	{
		rotate_map(ctrl, keydata);
	}
	mlx_delete_image((*ctrl).mlx, (*ctrl).img);
	initialize_image(ctrl);
	draw_map(ctrl);
}

/* A dispatcher function that will be called every time a key is pressed.
 * Pressing ESC must close the window and quit the program in a clean way.
 * <https://github.com/codam-coding-college/MLX42/wiki/Images>
 */
void	hook(mlx_key_data_t keydata, void *param)
{
	struct s_draw	*ctrl;

	ctrl = param;
	if (keydata.key == MLX_KEY_KP_ADD || keydata.key == MLX_KEY_KP_SUBTRACT)
	{
		zoom_map(ctrl, keydata);
		mlx_delete_image((*ctrl).mlx, (*ctrl).img);
		initialize_image(ctrl);
		center_map(ctrl);
	}
	if (keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_LEFT
		|| keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_DOWN
		|| keydata.key == MLX_KEY_E || keydata.key == MLX_KEY_D)
	{
		hook_to_draw(keydata, param);
	}
	if (mlx_is_key_down((*ctrl).mlx, MLX_KEY_ESCAPE))
	{
		free_s_draw(ctrl);
		exit(SUCCESS);
	}
}
