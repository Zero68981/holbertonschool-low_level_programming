#ifndef DOG_H
#define DOG_H

/**
 * struct dog - informations sur un chien
 * @name: nom du chien
 * @age: âge du chien
 * @owner: propriétaire du chien
 */
struct dog
{
char *name;
float age;
char *owner;
};

void init_dog(struct dog *d, char *name, float age, char *owner);

#endif /* DOG_H */
