; This file contains many common errors possible

mcro yes
    mov #5,r3
    mov r3,r4
    add r3,r4
mcroend

mcro yes
    add r3,r2
mcroend

; Definition of an entry and extern with the same name
.entry HELLO
.extern HELLO

; The destination operand does not have such an addressing mode
mov r3, #5