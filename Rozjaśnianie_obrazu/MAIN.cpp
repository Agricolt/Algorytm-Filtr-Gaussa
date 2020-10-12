#include "MAIN.h"

#define NAZWA_PLIKU "2x2.bmp"
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


void wyswietl_tablice_koloru(int height, int width, byte * temp1)
{
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			std::cout << (short)temp1[(j*width) + i] << ", ";
		}
	}
}


bool wczytajBitmape(int & width, int & height, int &padding, byte * &b, byte * &g, byte * &r, std::string & komunikat, BITMAPFILEHEADER *&header,
	BITMAPINFOHEADER *&info_header)
{
	/*ODCZYT*/
	ifstream ifs(NAZWA_PLIKU, ios::binary);
	/*Wczytanie naglowka pliku*/
	char* temp = new char[sizeof(BITMAPFILEHEADER)];
	if (ifs.good() == false) {
		komunikat += "Nie udalo sie otworzyc pliku .bmp. Plik nie istnieje badz nie masz wystarczajacych uprawnien. Anulowanie.\n";
		return false;
	}
	komunikat += "Otwarto plik\n";
	ifs.read(temp, sizeof(BITMAPFILEHEADER));
	header = (BITMAPFILEHEADER*)(temp);
	byte c1, c2;
	c1 = (header->bfType) & 0xFF;	//Wyciagnij pierwszy bajt
	c2 = ((header->bfType) >> 8) & 0xFF;//Wyciagnij drugi bajt
	if (c1 == 'B' && c2 == 'M')
		komunikat += "Plik jest plikiem w formacie .bmp\n";
	else {
		komunikat += "Plik nie jest plikiem w formacie .bmp. Anulowanie";
		return false;
	}

	/*Wczytanie naglowka obrazu*/
	temp = new char[sizeof(BITMAPINFOHEADER)];
	ifs.read(temp, sizeof(BITMAPINFOHEADER));
	info_header = (BITMAPINFOHEADER*)(temp);

	/*Wczytanie bitmapy*/
	//Obliczenie wyrownania do 4 bitow
	padding = 0;
	if (info_header->biWidth % 4 == 0)
		padding = 0;
	else
		padding += 4 - (info_header->biWidth % 4);

	width = info_header->biWidth;
	height = info_header->biHeight;
	short rozmiar_bitmapy_noPadding = height * width;
	short rozmiar_bitmapy_cala = rozmiar_bitmapy_noPadding + (padding * width);
	int jeden_wiersz = 3 * width;

	b = new byte[height*width];
	g = new byte[height*width];
	r = new byte[height*width];

	ifs.seekg(header->bfOffBits, ios::beg); // bfOffBits wskazuje pocz¹tek danych obrazka
	for (int i = 0; i < height; i++)
	{
		int pixel = 0;
		char * temp1 = new char[jeden_wiersz];
		ifs.read(temp1, jeden_wiersz);
		ifs.seekg(padding, std::ios::cur);
		byte *temp2 = (byte*)(temp1);
		for (int k = 0; k < jeden_wiersz; k = k + 3)
		{
			/*tablica 2-wymiarowa w jednowymiarowej i - ktory wiersz iterujemy * szerokosc wiersza (tyle przeskakujemy)
			+ indeks danej
			np. dla bitmapy 2x2
			i = 0 (wiersz zerowy (najnizszy)
			width = szerokosc obrazka (wiersza) w pixelach = 2
			i*width = 0 czyli zerowy wiersz
			+pixel = 0 czyli element [0][0] */
			b[(i*width) + pixel] = temp2[k];
			g[(i*width) + pixel] = temp2[k + 1];
			r[(i*width) + pixel] = temp2[k + 2];
			pixel++;
		}
	}
	return true;
	/*KONIEC ODCZYTU*/
}


bool zapiszBitmape(int & width, int & height, int &padding, byte * &b, byte * &g, byte * &r, std::string & komunikat, BITMAPFILEHEADER *&header,
	BITMAPINFOHEADER *&info_header)
{
	/*ZAPIS*/
	ofstream wy("zapis.bmp", ios::binary);
	//Zapis naglowka pliku
	wy.write((char*)(header), sizeof(BITMAPFILEHEADER));
	//Zapis naglowka bitmapy
	wy.write((char*)(info_header), sizeof(BITMAPINFOHEADER));
	//Zapis pixeli bitmapy
	int jeden_wiersz = width * 3;
	int ilosc_bajtow = jeden_wiersz * width + padding * width;
	char * zapis = new char[ilosc_bajtow];
	int znak = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			zapis[znak] = b[(i*width) +j];
			zapis[znak + 1] = g[(i*width) + j];
			zapis[znak + 2] = r[(i*width) + j];
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
	return true;
}


void utworzWatki(int ilosc_watkow, std::thread *& tab)
{
	tab = new std::thread[ilosc_watkow];
}

int main()
{
	byte *b = nullptr;
	byte *g = nullptr;
	byte *r = nullptr;
	int width = 0, height = 0, padding = 0;
	std::string komunikat = "Liczba wykrytych watkow: " + to_string(thread::hardware_concurrency()) + "\n";
	BITMAPFILEHEADER *header = nullptr;
	BITMAPINFOHEADER *info_header = nullptr;

	wczytajBitmape(width, height, padding, b, g, r, komunikat, header, info_header);

	/*Uzycie funkcji z .dll'ek*/
	typedef void(*filtrGaussa)(byte *, byte *, byte *, int, int);

	HMODULE dll;
	filtrGaussa filtr;
	if ((dll = LoadLibrary(L"DLL_C.dll")) != NULL)
	{
		filtr = (filtrGaussa)GetProcAddress(dll, "filtrGaussa");
		if (filtr != NULL)
		{
			filtr(b, g, r, height, width);
		}
		FreeLibrary(dll);
	}
	zapiszBitmape(width, height, padding, b, g, r, komunikat, header, info_header);
	/*Usuwanie obiektow z pamieci*/
	delete[] b;
	delete[] g;
	delete[] r;
	delete header;
	delete info_header;
	/*Koniec usuwania*/
	cout << komunikat;
	getchar();
}
