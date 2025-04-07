; Testing assembler with macros and various commands

.entry START
.extern GLOBAL_LABEL

; Macro definition
mcro PRINT_MACRO
    prn r1
    prn r2
    prn r3
mcroend

; Main program starts here
START: mov r4,GLOBAL_LABEL
       PRINT_MACRO
       add r5,r6
       sub r7,r1
       jmp FUNC

; Function definition
FUNC: PRINT_MACRO
      inc r2
      dec r3
      jsr LOOP
      rts

; Loop with conditional branching
LOOP: cmp r1,r4
      bne GLOBAL_LABEL
      clr r5
      stop

; Data and strings
MESSAGE: .string "macro test"
VALUES: .data 1,-2,3,4
.extern EXTERNAL_VAR