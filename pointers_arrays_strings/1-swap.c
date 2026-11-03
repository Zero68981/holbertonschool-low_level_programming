#include "main.h"
#include <stdio.h>

/**
*swap_int -  function that swaps the values of two integers.
*@a: pointeur vers le premier entier
*@b: pointeur vers le second entier
*Return: Always 0.
*/
void swap_int (int *a, int *b)
{
int tmp;
tmp = *a;
*a = *b;
*b = tmp;
}
