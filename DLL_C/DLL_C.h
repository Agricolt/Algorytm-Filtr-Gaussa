// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLL_C_EXPORTS
#pragma once
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLL_C_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLL_C_EXPORTS
#define DLL_C_API __declspec(dllexport)
#else
#define DLL_C_API __declspec(dllimport)
#endif

#include <iostream>

typedef unsigned char byte;

/*FUNKCJE NIE EKSPORTOWANE NA ZEWNATRZ DLL*/
void wyswietl_tablice_koloru(int height, int width, byte ** temp1);
/**************************************/

/*FUNCKJE ESKPORTOWANIE NA ZEWNATRZ DLL*/
int DLL_C_API dodaj(int x, int y);

void DLL_C_API filtrGaussa(byte **b, byte **g, byte **r, int height, int width);
/***************************************/