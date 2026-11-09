#include <stddef.h>
/**
 *_strchr - locates a character in a string
*@s: the string to search
*@c: the character to look for
*
*Return: pointer to the first occurrence of c in s,
*or NULL if c is not found
*/
char *_strchr(char *s, char c)
{
while (*s != '\0')
{
if (*s == c)
return (s);
s++;
}

/*The terminating '\0' is part of the string too */
if (c == '\0')
return (s);

return (NULL);
}
