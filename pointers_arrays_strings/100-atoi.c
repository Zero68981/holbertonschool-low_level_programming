#include "main.h"

/**
 * _atoi - converts a string to an integer
 * @s: the string to convert
 *
 * Return: the integer, or 0 if no number is found
 */
int _atoi(char *s)
{
int sign = 1;
int result = 0;
int found = 0;
int i = 0;

while (s[i] != '\0')
{
if (s[i] >= '0' && s[i] <= '9')
{
found = 1;
result = result * 10 - (s[i] - '0');
}
else if (found)
break;
else if (s[i] == '-')
sign = -sign;
i++;
}

if (!found)
return (0);
if (sign == 1)
return (-result);
return (result);
}
