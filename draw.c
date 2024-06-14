/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 13:59:13 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/05 00:39:08 by ggeorgie         ###   ########.fr       */
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
//	printf(" Z0: %3d\tto\tz1: %3d\twith color %lX and color1 %lX\n", (*coord).z0, (*coord).z1, (*coord).color, (*coord).color1);
//	printf("COLORP: %X, COLORN: %X\n", COLORP, COLORN);
//	printf("color: %X, z1: %i\n", COLOR0, (*coord).z1);
	if ((*coord).clr == COLOR0 && (((*coord).z0 >= 0 && (*coord).z1 > 0)
			|| ((*coord).z0 > 0 && (*coord).z1 >= 0)))
	{
//		printf("Sir, are you in P?\n");
		(*coord).clr = COLORP;
	}
	else if ((*coord).clr == COLOR0 && (((*coord).z0 <= 0 && (*coord).z1 < 0)
			|| ((*coord).z0 < 0 && (*coord).z1 <= 0)))
	{
//		printf("Sir, are you in N?\n");
		(*coord).clr = COLORN;
	}
//	printf("color: %lX\n", (*coord).color);
}

// <https://en.wikipedia.org/wiki/Absolute_value>
float	ft_abs(float *n)
{
	if (*n < 0)
		return (-*n);
	else
		return (*n);
}

float	ft_min(float m, float n)
{
	float	min;

//	printf("@ max: m: %f, n: %f\n", m, n);
	if (m < n)
		min = m;
	else
		min = n;
	return (min);
}

float	ft_max(float m, float n)
{
	float	max;

//	printf("@ max: m: %f, n: %f\n", m, n);
	if (m > n)
		max = m;
	else
		max = n;
	return (max);
}

// Limits map translation to reasonable values
void	limit_translation(struct s_draw *ctrl)
{
//	printf("before limit_translation: shift_x: %i, shift_y: %i\n", (*ctrl).shift_x, (*ctrl).shift_y);
	if ((*ctrl).shift_x < -(int)(*ctrl).w_w * 0.5)
	{
//		printf("before adjustment: shift_x: %i, (*ctrl).w_width: %i\n", (*ctrl).shift_x, (*ctrl).w_width);
		(*ctrl).shift_x = -(int)(*ctrl).w_w * 0.5;
//		printf(" after adjustment: shift_x: %i, (*ctrl).w_width: %i\n", (*ctrl).shift_x, (*ctrl).w_width);
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
//	printf(" after limit_translation: shift_x: %i, shift_y: %i\n", (*ctrl).shift_x, (*ctrl).shift_y);
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

	i_x = -1;																	// Top window border
	while (i_x++ < WIDTH - 1)
		mlx_put_pixel(img, i_x, 0, COLORP);
	i_y = -1;																	// Left window border
	while (i_y++ < HEIGHT - 1)
		mlx_put_pixel(img, 0, i_y, COLOR0);
	i_x = -1;																	// Bottom window border
	while (i_x++ < WIDTH - 1)
		mlx_put_pixel(img, i_x, HEIGHT - 1, COLORN);
	i_y = -1;																	// Right window border
	while (i_y++ < HEIGHT - 1)
		mlx_put_pixel(img, WIDTH - 1, i_y, COLOR0);
	i_x = -1;																	// Horizontal crosshair
	while (i_x++ < WIDTH - 1)
		mlx_put_pixel(img, i_x, HEIGHT / 2, 0X55555555);
	i_y = -1;																	// Vertical crosshair
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

// Changes the size of the map by generating a 'zoom_new' value.
void	zoom_map(struct s_draw *ctrl, mlx_key_data_t keydata)
{
	float	increment;

	printf("zoom: %f, key: %c\n", (*ctrl).zoom, keydata.key);
	increment = 1.025;
// 	if (mlx_is_key_down((*ctrl).mlx, MLX_KEY_EQUAL) || mlx_is_key_down((*ctrl).mlx, MLX_KEY_KP_ADD))
// 	{
// //		mlx_resize_image((*ctrl).img, (*ctrl).w_width * increment, (*ctrl).w_height * increment); // Resizes the whole image, not just the map.
// 	}
//	if (keydata.key == MLX_KEY_EQUAL || keydata.key == MLX_KEY_KP_ADD)
	if (keydata.key == MLX_KEY_KP_ADD)
	{
		(*ctrl).zoom_new = (*ctrl).zoom * increment;
	}
//	else if (keydata.key == MLX_KEY_MINUS || keydata.key == MLX_KEY_KP_SUBTRACT)
	else if (keydata.key == MLX_KEY_KP_SUBTRACT)
	{
		(*ctrl).zoom_new = (*ctrl).zoom / increment;
	}
	limit_zoom(&(*ctrl).zoom_new, ctrl);
//	(*((*ctrl).array)).width - 1 = ((*ctrl).array.width - 1) / (*ctrl).zoom;
//	ctrl->array->width - 1 = ((*ctrl).array->width - 1) / (*ctrl).zoom;
//	(*((*ctrl).array)).width - 1 = ((*((*ctrl).array)).width - 1) / (*ctrl).zoom;
//	(*ctrl).array->height;
//	(*((*ctrl).array)).height - 1;
	printf("zoom_new: %f\n", (*ctrl).zoom_new);
}

// void	scale_z(struct s_draw *ctrl, mlx_key_data_t keydata)
// {
// 	float	increment;

// //	printf("angle: %f degrees\n", (*ctrl).angle * (180.0 / PI));
// 	increment = 1.025;
// 	if (keydata.key == MLX_KEY_A)
// 	{
// 		(*coord).z0 = (*coord).z0 * increment;
// 		(*coord).z1 = (*coord).z1 * increment;
// 	}
// 	else if (keydata.key == MLX_KEY_Z)
// 	{
// 		(*coord).z0 = (*coord).z0 / increment;
// 		(*coord).z1 = (*coord).z1 / increment;
// 	}
// //	printf("angle: %f degrees\n", (*ctrl).angle * (180.0 / PI));
// }

// Translates the map by changing 'shift' values.
void	translate_map(struct s_draw *ctrl, mlx_key_data_t keydata)
{
	float	increment;

//	printf("before translate: shift_x: %i, shift_y: %i\n", (*ctrl).shift_x, (*ctrl).shift_y);
	increment = 10;
//	if (mlx_is_key_down((*ctrl).mlx, MLX_KEY_UP))
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
//	printf(" after translate: shift_x: %i, shift_y: %i\n", (*ctrl).shift_x, (*ctrl).shift_y);
}

// Rotates the map be changing the 'angle'.
void	rotate_map(struct s_draw *ctrl, mlx_key_data_t keydata)
{
	float	increment;

//	printf("angle: %f degrees\n", (*ctrl).angle * (180.0 / PI));
	increment = .025;
	if (keydata.key == MLX_KEY_E)
	{
		(*ctrl).angle = (*ctrl).angle - increment;
	}
	else if (keydata.key == MLX_KEY_D)
	{
		(*ctrl).angle = (*ctrl).angle + increment;
	}
//	printf("angle: %f degrees\n", (*ctrl).angle * (180.0 / PI));
}

// Manages user interactions, which do not require recentering the map.
void	hook_to_draw(mlx_key_data_t keydata, void *param)
{
	struct s_draw	*ctrl;

	ctrl = param;
	if (keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_LEFT
		|| keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_DOWN)			// Move the map up/down/left/right.
	{
		translate_map(ctrl, keydata);
	}
	if (keydata.key == MLX_KEY_E || keydata.key == MLX_KEY_D)					// Rotate the map up/down.		rotate_map(ctrl, keydata);
	{
		rotate_map(ctrl, keydata);
	}
	mlx_delete_image((*ctrl).mlx, (*ctrl).img);									// Free the previous image
	initialize_image(ctrl);
	printf("\nfrom hook/translate to draw_map\n");
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
//	if (keydata.key == MLX_KEY_EQUAL || keydata.key == MLX_KEY_KP_ADD || keydata.key == MLX_KEY_MINUS || keydata.key == MLX_KEY_KP_SUBTRACT)
	if (keydata.key == MLX_KEY_KP_ADD || keydata.key == MLX_KEY_KP_SUBTRACT)	// Zoom in/out. A line less of code.
	{
		zoom_map(ctrl, keydata);
//		printf("@ hook zoom: %f\n", (*ctrl).zoom);
		mlx_delete_image((*ctrl).mlx, (*ctrl).img);								// Free the previous image
		initialize_image(ctrl);
		printf("\nfrom hook/zoom to center_map\n");
		center_map(ctrl);
	}
	// if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_Z)					// Rotate the map up/down.
	// {
	// 	scale_z(ctrl, keydata);
	// 	mlx_delete_image((*ctrl).mlx, (*ctrl).img);								// Free the previous image
	// 	initialize_image(ctrl);
	// 	printf("\nfrom hook/scale_z to draw_map\n");
	// 	draw_map(ctrl);
	// }
	if (keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_LEFT
		|| keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_DOWN
		|| keydata.key == MLX_KEY_E || keydata.key == MLX_KEY_D)
	{
		hook_to_draw(keydata, param);
	}
	if (mlx_is_key_down((*ctrl).mlx, MLX_KEY_ESCAPE))
	{
		printf("ESC pressed\n");
		free_s_draw(ctrl);
		exit(SUCCESS);
	}
}

// void my_scrollhook(double xdelta, double ydelta, void* param)
// {
// 	(void)param;

// 	// Simple up or down detection.
// 	if (ydelta > 0)
// //		shift_y = shift_y - 10;
// 		puts("Up!");
// 	else if (ydelta < 0)
// 		puts("Down!");
	
// 	// Can also detect a mousewheel that go along the X (e.g: MX Master 3)
// 	if (xdelta < 0)
// 		puts("Sliiiide to the left!");
// 	else if (xdelta > 0)
// 		puts("Sliiiide to the right!");
// }

// void	resize_window(int32_t width, int32_t height, void *param)
// {
// 	struct s_draw	*ctrl;

// 	ctrl = param;
// 	if ((*ctrl).mlx)
// 		printf("mlx is not NULL\n");
// 	if ((*ctrl).img)
// 		printf("img is not NULL\n");
// 	printf("resize_window from: %i, %i\n", (*ctrl).w_width, (*ctrl).w_height);
// 	printf("1.resize_window to: %i, %i\n", width, height);
// 	(*ctrl).w_width = width;
// 	(*ctrl).w_height = height;
// //	printf("2.resize_window to: %i, %i\n", (*ctrl).w_width, (*ctrl).w_height);
// 	if ((*ctrl).mlx && (*ctrl).img)
// 	{
// //		printf("3.resize_window to: %i, %i\n", (*ctrl).w_width, (*ctrl).w_height);
// //		mlx_set_window_size((*ctrl).mlx, width, height);						// Resize the window - an alternative to 'mlx_delete_image' & 'mlx_new_image'? 
// //		mlx_resize_image((*ctrl).img, width, height);							// Resize the image - an alternative to 'mlx_delete_image' & 'mlx_new_image'? 
// 		mlx_delete_image((*ctrl).mlx, (*ctrl).img);								// Free the previous image
// 		(*ctrl).img = NULL;
// //		printf("4.resize_window to: %i, %i\n", (*ctrl).w_width, (*ctrl).w_height);
// //		mlx_delete_image((*ctrl).mlx, (*ctrl).img); // Free the previous image
// 		(*ctrl).img = NULL;														// Set the image pointer to NULL
// //		printf("5.resize_window to: %i, %i\n", (*ctrl).w_width, (*ctrl).w_height);
// 	}
// //	printf("6.resize_window to: %i, %i\n", (*ctrl).w_width, (*ctrl).w_height);
// 	initialize_image(ctrl);
// //	(*ctrl).img = mlx_new_image((*ctrl).mlx, width, height); // Create a new image
// 	// (*ctrl).img = mlx_new_image((*ctrl).mlx, (*ctrl).w_width, (*ctrl).w_height);
// 	// if (!(*ctrl).img || (mlx_image_to_window((*ctrl).mlx, (*ctrl).img, 0, 0) < 0))
// 	// {
// 	// 	/* error-handling code */
// 	// 	mlx_terminate((*ctrl).mlx);
// 	// 	drawing_error();
// 	// }
// 	printf("resized the window to: %i, %i\n", (*ctrl).w_width, (*ctrl).w_height);
// 	printf("\nfrom resize_window to center_map\n");
// 	center_map(ctrl);
// }

/* Converts 3D coordinates to 2D isometric projection.
 *	<https://www.tutorialspoint.com/c_standard_library/math_h.htm>
 *	<https://manpages.ubuntu.com/manpages/noble/en/man3/math.h.3avr.html>
 *	double cos(double);
 *	double sin(double);
 *	x' = (x - y) * cos(angle)
 *	y' = (x + y) * sin(angle) - z
 *	sin(angle) = cos(angle) = 0.7071 @ 45 degrees = .7853981634 radians
 */
void	convert_3d(struct s_draw *ctrl, struct s_line *coord)
{
	struct s_line	temp;

//	printf("x0: %f, y0: %f, z0: %i\n", (*coord).x0, (*coord).y0, (*coord).z0);
//	printf("x1: %f, y1: %f, z1: %i\n", (*coord).x1, (*coord).y1, (*coord).z1);
//	printf("1.1. draw a line from %3f, %3f, %3i\tto\t%3f, %3f, %3i\n", (*coord).x0, (*coord).y0, (*coord).z0, (*coord).x1, (*coord).y1, (*coord).z1);
//	printf("zoom: %f\n", (*ctrl).zoom);
	(temp).x0 = (*coord).x0 * (*ctrl).zoom;
	(temp).y0 = (*coord).y0 * (*ctrl).zoom;
	(temp).x1 = (*coord).x1 * (*ctrl).zoom;
	(temp).y1 = (*coord).y1 * (*ctrl).zoom;
//	printf("angle: %f, sin: %f, cos: %f\n", angle, sin(angle), cos(angle));
//	printf("1.2. draw a line from %3i, %3i, %3i\tto\t%3i, %3i, %3i\n", (int)(temp).x0, (int)(temp).y0, coord.z0, (int)(temp).x1, (int)(temp).y1, coord.z1);
	(*coord).x0 = ((temp).x0 - (temp).y0) * cos((*ctrl).angle);
	(*coord).y0 = ((temp).x0 + (temp).y0) * sin((*ctrl).angle) - (*coord).z0;
	(*coord).x1 = ((temp).x1 - (temp).y1) * cos((*ctrl).angle);
	(*coord).y1 = ((temp).x1 + (temp).y1) * sin((*ctrl).angle) - (*coord).z1;
//	printf("1.3. draw a line from %3f, %3f, %3i\tto\t%3f, %3f, %3i\n", (*coord).x0, (*coord).y0, (*coord).z0, (*coord).x1, (*coord).y1, (*coord).z1);
}

// Draws a line between two points using Bresenham's line algorithm
// <https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#All_cases>
// struct s_line *coord : Coordinates of the line to be drawn
void	draw_line(struct s_draw *ctrl, struct s_line *coord)
{
	float	max;

//	if (!coord.x0 || !coord.y0 || !coord.z0 || !coord.x1 || !coord.y1 || !coord.z1)		// Is this a good way to check if the coordinates are valid?
	if (!coord)
	{
		drawing_error(ctrl);
	}
//	printf("1. draw a line from %3i, %3i, %3i\tto\t%3i, %3i, %3i\n", (int)(*coord).x0, (int)(*coord).y0, (*coord).z0, (int)(*coord).x1, (int)(*coord).y1, (*coord).z1);
//	printf("\n\t\tfrom draw_ln to convert_3d\n");
	convert_3d(ctrl, coord);
//	printf("2. draw a line from %3i, %3i, %3i\tto\t%3i, %3i, %3i\n", (int)(*coord).x0, (int)(*coord).y0, (*coord).z0, (int)(*coord).x1, (int)(*coord).y1, (*coord).z1);
	(*coord).x0 = (*coord).x0 + (*ctrl).shift_x;
	(*coord).y0 = (*coord).y0 + (*ctrl).shift_y;
	(*coord).x1 = (*coord).x1 + (*ctrl).shift_x;
	(*coord).y1 = (*coord).y1 + (*ctrl).shift_y;
//	printf("3. draw a line from %12f, %12f, %3i\tto\t%12f, %12f, %3i\n", (*coord).x0, (*coord).y0, (*coord).z0, (*coord).x1, (*coord).y1, (*coord).z1);
	(*ctrl).step_x = (*coord).x1 - (*coord).x0;
	(*ctrl).step_y = (*coord).y1 - (*coord).y0;
//	printf("step_x: %f, step_y: %f\n", (*ctrl).step_x, (*ctrl).step_y);
	max = ft_max(ft_abs(&(*ctrl).step_x), ft_abs(&(*ctrl).step_y));
//	printf("max: %f\n", max);
	(*ctrl).step_x = (*ctrl).step_x / max;										// to ensure that difference between adjacent pixels is not more that 1 pixel
	(*ctrl).step_y = (*ctrl).step_y / max;										// to ensure that difference between adjacent pixels is not more that 1 pixel
	// printf("step_x: %d, step_y: %d\n", (int)step_x, (int)step_y);
	colorize_vertically(coord);
	while ((int)((*coord).x0 - (*coord).x1) || (int)((*coord).y0 - (*coord).y1))
	{
//		printf(" draw a line from %3f, %3f, %3d\tto\t%3f, %3f, %3d\twith color %lu and step %3f & %3f\n", (*coord).x0, (*coord).y0, (*coord).z0, (*coord).x1, (*coord).y1, (*coord).z1, (*coord).color, (ctrl).step_x, (ctrl).step_y);
		if ((*coord).x0 >= 0 && (*coord).x0 < WIDTH
			&& (*coord).y0 >= 0 && (*coord).y0 < HEIGHT)
			mlx_put_pixel((*ctrl).img, (*coord).x0, (*coord).y0, (*coord).clr);	// According to some, colors should gradually change with elevation change
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
	while (i_y++ <= (*((*ctrl).array)).h)										// Equivalent to 'while (i_y <= ctrl->array->height)'
	{
//		printf("\n   draw a line from  x0,  y0,  z0\tto\t x1,  y1,  z1\n");
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
	draw_border((*ctrl).img);
}

// Define iniial zoom and shift values
// struct s_line	coord: Coordinates of the extreme points of the map
//void	center_map_initial(struct s_draw *ctrl, struct s_line *coord)
void	center_map_initial(struct s_draw *ctrl)
{
	struct s_line	coord;
	float			zoom_width;
	float			zoom_height;

	printf("\n\t\t\t@ center_map_initial\n");
//	printf("larger raw zoom is better\n");
//	printf("window dimensions: %i, %i; zoom: %f\n", (*ctrl).w_width, (*ctrl).w_height, (*ctrl).zoom);
//	printf("%i, %i\n", (array).width, (array).height);
//	printf("zoom: %f\n", (*ctrl).zoom);

	coord = (struct s_line){0, (*((*ctrl).array)).h - 1, 0, 0,
		(*((*ctrl).array)).w - 1, 0, 0};										// Using 'z' values (e.g. '(array).z[(array).height - 1][(array).width - 1]') is not a good idea as it shifts the image base
	convert_3d(ctrl, &coord);
//	printf(" zoom_width: %f\nzoom_height: %f\n\n", ft_abs(&(coord).x1) - (coord).x0, ft_abs(&(coord).y1) - (coord).y0);
//	printf("  up-right 3D: x0: %f, y0: %f, x1: %f, y1: %f\n", (coord).x0, (coord).y0, (coord).x1, (coord).y1);
//	printf("x1: %f, x0: %f, y1: %f, y0: %f\n", (coord).x1, (coord).x0, (coord).y1, (coord).y0);

	zoom_width = (*ctrl).w_w / ft_max(ft_abs(&(coord).x1) - (coord).x0,
			ft_abs(&(coord).y1) - (coord).y0);
	limit_zoom(&zoom_width, ctrl);
	zoom_height = (*ctrl).w_h / ft_max(ft_abs(&(coord).x1) - (coord).x0,
			ft_abs(&(coord).y1) - (coord).y0);
	limit_zoom(&zoom_height, ctrl);
	// printf(" zoom_width: %f\n", zoom_width);
	// printf("zoom_height: %f\n", zoom_height);
	if (zoom_height < zoom_width)
		(*ctrl).zoom = zoom_height;
	else
		(*ctrl).zoom = zoom_width;
	printf("\t\t\tzoom: %f\n", (*ctrl).zoom);

// 	coord = (struct s_line){0, 0, 0, 0, (*((*ctrl).array)).width - 1, (*((*ctrl).array)).height - 1, 0, 0};	// Using 'z' values (e.g. '(array).z[(array).height - 1][(array).width - 1]') is not a good idea as it shifts the image base
// //	printf("2D: x0: %f, y0: %f, x1: %f, y1: %f\n", (coord).x0, (coord).y0, (coord).x1, (coord).y1);
// 	// (coord).x0 = (coord).x0 / (*ctrl).zoom;
// 	// (coord).y0 = (coord).y0 / (*ctrl).zoom;
// 	// (coord).x1 = (coord).x1 / (*ctrl).zoom;
// 	// (coord).y1 = (coord).y1 / (*ctrl).zoom;
// 	// (*ctrl).zoom = (*ctrl).zoom_new;
// //	printf("2D.2: x0: %f, y0: %f, x1: %f, y1: %f\n", (coord).x0, (coord).y0, (coord).x1, (coord).y1);
// //	printf("\n\t\tfrom cetner_drawing to convert_3d\n");
// 	convert_3d(ctrl, &coord);
// 	printf("down-right 3D: x0: %f, y0: %f, x1: %f, y1: %f\n", (coord).x0, (coord).y0, (coord).x1, (coord).y1);
// //	printf("down-right 3D: zoom_width: %f, zoom_height: %f\n", ft_max(ft_abs(&(coord).x1) - (coord).x0, ft_abs(&(coord).y1) - (coord).y0), ft_max(ft_abs(&(coord).x1) - (coord).x0, ft_abs(&(coord).y1) - (coord).y0));
// //	better: 

	(*ctrl).shift_x = ((*ctrl).w_w
			- ((coord).x1 + (coord).x0) * (*ctrl).zoom) / 2;
	(*ctrl).shift_y = ((*ctrl).w_h
			- ((coord).y1 + (coord).y0) * (*ctrl).zoom) / 2;
	printf("\t\t\tshift_x: %i, shift_y: %i\n", (*ctrl).shift_x, (*ctrl).shift_y);
//	printf("x0: %f, y0: %f, x1: %f, y1: %f\n", (*coord).x0 * (*ctrl).zoom, (*coord).y0 * (*ctrl).zoom, (*coord).x1 * (*ctrl).zoom, (*coord).y1 * (*ctrl).zoom);
	// (*coord) = (struct s_line){0, (array).height, 0, 0, (array).width, 0, 0, 0};	// Next few lines of code result is worse centering of the image.
	// (*coord) = (struct s_line){0, 0, 0, 0, (array).width, (array).height, 0, 0};
	// convert_3d(coord, ctrl);
	// printf("x0: %f, y0: %f, x1: %f, y1: %f\n", (*coord).x0, (*coord).y0, (*coord).x1, (*coord).y1);
	// (*ctrl).shift_y = ((*ctrl).w_height - (*coord).y1 + (*coord).y0) / 2;
	// if ((*ctrl).shift_y < 0)
	// {
	// 	(*ctrl).shift_y = 0;
	// }
	// if ((unsigned)(*ctrl).shift_y > (*ctrl).w_height)
	// {
	// 	(*ctrl).shift_y = (*ctrl).w_height;
	// }
	// printf("shift_x: %i, shift_y: %i\n", (*ctrl).shift_x, (*ctrl).shift_y);
//	if (coord)																	// It shouldn't be necessary to check if 'coord' is not NULL, because it was checked earlier.
//	fn_free((char *)&coord);													// This throws an error. Why?
}

// Center map on the image and envoke drawing function
void	center_map(struct s_draw *ctrl)
{
	int	vacant;

	printf("zoom: %f, zoom_new: %f\n", (*ctrl).zoom, (*ctrl).zoom_new);
	if ((*ctrl).zoom == (*ctrl).zoom_new && (*ctrl).zoom == 1)
	{
		center_map_initial(ctrl);
	}
//	else if ((*ctrl).zoom - (*ctrl).zoom_new > 0.1 || (*ctrl).zoom_new / (*ctrl).zoom > 0.1)
//	else if ((*ctrl).zoom / (*ctrl).zoom_new > 0.1)
	else
	{
		printf("shift x: %i, shift_y: %i\n", (*ctrl).shift_x, (*ctrl).shift_y);
//		printf("shift x: %i, w_width: %i\n", (*ctrl).shift_x, (*ctrl).w_width);
//		printf("shift y: %i, w_height: %i\n", (*ctrl).shift_y, (*ctrl).w_height);
//		(*ctrl).shift_x = (*ctrl).shift_x * (*ctrl).zoom / (*ctrl).zoom_new;
		vacant = (*ctrl).w_w / 2 - (*ctrl).shift_x;
		(*ctrl).shift_x = (*ctrl).w_w / 2
			- vacant * (*ctrl).zoom_new / (*ctrl).zoom;
		// (*ctrl).shift_x = (*ctrl).w_width / 2 - ((*ctrl).w_width / 2 - (*ctrl).shift_x) / (*ctrl).zoom * (*ctrl).zoom_new;
//		(*ctrl).shift_x = (*ctrl).w_width / 2 - ((*ctrl).shift_x - (*ctrl).w_width / 2) / (*ctrl).zoom * (*ctrl).zoom_new;
//		(*ctrl).shift_x = (*ctrl).w_width * (1 + 1 / (*ctrl).zoom * (*ctrl).zoom_new);											// This is buggy

		vacant = (*ctrl).w_h / 2 - (*ctrl).shift_y;
		(*ctrl).shift_y = (*ctrl).w_h / 2
			- vacant * (*ctrl).zoom_new / (*ctrl).zoom;
		// (*ctrl).shift_y = (*ctrl).w_height / 2 - ((*ctrl).w_height / 2 - (*ctrl).shift_y) * (*ctrl).zoom_new / (*ctrl).zoom;
		(*ctrl).zoom = (*ctrl).zoom_new;
		printf("\t\t\tshift_x: %i, shift_y: %i\n", (*ctrl).shift_x, (*ctrl).shift_y);
	}
	// else
	// {
	// 	printf("min/max zoom reached\n");
	// 	(*ctrl).zoom = (*ctrl).zoom * 2;
	// }
	limit_translation(ctrl);
//	printf("shift_x: %i, shift_y: %i\n", (ctrl).shift_x, (ctrl).shift_y);
	draw_map(ctrl);
}

// Create an image/background on which drawing will happen and check for errors
void	initialize_image(struct s_draw *ctrl)
{
//	printf("window width: %i & height: %i\n", (*ctrl).w_width, (*ctrl).w_height);	
	(*ctrl).img = mlx_new_image((*ctrl).mlx, (*ctrl).w_w, (*ctrl).w_h);
	if (!(*ctrl).img
		|| (mlx_image_to_window((*ctrl).mlx, (*ctrl).img, 0, 0) < 0))
	{
		/* error-handling code */
		// printf("failed to create image (MLX_WINFAIL?)\n");
		// mlx_terminate((*ctrl).mlx);
		drawing_error(ctrl);
	}
}

// Set up the window/drawing environment, invoke functions enabling drawing,
// as well as key hooks to control the drawn image, and finallly close cleanly.
// LeakSanitizer sees a problems with 'mlx_init',
// but this is happening inside the library code.
// "Summary: 7 leaks, 512 B lost."
//void	setup_window(char *file_name, struct s_draw *ctrl)
void	setup_window(struct s_draw *ctrl)
{
	(*ctrl).w_w = WIDTH;
	(*ctrl).w_h = HEIGHT;
	printf("window dimensions: %i * %i pixels.\n", (*ctrl).w_w, (*ctrl).w_h);
//	mlx_set_setting(MLX_MAXIMIZED, true);
//	mlx_set_setting((*ctrl).w_w, (*ctrl).w_h);
	(*ctrl).mlx = mlx_init((*ctrl).w_w, (*ctrl).w_h,
			"filet de fer / iron wire net", true);								// Window width, height, title, resizable
//	(ctrl).mlx = mlx_init((ctrl).w_w, (ctrl).w_h, file_name, true);				// Window width, height, title, resizable
	if (!(*ctrl).mlx)
	{
		// printf("failed to initialize MLX42\n");
		drawing_error(ctrl);
	}
	initialize_image(ctrl);
	(*ctrl).zoom = 1;
	(*ctrl).zoom_new = 1;
	(*ctrl).angle = ANGLE;
	printf("\nfrom setup_window to center_map\n");
	center_map(ctrl);
//	mlx_resize_hook((*ctrl).mlx, &resize_window, ctrl);							// The last variable should be the one to which 'param' is assigned.
//	mlx_loop_hook((ctrl).mlx, &hook, (ctrl).mlx);
	mlx_key_hook((*ctrl).mlx, &hook, ctrl);
//	mlx_scroll_hook(mlx, &my_scrollhook, NULL);
	mlx_close_hook((*ctrl).mlx, &closefunc, ctrl);
	mlx_loop((*ctrl).mlx);
	mlx_delete_image((*ctrl).mlx, (*ctrl).img);
	mlx_terminate((*ctrl).mlx);
}
