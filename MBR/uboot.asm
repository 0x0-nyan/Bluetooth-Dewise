[bits 16]
[org 0x7C00]

mov al, 'T'
call PrintCharacter
mov al, 'h'
call PrintCharacter
mov al, 'e'
call PrintCharacter
mov al, ' '
call PrintCharacter
mov al, 'B'
call PrintCharacter
mov al, 'l'
call PrintCharacter
mov al, 'u'
call PrintCharacter
mov al, 'e'
call PrintCharacter
mov al, 't'
call PrintCharacter
mov al, 'o'
call PrintCharacter
mov al, 'o'
call PrintCharacter
mov al, 't'
call PrintCharacter
mov al, 'h'
call PrintCharacter
mov al, ' '
call PrintCharacter
mov al, 'd'
call PrintCharacter
mov al, 'e'
call PrintCharacter
mov al, 'w'
call PrintCharacter
mov al, 'i'
call PrintCharacter
mov al, 's'
call PrintCharacter
mov al, 'e'
call PrintCharacter
mov al, ' '
call PrintCharacter
mov al, 'a'
call PrintCharacter
mov al, 's'
call PrintCharacter
mov al, ' '
call PrintCharacter
mov al, 'c'
call PrintCharacter
mov al, 'o'
call PrintCharacter
mov al, 'n'
call PrintCharacter
mov al, 'n'
call PrintCharacter
mov al, 'e'
call PrintCharacter
mov al, 'c'
call PrintCharacter
mov al, 't'
call PrintCharacter
mov al, 'e'
call PrintCharacter
mov al, 'd'
call PrintCharacter
mov al, ' '
call PrintCharacter
mov al, 'u'
call PrintCharacter
mov al, 'h'
call PrintCharacter
mov al, 'h'
call PrintCharacter
mov al, ' '
call PrintCharacter
mov al, 's'
call PrintCharacter
mov al, 'u'
call PrintCharacter
mov al, 'c'
call PrintCharacter
mov al, 'c'
call PrintCharacter
mov al, 'e'
call PrintCharacter
mov al, 's'
call PrintCharacter
mov al, 's'
call PrintCharacter
mov al, 'f'
call PrintCharacter
mov al, 'u'
call PrintCharacter
mov al, 'l'
call PrintCharacter
mov al, 'l'
call PrintCharacter
mov al, 'y'
call PrintCharacter
mov al, '.'
call PrintCharacter
jmp $

PrintCharacter:
mov ah, 0x0E
mov bh, 0x00
mov bl, 0x07

int 0x10
ret

times 510 - ($ - $$) db 0
dw 0xAA55