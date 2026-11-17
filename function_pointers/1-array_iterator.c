#include "function_pointers.h"

/**
 * array_iterator - exécute une fonction sur chaque élément d'un tableau
 * @array: pointeur vers le premier élément du tableau d'entiers
 * @size: nombre d'éléments dans le tableau
 * @action: pointeur vers la fonction à appliquer à chaque élément
 *          (elle prend un int en paramètre et ne retourne rien)
 *
 * Description: parcourt le tableau de l'indice 0 à size - 1 et appelle
 * la fonction @action avec la valeur de chaque élément.
 * Si @array ou @action vaut NULL, la fonction ne fait rien.
 *
 * Return: Nothing.
 */
void array_iterator(int *array, size_t size, void (*action)(int))
{
size_t i;

if (array == NULL || action == NULL)
return;

for (i = 0; i < size; i++)
action(array[i]);
}
