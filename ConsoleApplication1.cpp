// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <omp.h>



int main(int argc, char *argv[])
{
	FILE *input;

	int wymiary[4];

	int i, j, k;
	int **tab1;
	int **tab2;
	
	int **wynik;

	if (argc < 4)
	{
		printf("\n Brak parametrow : wejscie wejscie wyjscie przetwarzanie");
		printf("\n wejscie - naawa pliku z opisem macierzy");
		printf("\n wyjscie - nazwa pliku wyjsciowego. Litera \"e\" wypisanie na ekran ");
		printf("\n przetwarzanie : r(ownolegle) lub s(ekwencyjne)\n ");

		exit(1);
	}

	fopen_s(&input, argv[1], "r");
	if (input == NULL)
	{
		printf("\n  Brak pliku - Blad");
		exit(1);
	}
	fscanf_s(input, "%d", &wymiary[0]);
	fscanf_s(input, "%d", &wymiary[1]);
	fclose(input);


	fopen_s(&input, argv[2], "r");
	if (input == NULL)
	{
		printf("\n  Brak pliku - Blad");
		exit(1);
	}
	fscanf_s(input, "%d", &wymiary[2]);
	fscanf_s(input, "%d", &wymiary[3]);
	fclose(input);


	if (wymiary[1] != wymiary[2])
	{
		printf("\n Niezgodnosc wymiarow - BLAD");
		exit(1);
	}


	tab1 = (int **)malloc(wymiary[0] * sizeof(int));
	tab2 = (int **)malloc(wymiary[2] * sizeof(int));
	wynik = (int **)malloc(wymiary[0] * sizeof(int));

	for (i = 0; i < wymiary[0]; i++)
		tab1[i] = (int *)malloc(wymiary[1] * sizeof(int));

	for (i = 0; i < wymiary[2]; i++)
		tab2[i] = (int *)malloc(wymiary[3] * sizeof(int));

	for (i = 0; i < wymiary[0]; i++)
		wynik[i] = (int *)malloc(wymiary[3] * sizeof(int));


	int temp = 0;

	fopen_s(&input, argv[1], "r");
	fscanf_s(input, "%d", &temp);
	fscanf_s(input, "%d", &temp);
	for (i = 0; i < wymiary[0]; i++)
		for (j = 0; j < wymiary[1]; j++)
			fscanf_s(input, "%d", &tab1[i][j]);
	fclose(input);

	fopen_s(&input, argv[1], "r");
	fscanf_s(input, "%d", &temp);
	fscanf_s(input, "%d", &temp);
	for (i = 0; i < wymiary[2]; i++)
		for (j = 0; j < wymiary[3]; j++)
			fscanf_s(input, "%d", &tab2[i][j]);
	fclose(input);


	for (i = 0; i < wymiary[0]; i++)
		for (j = 0; j < wymiary[3]; j++)
			wynik[i][j] = 0;


	double _Start = 0;
	double _Stop = 0;


	_Start = omp_get_wtime();

	if (!strcmp(argv[4], "s"))
	{
		for (i = 0; i < wymiary[0]; i++)
			for (j = 0; j < wymiary[3]; j++)
				for (k = 0; k < wymiary[1]; k++)
					wynik[i][j] += tab1[i][k] * tab2[k][j];
	}
	else
	{

		#pragma omp parallel shared(wynik,tab1,tab2) private(i,j,k) num_threads(2)
		{
			#pragma omp for
			for (i = 0; i < wymiary[0]; i++)
				for (j = 0; j < wymiary[3]; j++)
					for (k = 0; k < wymiary[1]; k++)
						wynik[i][j] += tab1[i][k] * tab2[k][j];

		}
	}

	_Stop = omp_get_wtime();
	   



	if (!strcmp(argv[3], "e"))
	{
		if (!strcmp(argv[4], "s")) printf("\nPrzetwarzanie : seksewncyjne");
		else printf("\nPrzetwarzanie : rownolegle");

		printf("\n TABLICA WYNIKOWA \n");
		for (i = 0; i < wymiary[0]; i++)
		{
			for (j = 0; j < wymiary[3]; j++)
				printf("  %d ", wynik[i][j]);
			printf("\n");
		}
		
		printf("\n Czas wykonania :\n         sekund : %f\n", _Stop - _Start);

	}
	else
	{
		fopen_s(&input, argv[3], "wt");

		if (!strcmp(argv[4], "s")) fprintf(input, "Przetwarzanie : seksewncyjne");
		else fprintf(input, "Przetwarzanie : rownolegle");

		fprintf(input, "\n%d %d\n", wymiary[0], wymiary[3]);
		for (i = 0; i < wymiary[0]; i++)
		{
			for (j = 0; j < wymiary[3]; j++)
				fprintf(input, "  %d ", wynik[i][j]);
			fprintf(input, "\n");
		}
		
		fprintf(input, "\n Czas wykonania :\n         sekund : %f\n", _Stop - _Start);

		fclose(input);
	}


	for (i = 0; i < wymiary[0]; i++)
		free(tab1[i]);
	free(tab1);

	for (i = 0; i < wymiary[2]; i++)
		free(tab2[i]);
	free(tab2);

	for (i = 0; i < wymiary[0]; i++)
		free(wynik[i]);
	free(wynik);

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
