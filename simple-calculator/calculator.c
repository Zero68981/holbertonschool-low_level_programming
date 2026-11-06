#include <stdio.h>
/**
 * main - display a simple calculator
 * Return: always 0
 */

int main(void)


{
int choice;
int a;
int b;

printf("Simple Calculator\n");
while (1)
{
printf("1) add\n");
printf("2) Subtract\n");
printf("3) Multiply\n");
printf("4) Divide\n");
printf("0) Quit\n");
printf("Choice: ");

if (scanf("%d", &choice) != 1)
return (1);

if (choice == 0)
{
printf("Bye!\n");
break;
}

if (choice < 0 || choice > 4)
{
printf("Invalid choice\n");
continue;
}
if (choice == 1)
	{
		printf("a: ");
		if (scanf("%d", &a) != 1)
			return (1);

		printf("B: ");
		if (scanf("%d", &b) != 1)
			return (1);

		printf("Result: %d\n", a + b);
	}
	if (choice == 2)
	{
		printf("A: ");
		if (scanf("%d", &a) != 1)
			return (1);

		printf("B: ");
		if (scanf("%d", &b) != 1)
			return (1);

		printf("Result: %d\n", a - b);
	}
	if (choice == 3)
		{
			printf("A: ");
			if (scanf("%d", &a) != 1)
				return (1);

			printf("B: ");
			if (scanf("%d", &b) != 1)
				return (1);

			printf("Result: %d\n", a * b);
		}
}
return(0);
}
