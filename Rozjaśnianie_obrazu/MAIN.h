#pragma once

#include <iostream>
#include <Windows.h>
#include <thread>
#include <string>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>

#define NAZWA_PLIKU "4k2.bmp"

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef void(*filtrGaussa)(byte *, int, int, int, int);

void wyswietl_tablice(int ilosc_znakow, char * temp1);


void wyswietl_tablice_koloru(int height, int width, byte * temp1);


bool wczytajBitmape(int &width, int &height, int &padding, byte *&bgr, std::string &komunikat, BITMAPFILEHEADER *&header,
	BITMAPINFOHEADER *&info_header);


bool zapiszBitmape(int &width, int &height, int &padding, byte *&bgr, std::string &komunikat, BITMAPFILEHEADER *&header,
	BITMAPINFOHEADER *&info_header);


void utworzWatki(byte ilosc_watkow, byte *bgr, int height, int width);


extern "C" int _stdcall MyProc(DWORD x, DWORD y);
