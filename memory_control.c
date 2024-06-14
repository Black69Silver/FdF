/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 03:37:03 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/02 22:32:09 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Frees a pointer and prevent double-freeing.
 * @param	char	*variable: a pointer to heap memory address.
 * @return	NULL.
 */
// void	fn_free(char *variable)
// {
// 	if (variable)
// 	{
// 		free(variable);
// 		variable = NULL;
// 	}
// }

/**
 * Frees the memory allocated for a double pointer and the pointers it points
 * to. The triple pointer is needed for modification of the original pointer.
 * @param	char	***ptr: a pointer to pointers.
 * @return	Void.
 */
void	fn_free_char_ptr(char ***ptr)
{
	int	i;

//	printf("fn_free_char_ptr\n");
	if (*ptr)
	{
		i = 0;
		while (*ptr[i])
		{
//			printf("fn_free(ptr[%i]): %s\n", i, ptr[i]);
			fn_free(&(*ptr)[i]);
			i++;
		}
		printf("fn_free_char_ptr\n");
		fn_free(*ptr);
	}
}

/**
 * Frees each sub-array of array and the array itself
 * @param	int	***array: a pointer to a 2D array of long unsigned integers,
 * 						which is to be freed.
 * @param	int	rows: the number of rows in the array.
 * @param	int	i: the index of the sub-array to be freed.
 * @return	Void.
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
 * @return	NULL.
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
 * Frees the stack, including all its elements.
 * @param	struct s_stack	*stack: Pointer to the stack.
 * @param	struct s_element	*current: Pointer to the current element.
 * @param	struct s_element	*next: Pointer to the next element.
 * @return	Void.
 */
// void	free_stack(struct s_stack *stack)
// {
// 	struct s_element	*current;
// 	struct s_element	*next;

// 	if (stack)
// 	{
// 		current = (*(*stack).head).next;
// 		while (current != (*stack).head)
// 		{
// 			next = (*current).next;
// 			fn_free((char *)current);
// 			current = next;
// 		}
// 		fn_free((char *)(*stack).head);
// 		fn_free((char *)stack);
// 	}
// }

/**
 * Frees an 's_draw' type of structure, including all its elements,
										particularly 's_fdf' type of structure.
 * @param	struct s_draw *an_s_draw: Pointer to the structure.
 * @return	Void.
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
//				printf("line %i:\n", i);
				if ((*(*an_s_draw).array).z[i])
				{
//					printf("free 'z'\n");
					fn_free((char **)&(*(*an_s_draw).array).z[i]);
				}
				if ((*(*an_s_draw).array).clr[i])
				{
//					printf("free 'color'\n");
					fn_free((char **)&(*(*an_s_draw).array).clr[i]);
				}
				i++;
			}
//			fn_free((char **)&(*(*an_s_draw).array).x);							// This isn't necessary, as there's no memory alocation.
//			fn_free((char **)&(*(*an_s_draw).array).y);							// This isn't necessary, as there's no memory alocation.
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
