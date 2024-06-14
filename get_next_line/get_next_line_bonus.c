/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 03:14:10 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/02 20:49:25 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*	1. Check if inputs are valid:
		- 0 < fd < USHRT_MAX,
		- 0 <= BUFFER_SIZE < UINT_MAX,
		- 'read' is able to read from 'fd',
	- If any of these is not true:
		- if 'carry_over' static pointer exists:
			- 'free' 'carry_over',
			- carry_over = 0;
		- return (NULL).
	2. If 'carry_over' static pointer to string does not exist,
		initialize it with length of 1 ('\0');
	- If not successful: 'return' 'NULL'.
	3. Initialize 'buffer' pointer to str with length of BUFFER_SIZE + 1 ('\0');
	- If not successful: 'free' 'carry_over' and 'return' 'NULL'.
	4. Initialize buffer_counter with positive integer.
	5. While buffer_counter is positive:
		buffer_counter = read(fd, buffer, BUFFER_SIZE);
	5.1. If buffer_counter = -1: 'free' 'carry_over' and 'buffer',
		and return (NULL);
	5.2. Add '\0' to the end of 'buffer'.
	5.3. Join 'carry_over' and 'buffer' into 'temp'.
	- If not successful: 'free' 'carry_over' and 'buffer', and 'return' 'NULL'.
	5.4. 'free' 'carry_over'.
	5.5. 'carry_over' = 'temp'.
	5.6. If 'carry_over' contains '\n', make 'buffer_counter' = 0 to exit 5.
	6. 'free' 'buffer'.
	7. If 'carry_over' is NULL or carry_over[0] = '\0', 'return' 'NULL'.
	8. Discover '\n' on the 'i'-th position of 'carry_over'
		within the range 0 < i < ft_strlen(carry_over).
	9.1. If '\n' is not found in 'carry_over'
	- Initialize 'line' pointer to string with length of i + 1 ('\0');
		- If not successful, 'free' 'carry_over', 'return' 'NULL'.
	- Copy 'carry_over' to 'line'.
	- Set carry_over[0] to '\0'.
	9.2. If '\n' is found in 'carry_over':
	- Initialize 'line' pointer to string with length of i + 2 ('\n' + '\0');
		- If not successful, 'free' 'carry_over', 'return' 'NULL'.
	- Copy from 'carry_over' to 'line' everything up to and '\n' itself.
	- Copy 'carry_over' from '\n' onwards to (the beginning of) 'carry_over'.
	10. 'return' 'line'.
 *
 * Memory allocation for carry_over, buffer, temp, line.
 */

#include "get_next_line_bonus.h"

/**
 * Frees a pointer and prevent double-freeing.
 * @param	char	*variable: a pointer to heap memory address.
 * @return	NULL.
 */
char	*fn_free(char **variable)
{
	if (*variable)
	{
		free(*variable);
		*variable = NULL;
	}
	return (*variable);
}

char	*process_carry_over(char *carry_over)
{
	int		i;
	char	*line;

//	printf("in process_carry_over\n");
	if (carry_over == NULL || carry_over[0] == '\0')
		return (NULL);
	i = 0;
	while (carry_over[i] && carry_over[i] != '\n')
		i++;
	if (i == ft_strlen(carry_over))
	{
		line = ft_calloc(i + 1, sizeof(char));
		if (!line)
			return (carry_over = fn_free(&carry_over), NULL);
		ft_strlcpy(line, carry_over, i + 1);
		carry_over[0] = '\0';
//		printf("1. carry_over: '%s'\n", carry_over);
//		printf("1. line: '%s'\n", line);
	}
	else
	{
		line = ft_calloc(i + 2, sizeof(char));									// + 2 for ('\n' + '\0')
		if (!line)
			return (carry_over = fn_free(&carry_over), NULL);
//		printf("2.0. carry_over: '%s'\n", carry_over);
//		printf("2.0. line: '%s'\n", line);
		ft_strlcpy(line, carry_over, i + 2);
		ft_strlcpy(carry_over, &carry_over[i + 1], ft_strlen(carry_over) - i);
//		printf("2.1. line: '%s'\n", line);
//		printf("2.1. carry_over: '%s'\n", carry_over);
	}
	return (line);
}

char	*read_buffer(int fd, char *carry_over)
{
	char	*buffer;
	int		buffer_counter;
	char	*temp;

	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (carry_over = fn_free(&carry_over), NULL);
	buffer_counter = 1;
	while (buffer_counter > 0)
	{
		buffer_counter = read(fd, buffer, BUFFER_SIZE);
		if (buffer_counter == -1)
			return (carry_over = fn_free(&carry_over), free(buffer), NULL);
		buffer[buffer_counter] = '\0';
//		printf("in read_buffer carry_over: '%s', buffer: '%s'\n", carry_over, buffer);
		temp = ft_strjoin(carry_over, buffer);
		if (temp == NULL)
			return (carry_over = fn_free(&carry_over), free(buffer), NULL);
//		printf("in read_buffer temp: %s\n", temp);
		carry_over = fn_free(&carry_over);
		carry_over = temp;
		if (ft_strchr(carry_over, '\n'))
			buffer_counter = 0;
	}
	buffer = fn_free(&buffer);
	return (carry_over);
}

char	*get_next_line(int fd)
{
	static char	*carry_over[USHRT_MAX];
	char		*line;

//	printf("in get_next_line\n");
//	printf("1 in get_next_line carry_over: '%s'\n", carry_over[fd]);
	if (fd < 0 || fd > USHRT_MAX || read(fd, 0, 0) < 0
		|| BUFFER_SIZE <= 0 || BUFFER_SIZE > UINT_MAX)
	{
		if (carry_over[fd])
		{
			carry_over[fd] = fn_free(&carry_over[fd]);
//			carry_over[fd] = 0;
		}
		return (NULL);
	}
//	printf("2 in get_next_line carry_over: '%s'\n", carry_over[fd]);
	
	if (!carry_over[fd])
	{
		carry_over[fd] = ft_calloc(1, sizeof(char));
		if (!carry_over[fd])
			return (NULL);
	}
//	printf("3 in get_next_line carry_over: '%s'\n", carry_over[fd]);
	carry_over[fd] = read_buffer(fd, carry_over[fd]);
	if (!carry_over[fd])
		return (NULL);
//	printf("in get_next_line carry_over: %s\n", carry_over[fd]);
	line = process_carry_over(carry_over[fd]);
//	printf("in get_next_line line: %s\n", line);
	if (carry_over[fd] && carry_over[fd][0] == '\0')
	{
		carry_over[fd] = fn_free(&carry_over[fd]);
//		carry_over[fd] = 0;
	}
//	if (line != 0 && line[0] == '\0')
//		return (free(line), NULL);
	return (line);
}
