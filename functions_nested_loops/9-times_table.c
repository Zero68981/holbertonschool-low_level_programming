#include "main.h"

/**
 * times_table - prints the 9 times table, starting with 0
 */
void times_table(void)
{
int row, col, n;

for (row = 0; row <= 9; row++)
{
_putchar('0');
for (col = 1; col <= 9; col++)
{
n = row * col;
_putchar(',');
_putchar(' ');
if (n < 10)
_putchar(' ');
else
_putchar('0' + n / 10);
_putchar('0' + n % 10);
}
_putchar('\n');
}
}
