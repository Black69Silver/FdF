/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:56:23 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/01 03:03:48 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

	if (m < n)
		min = m;
	else
		min = n;
	return (min);
}

float	ft_max(float m, float n)
{
	float	max;

	if (m > n)
		max = m;
	else
		max = n;
	return (max);
}

/**
 * Finds the start of the word/sub-string.
 * @param	char	*str: string to be evaluated.
 * @param	char	c: delimiter character.
 * @param	int		i: index for the beginning of the search.
 * @return	The index for the beginning of the word/sub-string.
 */
int	find_start(const char *str, char c, int i)
{
	while (str[i] == c && str[i] != '\0')
		i++;
	return (i);
}

/**
 * Finds the end of the word/sub-string.
 * @param	char	*str: string to be evaluated.
 * @param	char	c: delimiter character.
 * @param	int		i: index for the start of the word/sub-string.
 * @return	The index for the end of the word/sub-string.
 */
int	find_end(const char *str, char c, int i)
{
	while (str[i] != c && str[i] != '\0')
		i++;
	return (i);
}
