
#include <stdio.h>

/**
 * main - Display a simple calculator menu
 *
 * Return: Always 0
 */
int main(void)
{
	int choice;
	int A;
	int B;

	do
	{
		printf("Simple Calculator\n");
		printf("1) Add\n");
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
		}

		if (choice < 0 || choice > 4)
		{
			printf("Invalid choice\n");
		}

		if (choice == 1)
		{
			printf("A: ");
			if (scanf("%d", &A) != 1)
				return (1);

			printf("B: ");
			if (scanf("%d", &B) != 1)
				return (1);

			printf("Result: %d\n", A + B);
		}

		if (choice == 2)
		{
			printf("A: ");
			if (scanf("%d", &A) != 1)
				return (1);

			printf("B: ");
			if (scanf("%d", &B) != 1)
				return (1);

			printf("Result: %d\n", A - B);
		}

		if (choice == 3)
		{
			printf("A: ");
			if (scanf("%d", &A) != 1)
				return (1);

			printf("B: ");
			if (scanf("%d", &B) != 1)
				return (1);

			printf("Result: %d\n", A * B);
		}

		if (choice == 4)
		{
			printf("A: ");
			if (scanf("%d", &A) != 1)
				return (1);

			printf("B: ");
			if (scanf("%d", &B) != 1)
				return (1);

			if (B == 0)
			{
				printf("Error: division by zero\n");
			}
			else
			{
				printf("Result: %g\n", (double) A / B);
			}
		}

	} while (choice != 0);

	return (0);
}