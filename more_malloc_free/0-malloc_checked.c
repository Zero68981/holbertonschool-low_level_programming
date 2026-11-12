#include "main.h"
#include <stdlib.h>

/**
 * malloc_checked - alloue de la mémoire avec malloc
 * @b: nombre d'octets à allouer
 * Return: pointeur vers la mémoire allouée.
 * Si malloc échoue, le processus se termine avec le statut 98.
 */
void *malloc_checked(unsigned int b)
{
void *ptr;
ptr = malloc(b);
if (ptr == NULL)
exit(98);
return (ptr);
}
