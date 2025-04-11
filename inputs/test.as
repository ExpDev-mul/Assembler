; Test input for assembler

.extern EXTLABEL
.entry START
.entry FUNC1

MAIN:   lea STR, r3
        add r3, r1
        cmp r3, #5
        bne FUNC1
        jsr FUNC1
        prn r3
        stop

STR:    .string "abc123"
NUMS:   .data 5, -7, 15
START:  mov r2, r3
        red r4
        inc r2
        not r5
        clr r1
        dec r4
        jmp LOOP
        rts

LOOP:   cmp #0, r2
        bne MAIN
        sub r3, r2
        prn #-99
        stop

FUNC1:  mov r1, r1
        jmp EXTLABEL
        stop

.extern EXT2
DATA2: .data 0, 10, -10
STR2:  .string "ZXY!"

