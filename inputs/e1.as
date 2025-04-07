; This file contains many common errors possible

; Definition of an entry and extern with the same name
.entry HELLO
.extern HELLO

; The destination operand does not have such an addressing mode
mov r3, #5