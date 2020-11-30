
# [PL] Algorytm-Filtr-Gaussa
Filtr Gaussa napisany w języku C++ oraz masm64.
Projekt stworzony został z myślą o porównaniu szybkości działania zwykłych instrukcji C++ z instrukcjami SIMD w języku asemblera.

# [ENG] Gaussian Blur algorithm

Gaussian blur algorithm written in C++ and assembly (masm64)
The project is designed to compare the speed of simple C++ instructions and assembly SIMD instructions.

### Spis treści:
1. [Przykład działania](#examplepl)    
2. [Wyniki czasowe wykonania](#statspl)
3. [Przydatne linki](#references)

### Table of contents
1. [An example](#exampleeng)    
2. [Execution time results](#statseng)
3. [Useful references](#references)

## [PL]

<a name="examplepl"/>
1. Przykład działania

![Przykładowa mapa bitowa przed i po przetworzeniu. ](https://iili.io/FN0cRS.png)
Przykładowa mapa bitowa przed i po przetworzeniu (oba algorytmy dają ten sam rezultat). Krawędzie obrazu nie są przetwarzane.
<a name="statspl"/>

2. Wyniki czasowe wykonania

![enter image description here](https://iili.io/FN0Mib.png)

Wyniki czasowe dla mapy bitowej o rozmiarze 220MB. Program wykonywany na komputerze ze 6 rdzeniami.
Program napisany w asemblerze był średnio 1.3 razy wolniejszy od programu w C++. 

Dlaczego program w asemblerze był wolniejszy mimo wykorzystania instrukcji SIMD?
a) Kompilatory języków wysokiego poziomu bardzo dobrze optymalizują kod wynikowy (optymalizacja dostępów do pamięci, skoków itd.)
b) Niedoskonałość programu w asemblerze i mierne wykorzystanie rejestrów XMM (jeden piksel na jeden rejestr - wykorzystane 6 bajtów z 16 dostępnych.

Program napisany w C++ z wykorzystaniem rozkazów wektorowych (Intrinsics) byłby programem najszybszym.

## [ENG]

<a name="exampleeng"/>
1. An example

![enter image description here](https://iili.io/FN0cRS.png)
Sample bitmap, before and after processing (both algorithms give the same output). The edges are not processed.

<a name="statseng"/>
3. Execution Time Results

![enter image description here](https://iili.io/FN102R.png)

Time results for 220MB bitmap. Program was executed on a 6-core processor.
Program written in assembly was on average 1.3 times slower than program written in C++.

Why is assembly program slower than program in C++, despite using SIMD instructions?
a) High-level languages compilers are optimizing the resultant bytecode very well (memory acces time optimization, jumps optimizations etc.)
b) The imperfection of assembly program and mediocre use of SIMD XMM registers (one pixel to one register - only 6 bytes are used of all 16.)

Program written in C++ using SIMD Intrinsics would be the fastest program.

<a name="references"/>
4. Przydatne linki / Useful References

http://www.algorytm.org/przetwarzanie-obrazow/filtrowanie-obrazow.html [PL]
http://informatyka.wroc.pl/node/443?page=0,2 [PL]
https://stackoverflow.com/questions/1696113/how-do-i-gaussian-blur-an-image-without-using-any-in-built-gaussian-functions [ENG]
https://stackoverflow.blog/2020/07/08/improving-performance-with-simd-intrinsics-in-three-use-cases/ [ENG]
https://en.wikipedia.org/wiki/BMP_file_format [ENG]
http://www.cplusplus.com/articles/G8hv0pDG/ :) [ENG]
