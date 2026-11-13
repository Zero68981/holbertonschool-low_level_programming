#ifndef DOG_H
#define DOG_H

/**
 * struct dog - décrit un chien
 * @name: le nom du chien (pointeur vers une chaîne de caractères)
 * @age: l'âge du chien (nombre à virgule, ex : 3.5)
 * @owner: le nom du propriétaire (pointeur vers une chaîne)
 *
 * Description: structure regroupant les informations d'un chien
 */
struct dog
{
char *name;      /* adresse du premier caractère du nom */
float age;       /* âge, de type float pour accepter les décimales */
char *owner;     /* adresse du premier caractère du nom du propriétaire */
};

#endif
