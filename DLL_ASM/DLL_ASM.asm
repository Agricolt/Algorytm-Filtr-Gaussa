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
;przypisanie wartosci zmiennym
mov od, r9
mov to, r10
add r8, 2
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
mov r14, pixs
mov r15, trojka
;WCZYTANIE KERNELI POZA PETLE (nie musi byc za kazdym razem wczytywany)
movdqu xmm1, oword ptr[my_arr] ;wczytujemy od razu caly mozliwy kernel (16 bajtow kernela)
movdqu xmm2, oword ptr[my_arr + 18] ;wczytujemy dalsza czesc kernela o przesuniecie poprzednio przetworzonego czyli 8 * 2 + 2 = 18 bajtow
movdqu xmm3, oword ptr[my_arr + 36] ;34 + 2
POCZATEK_PETLI:
;Tutaj czesc wykonawcza petli, RCX (r9) to iterator podstawowy (i)
;dolny rzad
mov r11, rcx
add r11, r9
sub r11, r14
sub r11, 3
movdqu xmm0, [r11] ;wczytujemy 16 bajtow
pmovzxbw xmm0, xmm0 ;konwertujemy pierwsze 8 bajtow z BYTE na WORD
pmullw xmm0, xmm1 ;mnozymy 8 bajtow przez kernel ;wynik w xmm0
;teraz trzeba sie zajac ostatnim bajtem (3 piksele to 9 bajtow BGRBGRBGR)
movzx rsi, byte ptr [r11 + 8]
movzx rax, word ptr[my_arr + 16] ;wczytujemy kolejne 2 bajty kernela (teraz juz przetworzone 18 bajtow)
imul rsi, rax ;w rsi bedzie 9 bajt
;koniec dolnego rzedu
;srodkowy rzad
add r11, r14
movdqu xmm4, [r11] ;wczytujemy 16 bajtow
pmovzxbw xmm4, xmm4 ;konwertujemy pierwsze 8 bajtow z BYTE na WORD
pmullw xmm4, xmm2 ;mnozymy 8 bajtow przez kernel ;wynik w xmm4
;teraz trzeba sie zajac ostatnim bajtem (3 piksele to 9 bajtow BGRBGRBGR)
movzx r13, byte ptr [r11 + 8]
movzx rax, word ptr[my_arr + 34] ; 18 przesuniecia + 16 wczytanych
imul r13, rax ;w r13 bedzie 9 bajt
;koniec srodkowego rzedu
;gorny rzad
add r11, r14
movdqu xmm5, [r11] ;wczytujemy 16 bajtow
pmovzxbw xmm5, xmm5 ;konwertujemy pierwsze 8 bajtow z BYTE na WORD
pmullw xmm5, xmm3 ;mnozymy 8 bajtow przez kernel ;wynik w xmm5
;teraz trzeba sie zajac ostatnim bajtem (3 piksele to 9 bajtow BGRBGRBGR)
movzx rbp, byte ptr [r11 + 8]
movzx rax, word ptr[my_arr + 50] ; 34 przesuniecia + 16 wczytanych
imul rbp, rax ;w rbp bedzie 9 bajt
;koniec gornego  rzedu
;sumowanie
;RAX - R
;RBX - B
;RDI - G
mov rax, rsi
add rax, r13
add rax, rbp
paddw xmm0, xmm4
paddw xmm0, xmm5 

;			RAX		xmm0 
;			(R)	 BG RGB RGB
;
;
movd rdx, xmm0
;dolna polowka rejestru xmm
movzx rbx, dx ;w rbx B

shr rdx, 16
movzx rdi, dx ;w rdi G

shr rdx, 16
add ax, dx ;w rsi R

shr rdx, 16
add bx, dx

pextrq  rdx,  xmm0, 1
;teraz przetwarzamy gorna polowke rejestru xmm
add di, dx

shr rdx, 16
add ax, dx

shr rdx, 16
add bx, dx

shr rdx, 16
add di, dx

;podizelic na 16
shr rax, 4 
shr rbx, 4
shr rdi, 4
;zapis
mov [rcx+ r9], bx
mov [rcx+ r9 + 1], di
mov [rcx+ r9 + 2], ax
;koniec zapisu
WARUNEK_NIESPELNIONY: ;jesli ktorys z warunkow nie zostal spelniony
add r9, 1
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