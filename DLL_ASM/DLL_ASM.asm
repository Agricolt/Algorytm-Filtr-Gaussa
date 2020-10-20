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
trojka qword ?
poczatek_tablicy qword ?
iterator qword ?

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
imul r8, rdx
mov trojka, r8
mov _height, rdx
mov poczatek_tablicy, rcx ;odwolanie sie poprzez poczatek_tablicy jako adresu nie dziala

;*******************glowna petla filtru******************
;						pierwszy w RCX, drugi RDX, trzeci R8, czwarty R9, piaty w R10
;void DLL_C_API filtrGaussa(byte *bgr, int height, int width, int od, int to)

;mov al, byte PTR[poczatek_tablicy] z jakiegos powodu to nie dziala mimo ze w zmiennej jest dobry adres
;mov al, byte PTR[rcx] ; za to to dziala mimo ze w rcx i w poczatek_tablicy jest ten sam adres

POCZATEK_PETLI:
;mov byte PTR[rcx+4], 5
;mov r13, my_arr
;imul eax, DWORD PTR[r13+16]
;Tutaj czesc wykonawcza petli, RCX (r9) to iterator podstawowy (i), w R12 bedziemy liczyli kolejne wartosci iteratora
;pierwszy if
mov iterator, r9
mov R12, r9
sub r12, pixs
sub r12, 3
cmp r12, 0
jb WARUNEK_NIESPELNIONY
cmp r12, trojka
ja WARUNEK_NIESPELNIONY

;koniec pierwszego ifa
;poczatek drugiego ifa
add r12, 6 ;3 ktore odjelismy wczesniej i 3 dodatkowe
cmp r12, 0
jb WARUNEK_NIESPELNIONY
cmp r12, trojka
ja WARUNEK_NIESPELNIONY
;koniec drugiego ifa
;poczatek 3 ifa
add R12, pixs
add r12, pixs
cmp R12, 0
jb WARUNEK_NIESPELNIONY
cmp r12, trojka
ja WARUNEK_NIESPELNIONY
;koniec 3 ifa
;poczatek 4 ifa
sub R12, 6
cmp R12, 0
jb WARUNEK_NIESPELNIONY
cmp r12, trojka
ja WARUNEK_NIESPELNIONY
;koniec 4 ifa, jesli wszystkie warunki zostaly spelnione wykonuj dalej:
xor rax, rax
mov al, byte ptr[rcx + r9]

WARUNEK_NIESPELNIONY: ;jesli ktorys z warunkow nie zostal spelniony
inc r9
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