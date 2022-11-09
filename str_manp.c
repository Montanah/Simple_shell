#include "shell.h"
/**
 * _strdup - allocates a space in memory for a copy of a string
 * @src: string a copy is made of
 * Return: a pointer to the copy, or NULL if failure
 */
char *_strdup(char *src)
{
	int len = _strlen(src);
	char *dest = malloc(len + 1);
	char *ptr;

	if (dest == NULL)
		exit(EXIT_FAILURE);

	ptr = dest;

	while (*src != '\0')
	{
		*ptr = *src;
		ptr++;
		src++;
	}
	*ptr = '\0';

	return (dest);
}
/**
 * _strlen - returns length of a string
 * @str: string to be evaluated
 * Return: length of string
 */
int _strlen(char *str)
{
	int i = 0;

	if (str == NULL)
		return (0);
	while (*str != '\0')
	{
		i++;
		str++;
	}
	return (i);
}
