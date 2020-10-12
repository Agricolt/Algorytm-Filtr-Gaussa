// DLL_C.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DLL_C.h"

void wyswietl_tablice_koloru(int height, int width, byte ** temp1)
{
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			std::cout << (short)temp1[j][i] << ", ";
		}
		std::cout << std::endl;
	}
}

int DLL_C_API dodaj(int x, int y)
{
	return x + y;
}

void DLL_C_API filtrGaussa(byte ** b, byte ** g, byte ** r, int height, int width)
{
	wyswietl_tablice_koloru(height, width, b);
}


