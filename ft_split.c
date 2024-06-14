/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 16:25:04 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/06/02 22:07:29 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

char	*ft_substr_end(char *substr, int len_substr, char const *str,
	unsigned int start)
{
	int	i;

	i = 0;
	if (substr != NULL)
	{
		while (i < len_substr)
		{
			substr[i] = str[start + i];
			i++;
		}
		substr[i] = '\0';
	}
	return (substr);
}

/** 
 * Copy in 'substr' string 'len' characters from 'str' string, starting at 'start'. 
 * @param	char	*str: string to be copied from.
 * @param	int		start: position to start copying from.
 * @param	int		len: maximum length of the slot.
 * @param	int		len_str: length of 'str' string.
 * @param	int		len_substr: length of 'substr' string.
 * @return	char	*substr: newly created string, or NULL if allocation fails.
*/
char	*ft_substr(char const *str, int start, int len)
{
	int		len_str;
	int		len_substr;
	char	*substr;

	len_str = ft_strlen(str);
	if (*str == '\0' || start > len_str)
	{
		substr = ft_calloc(1, sizeof(char *));
		if (substr != NULL)
			substr[0] = '\0';
		return (substr);
	}
	if (len_str == 0)
		return ("");
	if (start + len <= len_str)
		len_substr = len;
	else
		len_substr = len_str - start;
	substr = ft_calloc((len_substr + 1), sizeof(char));
	return (ft_substr_end(substr, len_substr, str, start));
}

/**
 * Counts the number of words/sub-strings in a string.
 * @param	char	*str: string to be evaluated.
 * @param	char	c: delimiter character.
 * @param	int		in_word: flag for current position being in a word.
 * @return	int		count: The number of words/sub-strings.
 */
int	word_counter(char const *str, char c)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			in_word = 0;
		else if (!in_word && str[i] != '\n')
		{
			in_word = 1;
			count++;
		}
//		printf("char: %c, count: %i, in_word: %i\n", str[i], count, in_word);
		i++;
	}
	return (count);
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

/**
 * Frees the memory allocated for '**pointers'. The triple pointer is
 * needed for modification of the original pointer in 'ft_split' function.
 * @param	char ***pointers: string of pointers.
 * @param	int	arr[3]: index for the word/sub-string/pointers.
 * @return	Freed memory.
 */
// void	free_memory(char ***pointers, int i_ptrs)
// {
// 	while (i_ptrs > 0)
// 	{
// 		i_ptrs--;
// 		free((*pointers)[i_ptrs]);
// 	}
// 	free(*pointers);
// 	*pointers = NULL;
// }

/**
 * Splits string ’s’ into an array of sub-strings using ’c’ delimiter.
 * @param	char	*str: string to be split.
 * @param	char	c: delimiter character.
 * @param	int		arr: array for 4 counters.
 * @param	int		arr[0]: position within 's' string.
 * @param	int		arr[1]: index for the start of the word.
 * @param	int		arr[2]: index for the end of the word.
 * @param	int		arr[3]: index for the word/sub-string/pointers.
 * @return	A string of pointers to the new strings resulting from the split.
 */
char	**ft_split(char const *str, char c)
{
	char	**pointers;
	int		arr[4];

//	printf("in ft_split\n");
	arr[0] = 0;
//	pointers = malloc(sizeof(char *) * (word_counter(str, c) + 1));
	pointers = ft_calloc((word_counter(str, c) + 1), sizeof(char *));			// + 1 for NULL-terminator
	if (!pointers || !str)
		return (NULL);
	arr[3] = 0;
	while (str[arr[0]])
	{
//		printf("row to split: %s, position within 's' string: %i\n", s, arr[0]);
		arr[1] = find_start(str, c, arr[0]);
//		printf("row to split: %s, index for the start of the word: %i\n", s, arr[1]);
		arr[2] = find_end(str, c, arr[1]);
//		printf("index for the end of the word: %i\n", arr[2]);
		if (arr[2] > arr[1])
		{
			pointers[arr[3]] = ft_substr(str, arr[1], arr[2] - arr[1]);
			if (pointers[arr[3]] == NULL)
//				return (free_memory(&pointers, arr[3]), NULL);
				return (fn_free_char_ptr(&pointers), NULL);
//			printf("in ft_split number[%i]: '%s'\n", arr[3], pointers[arr[3]]);
			arr[3]++;
		}
		arr[0] = arr[2];
	}
	pointers[arr[3]] = NULL;
//	printf("in ft_split number[%i]: '%s'\n", arr[3], pointers[arr[3]]);
//	printf("number of numbers/word/sub-string: %i + '\\0'\n", arr[3]);
	return (pointers);
}
