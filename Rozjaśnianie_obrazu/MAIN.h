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

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned short ushort;

typedef void(*filtrGaussaC)(byte *, int, int, int, int);
typedef int(*filtrGaussaAsm)(BYTE *, DWORD32, DWORD32, DWORD32, DWORD32);

extern "C" int _stdcall filtrGaussa_asm(BYTE *bgr, DWORD32 height, DWORD32 width, DWORD32 od, DWORD32 to);

void wyswietl_tablice(int ilosc_znakow, char * temp1);


void wyswietl_tablice_koloru(int height, int width, byte * temp1);


bool wczytajBitmape(int &width, int &height, int &padding, byte *&bgr, std::string &nazwa_pliku, BITMAPFILEHEADER *&header,
	BITMAPINFOHEADER *&info_header);


bool zapiszBitmape(int &width, int &height, int &padding, byte *&bgr, BITMAPFILEHEADER *&header,
	BITMAPINFOHEADER *&info_header);


void utworzWatki(int ilosc_watkow, byte *bgr, int height, int width, int ktora_biblioteka);
