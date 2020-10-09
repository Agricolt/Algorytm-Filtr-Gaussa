.const

.data
moje sbyte -55
.data?

.code
MyProc proc
mov rax, rcx
add rax, rdx
xor rax, rax
;wyzsze nie maja znaczenia
mov al, -55
neg al
ret
MyProc endp
end