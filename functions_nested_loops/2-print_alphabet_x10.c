#include "main.h"

/**
*print_alphabet_x10 - print the alphabet in lowercase 10 times,
*followed by a new line
*
*Return: Always 0
*/
void print_alphabet_x10(void)
{

int line;
char c;

for (line = 0; line < 10; line++)
{
for (c = 'a'; c <= 'z'; c++)
_putchar(c);
_putchar('\n');
}
}
