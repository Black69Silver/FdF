#include "get_next_line.h"

int	main(void) 
{
	FILE	*file;
	char	*line;
	int		fd;
	int		i;

	// Open the file in read mode ("r")
	file = fopen("/Users/ggeorgie/Gprojects/23_FdF/get_next_line/input.txt", "r");

	// Check if the file was successfully opened
	if (file == NULL) 
	{
		printf("Unable to open the file.\n");
		return (1);
	}

	// Obtain the file descriptor
	fd = fileno(file);

	// Perform operations on the file using fd if needed
	i = 0;
	while (i < 9)
	{
		line = get_next_line(fd);
		printf("return [%i]: '%s'\n", i, line);
		free(line);
		i++;
	}

	// Close the file when done
	fclose(file);

	// Return success code
	return (0);
}
