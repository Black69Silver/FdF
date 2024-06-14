/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 23:46:06 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/05 00:30:09 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <unistd.h>	/* to use 'close', 'read', 'write' */
# include <stdlib.h>	/* to use 'exit', 'free', 'malloc' */
# include <fcntl.h>		/* to use 'open' */ 
# include <limits.h>	/* to use INT_MIN and INT_MAX */						// It's already included in get_next_line - redundant headers are not a problem as 'include' guards are used
# include <math.h>		/* to use various mathematical functions */
# include <stdio.h>		/* to use 'print' */									// comment out before submission
# include "MLX42/MLX42.h"
# include "get_next_line_bonus.h"

# ifndef WIDTH
#  define WIDTH 2000
# endif
# ifndef HEIGHT
#  define HEIGHT 1000
# endif

# define Z_MIN -30000
# define Z_MAX 30000
# define COLORP 0XFF5500FF
# define COLOR0 0X55FFAAFF
# define COLORN 0X0055FFFF
# define ANGLE .7853981634								// 45 degrees in radians
# define PI 3.14159265358979323846
# define FAILURE -1
# define SUCCESS 0

// Structure which stores input data
struct s_fdf											// array
{
	int				w;
	int				h;
	int				**z;
	unsigned long	**clr;
};

// Structure which stores the coordinates of the line to be drawn
struct s_line											// coord
{
	float			x0;
	float			y0;
	int				z0;
	unsigned long	clr;
	float			x1;
	float			y1;
	int				z1;
};

// mlx_t			*mlx : The instance/window in which the drawing takes place
// mlx_image_t		*img : The background on which drawing takes place
// w_w				: The width of the window
// w_h				: The height of the window
// shift_x			: The horizontal map shift, so that it is centered
// shift_y			: The vertical map shift, so that it is centered
// zoom				: The zoom factor of the map
// zoom_new			: New zoom factor of the map, when requested by the user
// step_x			: The step with which the map is drawn horizontally
// step_y			: The step with which the map is drawn vertically
// angle			: The angle of the isometric projection 
struct s_draw											// ctrl
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	struct s_fdf	*array;
	unsigned int	w_w;
	unsigned int	w_h;
	int				shift_x;
	int				shift_y;
	float			zoom;
	float			zoom_new;
	float			step_x;
	float			step_y;
	float			angle;
};

// fdf.c
//void			leaks(void);
int				print_mem_fail(void);
int				print_wrong_argc(void);
int				check_extension(char *file_name);
int				main(int argc, char **argv);

// read_input.c
char			**get_ascii_line(int fd);
void			fill_array(struct s_draw *ctrl, int fd);
int				scan_input(struct s_draw *ctrl, int fd, int file_rows);
int				get_dimensions(char *file_name, struct s_draw *ctrl);
void			read_input(char *file_name, struct s_draw *ctrl);

// read_utils.c
void			free_ascii(struct s_draw *ctrl, char **ascii_numbers);
//int				fdf_atoi_sign(char *ascii_number, int *sign, int i);
//int				fdf_atoi(struct s_fdf *array, char *ascii_numbers);
int				fdf_atoi(struct s_draw *ctrl, char **ascii_numbers, int j);
unsigned long	**initialize_lu_array(int height, int width);
int				**initialize_int_array(int height, int width);

// colors.c
int				harmonize_color_length(int len, unsigned long converted);
int				ft_current_digit(char *original, int i, int len);
unsigned long	ft_hex_2_dec(struct s_draw *ctrl,
					char **ascii_number, char *original);
void			extract_colors(struct s_draw *ctrl,
					char **ascii_number, int i_row);

// ft_split.c
char			*ft_substr_end(char *substr, int len_substr, char const *str,
					unsigned int start);
char			*ft_substr(char const *str, int start, int len);
int				word_counter(char const *str, char c);
//void			free_memory(char ***pointers, int i_ptrs);
char			**ft_split(char const *s, char c);

// initialize_drawing.c
void			center_map_initial(struct s_draw *ctrl);
void			center_map(struct s_draw *ctrl);
void			initialize_image(struct s_draw *ctrl);
void			setup_window(struct s_draw *ctrl);

// draw.c
void			colorize_vertically(struct s_line *coord);
void			convert_3d(struct s_draw *ctrl, struct s_line *coord);
void			draw_line(struct s_draw *ctrl, struct s_line *coord);
void			draw_map(struct s_draw *ctrl);

// interactions.c
void			zoom_map(struct s_draw *ctrl, mlx_key_data_t keydata);
void			translate_map(struct s_draw *ctrl, mlx_key_data_t keydata);
void			rotate_map(struct s_draw *ctrl, mlx_key_data_t keydata);
void			hook_to_draw(mlx_key_data_t keydata, void *param);
void			hook(mlx_key_data_t keydata, void *param);
//void			resize_window(int32_t width, int32_t height, void *param);

// draw_utils.c
void			limit_translation(struct s_draw *ctrl);
void			limit_zoom(float *zoom, struct s_draw *ctrl);
void			draw_border(mlx_image_t *img);
//void			drawing_error(void);
void			drawing_error(struct s_draw *ctrl);

// memory_control.c
//void			fn_free(char *variable);
void			fn_free_char_ptr(char ***ptr);
void			free_lu_array(unsigned long ***array, int rows);
void			free_int_array(int ***array, int rows);
//void			free_stack(struct s_stack *stack);
//void			free_s_fdf(struct s_fdf *an_s_fdf);
void			free_s_draw(struct s_draw *an_s_draw);
void			free_s_draw_and_exit(struct s_draw *an_s_draw);

// utils.c
float			ft_abs(float *n);
float			ft_min(float m, float n);
float			ft_max(float m, float n);
int				find_start(const char *s, char c, int i);
int				find_end(const char *s, char c, int i);

#endif // FDF_H
