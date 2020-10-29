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
movdqu xmm0, oword ptr[my_arr] ;wczytujemy 6 bajtow
movdqu xmm1, oword ptr[my_arr + 6]
movdqu xmm2, oword ptr[my_arr + 12] 
movdqu xmm3, oword ptr[my_arr + 18] 
movdqu xmm4, oword ptr[my_arr + 24] 
movdqu xmm5, oword ptr[my_arr + 30] 
movdqu xmm6, oword ptr[my_arr + 36] 
movdqu xmm7, oword ptr[my_arr + 42] 
movdqu xmm8, oword ptr[my_arr + 48] 
;**********************************
;Rejestry od xmm0 do xmm8 wlacznie maja macierz w sobie
;Rejestry od xmm9 do xmm15 maja dane
;*******************************************************
POCZATEK_PETLI:
;Tutaj czesc wykonawcza petli, RCX (r9) to iterator podstawowy (i)
mov r11, rcx
add r11, r9
sub r11, r14
sub r11, 3
pmovzxbw xmm9, [r11]
add r11, 3
pmovzxbw xmm10, [r11]
add r11, 3
pmovzxbw xmm11, [r11]

add r11, r14
pmovzxbw xmm12, [r11]
sub r11, 3
pmovzxbw xmm13, [r11]
sub r11, 3
pmovzxbw xmm14, [r11]

pmullw xmm9, xmm0
pmullw xmm10, xmm1
pmullw xmm11, xmm2

pmullw xmm12, xmm3
pmullw xmm13, xmm4
pmullw xmm14, xmm5

paddw xmm9, xmm10
paddw xmm11, xmm12
paddw xmm9, xmm15
paddw xmm13, xmm14
paddw xmm9, xmm11
paddw xmm9, xmm13

add r11, r14
pmovzxbw xmm10, [r11]
add r11, 3
pmovzxbw xmm11, [r11]
add r11, 3
pmovzxbw xmm12, [r11]

pmullw xmm10, xmm6
pmullw xmm11, xmm7
pmullw xmm12, xmm8

paddw xmm9, xmm10
paddw xmm9, xmm11
paddw xmm9, xmm12
;podziel na 16
psrlw xmm9, 4

pextrw rax, xmm9, 0
pextrw rdx, xmm9, 1
pextrw rdi, xmm9, 2

mov [rcx+ r9], al
mov [rcx+ r9 + 1], dl
mov [rcx+ r9 + 2], dil
WARUNEK_NIESPELNIONY: ;jesli ktorys z warunkow nie zostal spelniony
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