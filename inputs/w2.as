; Testing assembler with a mix of commands and directives

.entry BEGIN
.extern GLOBAL_VAR

; Program starts here
BEGIN: cmp r2,r3
       dec r4
       jsr FUNC
       clr r5
       red r6
       jmp LOOP

; Function definition
FUNC: not r1
      add r2,r7
      prn #12
      rts

; Loop with conditional branching
LOOP: bne GLOBAL_VAR
      sub r3,r4
      stop

; Data and strings
TEXT: .string "assembler"
NUMBERS: .data 5,15,-25,35
.extern EXTERNAL_FUNC