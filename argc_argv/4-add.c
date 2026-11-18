#include <stdio.h>
#include <stdlib.h>

/**
 * is_number - checks that a string contains only digits
 * @s: string to check
 *
 * Return: 1 if @s contains only digits, 0 otherwise
 */
int is_number(char *s)
{
int i;

if (s[0] == '\0')
return (0);

for (i = 0; s[i] != '\0'; i++)
{
if (s[i] < '0' || s[i] > '9')
return (0);
}
return (1);
}

/**
 * main - adds positive numbers passed as arguments
 * @argc: number of command line arguments
 * @argv: array of command line arguments
 *
 * Return: 0 on success, 1 if an argument is not a positive number
 */
int main(int argc, char *argv[])
{
int i, sum = 0;

for (i = 1; i < argc; i++)
{
if (!is_number(argv[i]))
{
printf("Error\n");
return (1);
}
sum += atoi(argv[i]);
}

printf("%d\n", sum);
return (0);
}
