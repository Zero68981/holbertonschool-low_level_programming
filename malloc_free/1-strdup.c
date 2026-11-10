#include "main.h"
#include <stdlib.h>


/**
* _strdup - returns a pointer to a newly allocated space in memory
* which contains a copy of the string given as a parameter
* @str: the char used to fill the array
* Return: pointer to the duplicate, or NULL on faillure
*/
char *_strdup(char *str);
{
char *dup;
unsigned int len = 0;
unsigned int i;
if (str == NULL)
return (NULL)
While(str(len) != '\0')
len++;
dup = malloc(sizeof(char) * (len + 1));
if (dup == NULL)
return (NULL);
for (i = 0; i <= len; i++)
dup[i] = str[i]
return (dup);
}
