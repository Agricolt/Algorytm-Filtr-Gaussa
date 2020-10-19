.const

.data

my_arr:
dd 1
dd 2
dd 1
dd 2
dd 4
dd 2
dd 1
dd 2
dd 1
my_arr_size dword 36 ;rozmiar tablicy w bajtach
.data?
suma_matrix dword ?

od qword ?
to qword ?
_height qword ?
pixs qword ?
poczatek_tablicy qword ?

.code
filtrGaussa proc e: DWORD
;Zachowaj nieulotne rejestry na stosie

mov r10, qword PTR[rbp + 48] ;przenies 5. ty argument do rejestru r10

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
;petla zliczajaca sume wag w macierzy
mov rdi, my_arr
mov rbp, my_arr
add ebp, my_arr_size
xor rax, rax
@@:
add eax, [rdi]
add rdi, 4
cmp edi, ebp
jne @b
;koniec petli
;zapisz sume macierzy
mov suma_matrix, eax

;przypisanie wartosci zmiennym
mov od, r9
mov to, r10
imul r8, 3
mov pixs, r8
mov _height, rdx
mov poczatek_tablicy, rcx ;odwolanie sie poprzez poczatek_tablicy jako adresu nie dziala
add rcx, r9 ;dodanie przemieszczenia do wskaznika na pierwszy element

mov r11, r10
add r11, rcx
;r11 zawiera teraz adres ostatniego elementu ktory ma byc przetwrzany

;*******************glowna petla filtru******************
;pierwszy w RCX, drugi RDX, trzeci R8, czwarty R9, piaty w R10
;void DLL_C_API filtrGaussa(byte *bgr, int height, int width, int od, int to)

;mov al, byte PTR[poczatek_tablicy] z jakiegos powodu to nie dziala mimo ze w zmiennej jest dobry adres
;mov al, byte PTR[rcx] ; za to to dziala mimo ze w rcx i w poczatek_tablicy jest ten sam adres

POCZATEK_PETLI:
mov byte PTR[rcx], 5
mov r13, my_arr
imul eax, DWORD PTR[r13+16]
;Tutaj czesc wykonawcza petli, RCX to iterator podstawowy (i), w R12 bedziemy liczyli kolejne wartosci iteratora
;pierwszy if
mov R12, rcx
sub R12, pixs
sub R12, 3
cmp R12, rcx
jae WARUNEK_NIESPELNIONY
jbe WARUNEK_NIESPELNIONY
;koniec pierwszego ifa
;poczatek drugiego ifa
add R12, 6 ;3 ktore odjelismy wczesniej i 3 dodatkowe
cmp R12, rcx
jae WARUNEK_NIESPELNIONY
jbe WARUNEK_NIESPELNIONY
;koniec drugiego ifa
;poczatek 3 ifa
add R12, pixs
cmp R12, rcx
jae WARUNEK_NIESPELNIONY
jbe WARUNEK_NIESPELNIONY
;koniec 3 ifa
;poczatek 4 ifa
sub R12, 6
cmp R12, rcx
jae WARUNEK_NIESPELNIONY
jbe WARUNEK_NIESPELNIONY
;koniec 4 ifa, jesli wszystkie warunki zostaly spelnione wykonuj dalej:


WARUNEK_NIESPELNIONY: ;jesli ktorys z warunkow nie zostal spelniony
inc rcx
cmp rcx, r11 ;jesli rejestry sa rowne to znaczy ze przetwarzanie dotarlo do konca tabeli
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