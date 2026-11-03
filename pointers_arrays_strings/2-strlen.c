#include "main.h"
#include <stdio.h>

/**
 *_strlen - function that returns the length of a string
 @s: the string to be mesured
 Return: the number of character before the '\0'
 */
int _strlen(char *s)
{
int len = 0;
while (s[len] != '\0')
len++;
return (len);
}
