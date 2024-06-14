/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:56:23 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/05/31 12:57:23 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	print_mem_fail(void)
{
	write(2, mlx_strerror(MLX_MEMFAIL), ft_strlen(mlx_strerror(MLX_MEMFAIL)));
	write(2, "\n", 1);
}
