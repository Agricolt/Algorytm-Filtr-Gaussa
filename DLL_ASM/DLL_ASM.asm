.const

.data
my_oword oword 12297848147757817309

my_arr:
word 1 
word 1
word 1
word 2
word 2
word 2
word 1
word 1
word 1
word 2
word 2
word 2
word 4
word 4
word 4
word 2
word 2
word 2
word 1
word 1
word 1
word 2
word 2
word 2
word 1
word 1
word 1
my_arr_size dword 36 ;rozmiar tablicy w bajtach
.data?

od qword ?
to qword ?
_height qword ?
pixs qword ?
trojka qword ?
poczatek_tablicy qword ?

.code
filtrGaussa proc e: DWORD
;Zachowaj nieulotne rejestry na stosie
xor r10, r10
mov r10d, dword PTR[rbp + 48] ;przenies 5. ty argument do rejestru r10

push rbx
push rbp
push rdi
push rsi
push rsp
push r12
push r13
push r14
push r15
;******************GLOWNY PROGRAM*****************
add rdx, 2
add r8, 2
;przypisanie wartosci zmiennym
mov od, r9
mov to, r10
imul r8, 3
mov pixs, r8
imul r8, rdx
mov trojka, r8
mov _height, rdx
mov poczatek_tablicy, rcx ;odwolanie sie poprzez poczatek_tablicy jako adresu nie dziala

;*******************glowna petla filtru******************
;						pierwszy w RCX, drugi RDX, trzeci R8, czwarty R9, piaty w R10
;void DLL_C_API filtrGaussa(byte *bgr, int height, int width, int od, int to)

;mov al, byte PTR[poczatek_tablicy] z jakiegos powodu to nie dziala mimo ze w zmiennej jest dobry adres
;mov al, byte PTR[rcx] ; za to to dziala mimo ze w rcx i w poczatek_tablicy jest ten sam adres
;WCZYTANIE KERNELI POZA PETLE (nie musi byc za kazdym razem wczytywany)
vmovdqu ymm1, ymmword ptr[my_arr] ;wczytujemy od razu caly mozliwy kernel (16 bajtow kernela)
vmovdqu ymm2, ymmword ptr[my_arr + 18] ;wczytujemy dalsza czesc kernela o przesuniecie poprzednio przetworzonego czyli 8 * 2 + 2 = 18 bajtow
vmovdqu ymm3, ymmword ptr[my_arr + 36] ;34 + 2

mov r14, pixs
add r14, 3
mov r15, pixs
POCZATEK_PETLI:
;Tutaj czesc wykonawcza petli, RCX (r9) to iterator podstawowy (i), w R12 bedziemy liczyli kolejne wartosci iteratora
;pierwszy if
;koniec 4 ifa, jesli wszystkie warunki zostaly spelnione wykonuj dalej:
;dolny rzad
mov r11, rcx
add r11, r9
sub r11, r14
movdqu xmm0, [r11] ;wczytujemy 16 bajtow
vpmovzxbw ymm0, xmm0 ;konwertujemy pierwsze 9 bajtow z BYTE na WORD
;teraz mamy sytuacje ze wartosci pikseli sa 16bitowe i w pamieci wygladaja tak
;BGRB GRBG R**** ****
vpmullw ymm0, ymm0, ymm1
;koniec dolnego rzedu
;srodkowy rzad
add r11, r15
movdqu xmm4, [r11] ;wczytujemy 16 bajtow
vpmovzxbw ymm4, xmm4 ;konwertujemy pierwsze 9 bajtow z BYTE na WORD
vpmullw ymm4, ymm4, ymm2
;koniec srodkowego rzedu
;gorny rzad
add r11, r15
movdqu xmm5, [r11] ;wczytujemy 16 bajtow
vpmovzxbw ymm5, xmm5 ;konwertujemy pierwsze 9 bajtow z BYTE na WORD
vpmullw ymm5, ymm5, ymm3
;koniec gornego  rzedu
;sumowanie
vpaddw ymm0, ymm0, ymm4
vpaddw ymm0, ymm0, ymm5
;BGRB GRBG R*** ****
;***B GRBG RBGR ****
;**** **BG RBGR BGR*
;przesun w prawo
vPSRLW ymm4, ymm0, 4

;koniec zapisu
add r9, 3
cmp r9, r10 ;jesli rejestry sa rowne to znaczy ze przetwarzanie dotarlo do konca tabeli
jne POCZATEK_PETLI
;***************koniec glownej petli filtru**************

;przywroc nieulotne rejestry ze stosu
pop r15
pop r14
pop r13
pop r12
pop rsp
pop rsi
pop rdi
pop rbp
pop rbx
ret
filtrGaussa endp
end