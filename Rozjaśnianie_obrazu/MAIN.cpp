#include <iostream>
#include <Windows.h>
#include <thread>
#include <string>
#include <fstream>
#include <vector>
#include <string>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned short ushort;

using namespace std;

/**
Funkcja pomocnicza stworzona w celu podejrzenia zawartosci pamieci
*/
void wyswietl_tablice(int ilosc_znakow, char * temp1)
{
	byte * temp2 = (byte*)(temp1);
	int kreska = 0, linia = 0;
	for (int i = 0; i < ilosc_znakow; i++)
	{
		cout << (short)temp2[i] << ", ";
		kreska++;
		linia++;
		if (kreska % 3 == 0)
			cout << " | ";
		if (linia % 12 == 0)
			cout << endl;
	}
	cout << "KONIEC" << endl;
}

int main()
{
	int const MAX_LICZBA_WATKOW = thread::hardware_concurrency();
	string komunikat = "";
	komunikat += "Liczba wykrytych watkow: " + to_string(MAX_LICZBA_WATKOW) + "...\n";
	/*ODCZYT*/
	ifstream ifs("4k.bmp", ios::binary);
	/*Wczytanie naglowka pliku*/
	char* temp = new char[sizeof(BITMAPFILEHEADER)];
	ifs.read(temp, sizeof(BITMAPFILEHEADER));
	BITMAPFILEHEADER* header = (BITMAPFILEHEADER*)(temp);

	/*Wczytanie naglowka obrazu*/
	temp = new char[sizeof(BITMAPINFOHEADER)];
	ifs.read(temp, sizeof(BITMAPINFOHEADER));
	BITMAPINFOHEADER* info_header = (BITMAPINFOHEADER*)(temp);

	/*Wczytanie bitmapy*/
	//Obliczenie wyrownania do 4 bitow
	int padding = 0;
	if (info_header->biWidth % 4 == 0)
		padding = 0;
	else
		padding += 4 - (info_header->biWidth % 4);

	short width = info_header->biWidth;
	short height = info_header->biHeight;
	short rozmiar_bitmapy_noPadding = height * width;
	short rozmiar_bitmapy_cala = rozmiar_bitmapy_noPadding + (padding * width);
	int jeden_wiersz = 3 * width;

	byte ** b = new byte*[height], **g = new byte*[height], **r = new byte*[height]; //4 tablice z kolorami, ostatnia na wszystkie kolory
	for (int i = 0; i < height; i++)
	{
		b[i] = new byte[width];
		g[i] = new byte[width];
		r[i] = new byte[width];
	}

	ifs.seekg(header->bfOffBits, ios::beg); // bfOffBits wskazuje pocz¹tek danych obrazka
	for (int i = 0; i < height; i++)
	{
		int wiersz = 0;
		char * temp1 = new char[jeden_wiersz];
		ifs.read(temp1, jeden_wiersz);
		ifs.seekg(padding, std::ios::cur);
		byte *temp2 = (byte*)(temp1);
		for (int k = 0; k < jeden_wiersz; k = k + 3)
		{
			b[i][wiersz] = temp2[k];
			g[i][wiersz] = temp2[k + 1];
			r[i][wiersz] = temp2[k + 2];
			wiersz++;
		}
	}

	/*KONIEC ODCZYTU*/

	//typedef int(*DOD_PROC)(int, int);

	//HMODULE dll;
	//DOD_PROC procedura;
	//if ((dll = LoadLibrary(L"DLL_C.dll")) != NULL)
	//{
	//	procedura = (DOD_PROC)GetProcAddress(dll, "dodaj");
	//	if (procedura != NULL)
	//	{
	//		cout << procedura(3, 4);
	//	}
	//	FreeLibrary(dll);
	//}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			
		}
	}

	/*ZAPIS*/
	ofstream wy("zapis.bmp", ios::binary);
	//Zapis naglowka pliku
	wy.write((char*)(header), sizeof(BITMAPFILEHEADER));
	//Zapis naglowka bitmapy
	wy.write((char*)(info_header), sizeof(BITMAPINFOHEADER));
	//Zapis pixeli bitmapy
	int ilosc_bajtow = jeden_wiersz * width + padding * width;
	char * zapis = new char[ilosc_bajtow];
	int znak = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			zapis[znak] = b[i][j];
			zapis[znak + 1] = g[i][j];
			zapis[znak + 2] = r[i][j];
			znak += 3;
		}
		for (int j = 0; j < padding; j++)
		{
			znak++;
			zapis[znak + j] = 0;
		}
	}
	wy.write(zapis, ilosc_bajtow);
	wy.close();
	/*KONIEC ZAPISU*/
	/*Usuwanie obiektow z pamieci*/
	for (int i = 0; i < height; i++)
	{
		delete[] b[i];
		delete[] g[i];
		delete[] r[i];
	}
	delete[] b;
	delete[] g;
	delete[] r;
	delete header;
	delete info_header;
	/*Koniec usuwania*/
	cout << komunikat;
	getchar();
}
