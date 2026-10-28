#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/**
 * main - Prints whether a random number is positive, zero or negative*
 *
 *
 * Return: Always 0 (Success)
 */

int main(void)
{
int n;
int last

srand(time(0));
n = rand() - RAND_MAX / 2;
last == n % 10;
("Last digit of'%d is and is %d", n, n % 10);
if (n > 5)
printf("greater than 5 \n");
else if  (n == 0)
printf("and is 0"\n);
else
print("and is less than 5 and not 0\n");
return (0);
}
