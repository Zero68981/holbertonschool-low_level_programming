#include "main.h"

/**
 * is_separator - checks if a character is a word separator
 * @c: character to check
 *
 * Return: 1 if separator, 0 otherwise
 */
int is_separator(char c)
{
char seps[] = " \t\n,;.!?\"(){}";
int i;

for (i = 0; seps[i] != '\0'; i++)
{
if (c == seps[i])
return (1);
}
return (0);
}

/**
 * cap_string - capitalizes all words of a string
 * @str: string to modify
 *
 * Return: pointer to str
 */
char *cap_string(char *str)
{
int i = 0;
int new_word = 1;

while (str[i] != '\0')
{
if (is_separator(str[i]))
new_word = 1;
else if (new_word)
{
if (str[i] >= 'a' && str[i] <= 'z')
str[i] = str[i] - 32;
new_word = 0;
}
i++;
}
return (str);
}
