#include "main.h"
/**
*main -  print _putchar followed by a new line
*
*Return: Always 0
*/
int main(void);
void print_alphabet(void)
{
char c;

for (c = 'a'; c <= 'z'; c++)
	putchar(c);
_putchar('\n');

return (0);
}
