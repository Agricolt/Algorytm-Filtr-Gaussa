#pragma once

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

void wyswietl_tablice(int ilosc_znakow, char * temp1);

void wyswietl_tablice_koloru(int height, int width, byte * temp1);

bool wczytajBitmape(int &width, int &height, int &padding, byte *&b, byte *&g, byte *&r, std::string &komunikat, BITMAPFILEHEADER *&header,
	BITMAPINFOHEADER *&info_header);

bool zapiszBitmape(int &width, int &height, int &padding, byte *&b, byte *&g, byte *&r, std::string &komunikat, BITMAPFILEHEADER *&header,
	BITMAPINFOHEADER *&info_header);

void utworzWatki(int ilosc_watkow, std::thread *&tab);


extern "C" int _stdcall MyProc(DWORD x, DWORD y);
