; fibonacci program

.extern N

mcro init
    mov r0, #0
    mov r1, #1
    mov r2, #1

    ; r3 holds the fibonacci number we want
    mov r3, N
mcroend

init

FIB: 
    dec r3

    ; dec loop register
    mov r4, #0
    add r4,r1
    add r4,r2

    ; sum them both
    mov r1,r2
    mov r2,r4

    not r3
    jmp LOOP

; print the 6th fibonacci number
prn r2
stop

.entry FIB