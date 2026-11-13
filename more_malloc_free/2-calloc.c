#include "main.h"
#include <stdlib.h>

/**
 * _calloc - alloue de la mémoire pour un tableau et la met à zéro
 * @nmemb: nombre d'éléments
 * @size: taille (en octets) de chaque élément
 *
 * Return: pointeur vers la mémoire allouée, ou NULL en cas d'échec
 */
void *_calloc(unsigned int nmemb, unsigned int size)
{
char *p;
unsigned int i, total;

if (nmemb == 0 || size == 0)
return (NULL);

total = nmemb * size;
p = malloc(total);
if (p == NULL)
return (NULL);

for (i = 0; i < total; i++)
p[i] = 0;

return (p);
}
