// DLL_C.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DLL_C.h"

void wyswietl_tablice_koloru(int od, int to, byte * temp1)
{
	int kreska = 0, linia = 0;
	for (int i = od; i < to; i++)
	{
		std::cout << (short)temp1[i] << ", ";
		kreska++;
		linia++;
		if (kreska % 3 == 0)
			std::cout << " | ";
		if (linia % 12 == 0)
			std::cout << std::endl;
	}
	std::cout << "KONIEC" << std::endl;
}

//probna funkcja do eksportu
int DLL_C_API dodaj(int x, int y)
{
	return x + y;
}

void DLL_C_API filtrGaussa(byte *bgr, int height, int width, int od, int to)
{
	int matrix1[9] = 
	{ 1, 2, 1, 
		2, 4, 2, 
		1, 2, 1 };
	int suma_matrix1 = 16;
	int pixs = width * 3;
	for (int i = od; i < to; i++)
	{
		if ((i - pixs - 3) >= 0 && (i - pixs - 3) <= width*height * 3)
		{
			if ((i - pixs + 3) >= 0 && (i - pixs + 3) <= width*height * 3)
			{
				if ((i + pixs + 3) >= 0 && (i + pixs + 3) <= width*height * 3)
				{
					if ((i + pixs - 3) >= 0 && (i + pixs - 3) <= width*height * 3)
					{
						int x = /*gorny rzad*/ ((bgr[i - pixs - 3] * matrix1[0])
							+ (bgr[i - pixs] * matrix1[1])
							+ (bgr[i - pixs + 3] * matrix1[2])
							+ /*srodkowy*/ (bgr[i - 3] * matrix1[3])
							+ (bgr[i] * matrix1[4])
							+ (bgr[i + 3] * matrix1[5])
							+ /*dolny*/ bgr[i + pixs - 3] * matrix1[6]
							+ bgr[i + pixs] * matrix1[7]
							+ bgr[i + pixs + 3] * matrix1[8]) / suma_matrix1;
						bgr[i] = (byte)x;
					}
				}
			}
		}
	}
}


