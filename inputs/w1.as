; Example input file for testing assembler functionality

.entry START
.extern EXTERNAL_LABEL

; Main program starts here
MAIN: mov r3,START
LOOP: add r1,r2
      sub r4,r5
      prn #-5
      bne EXTERNAL_LABEL
      inc COUNT
      jmp END

; Data and string declarations
STR: .string "testing"
START: .data 10,-20,30
COUNT: .data 0

; Additional instructions
END: stop
.extern ANOTHER_LABEL