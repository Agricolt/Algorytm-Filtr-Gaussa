#include "MAIN.h"

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
	for (int j = 0; j < height*width*3; j++)
	{
		std::cout << (short)temp1[j] << ", ";
	}
	std::cout << std::endl;
}


bool wczytajBitmape(int &width, int &height, int &padding, byte *&bgr, std::string &komunikat, BITMAPFILEHEADER *&header,
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

	/*Sprawdz czy jest to blik .bmp (2 pierwsze bajty to litery B i M*/
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
		padding += 4 - ((info_header->biWidth*3) % 4);

	width = info_header->biWidth;
	height = info_header->biHeight;
	short rozmiar_bitmapy_noPadding = height * width;
	short rozmiar_bitmapy_cala = rozmiar_bitmapy_noPadding + (padding * width);
	int jeden_wiersz = 3 * width; //3 piksele razy dlugosc jednego wiersza

	/*Tablica przechowuje informacje o pikselach od do³u prawego do góry lewego w formacie b, g, r, b, g, r itd.*/
	bgr = new byte[height*width*3];
	ifs.seekg(header->bfOffBits, ios::beg); // bfOffBits wskazuje pocz¹tek danych obrazka

	for (int i = 0; i < height; i++)
	{
		char * temp1 = new char[jeden_wiersz];
		ifs.read(temp1, jeden_wiersz);
		ifs.seekg(padding, std::ios::cur);
		byte *temp2 = (byte*)(temp1);
		for (int j = 0; j < jeden_wiersz; j++)
		{
			bgr[j + i*jeden_wiersz] = temp2[j];
		}
	}
	return true;
	/*KONIEC ODCZYTU*/
}


bool zapiszBitmape(int &width, int &height, int &padding, byte *&bgr, std::string &komunikat, BITMAPFILEHEADER *&header,
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

	int ktora_linia = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < jeden_wiersz; j++)
		{
			zapis[j + i*jeden_wiersz] = bgr[j + i*jeden_wiersz];
			ktora_linia = j + i*jeden_wiersz;
		}
		for (int j = 1; j <= padding; j++)
		{
			zapis[ktora_linia + j] = 0;
		}
	}
	wy.write(zapis, ilosc_bajtow);
	wy.close();
	return true;
}


//ktora_biblioteka = true - DLL_C
//ktora_biblioteka = false - DLL_ASM
void utworzWatki(byte ilosc_watkow, byte *bgr, int height, int width, bool ktora_biblioteka)
{
	HMODULE dll_c, dll_asm;
	filtrGaussaC filtr_c;
	filtrGaussaAsm filtr_asm;
	std::thread *tab = new std::thread[ilosc_watkow];

	int na_jeden_watek = height * width * 3 / ilosc_watkow; //50*50 * 3/4 = 1875
	int reszta = height % ilosc_watkow; //50%4 = 2

	//Tablice tablic podzielonych danych ktore zostana przekazane do funkcji rozmycia
	if (ktora_biblioteka == true)
	{
		if ((dll_c = LoadLibrary(L"DLL_C.dll")) != NULL) {
			filtr_c = (filtrGaussaC)GetProcAddress(dll_c, "filtrGaussa");
			if (filtr_c != NULL) {
				/*Jesli poprawnie wczytano biblioteke i odnaleziono funkcje to kontynuuj*/
				tab = new std::thread[ilosc_watkow];
				int od = 0, to = 0;
				for (int i = 0, j = 0; i < ilosc_watkow, j < (height * width * 3); i++, j += na_jeden_watek)
				{
					od = j;
					to = j + na_jeden_watek;
					tab[i] = std::thread(filtr_c, bgr, height, width, od, to);
				}
				for (int i = 0; i < ilosc_watkow; i++)
				{
					tab[i].join();
				}
			}
		}
	}
	else
	{
		if ((dll_asm = LoadLibrary(L"DLL_ASM.dll")) != NULL) {
			filtr_asm = (filtrGaussaAsm)GetProcAddress(dll_asm, "filtrGaussa");
			if (filtr_asm != NULL) {
				/*Jesli poprawnie wczytano biblioteke i odnaleziono funkcje to kontynuuj*/
				tab = new std::thread[ilosc_watkow];
				int od = 0, to = 0;
				for (int i = 0, j = 0; i < ilosc_watkow, j < (height * width * 3); i++, j += na_jeden_watek)
				{
					od = j;
					to = j + na_jeden_watek;
					tab[i] = std::thread(filtr_asm, bgr, height, width, od, to);
				}
				for (int i = 0; i < ilosc_watkow; i++)
				{
					tab[i].join();
				}
			}
		}
	}
}

int main()
{
	std::clock_t start = std::clock();
	byte *bgr = nullptr;
	int width = 0, height = 0, padding = 0;
	byte watki = ILOSC_WATKOW; // thread::hardware_concurrency(); //rowzniez mozliwosc podania liczby watkow przez uzytkownika
	std::string komunikat = "Liczba wykrytych watkow: " + to_string(watki) + "\n";
	BITMAPFILEHEADER *header = nullptr;
	BITMAPINFOHEADER *info_header = nullptr;

	wczytajBitmape(width, height, padding, bgr, komunikat, header, info_header);

	for (int i = 0; i < ILOSC_POWTORZEN; i++)
	{
		utworzWatki(watki, bgr, height, width, CPP);
	}

	zapiszBitmape(width, height, padding, bgr, komunikat, header, info_header);
	/*Usuwanie obiektow z pamieci*/
	delete[] bgr;
	delete header;
	delete info_header;
	/*Koniec usuwania*/
	cout << komunikat;
	std::cout << "Czas wykonania programu: " << ((1000 * (std::clock() - start) / CLOCKS_PER_SEC))/1000 << std::endl;
	getchar();
}
