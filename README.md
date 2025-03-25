# ⚙️ Assembler 🚀

## 📖 Overview
This repository contains an assembler that translates an invented assembly language into machine code instructions in hexadecimal form. The assembler is designed for a custom CPU model with 24-bit words and supports a set of fundamental instructions, variable declarations, labels, and macros.

## ✨ Features
- 🔄 Converts assembly code into hexadecimal machine code.
- 🎯 Supports custom opcodes and `funct` values.
- 💾 Implements a register-based architecture with 24-bit words.
- 🏷️ Supports variable declaration, labels, and macros.
- 📜 Implements a comprehensive instruction set.

## 🔢 Supported Instructions
The assembler supports the following instructions:

### 🔹 Two-Operand Instructions
- `mov` (0, `EMPTY_FUNCT`)
- `cmp` (1, `EMPTY_FUNCT`)
- `add` (2, funct=1)
- `sub` (2, funct=2)
- `lea` (4, funct=0)

### 🔸 One-Operand Instructions
- `clr` (5, funct=1)
- `not` (5, funct=2)
- `inc` (5, funct=3)
- `dec` (5, funct=4)
- `jmp` (9, funct=1)
- `bne` (9, funct=2)
- `jsr` (9, funct=3)
- `red` (12, `EMPTY_FUNCT`)
- `prn` (13, `EMPTY_FUNCT`)

### ⚡ No-Operand Instructions
- `rts` (14, `EMPTY_FUNCT`)
- `stop` (15, `EMPTY_FUNCT`)

## 🔧 Requirements
- 🖥️ C Compiler (e.g., `gcc`)
- 🐧 Linux (Recommended for compatibility)

## 📦 Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/ExpDev-mul/Assembler.git
   cd Assembler
   ```
2. Compile the assembler:
   ```sh
   make
   ```

## ▶️ Usage
To assemble a file:
```sh
./main input.as output.hex
```

### 📝 Example assembly file (`fibonacci.asm`):
```
mov R1, #0    ; First number (Fib[0] = 0)
mov R2, #1    ; Second number (Fib[1] = 1)
mov R3, #10   ; Number of terms (N = 10)
loop:
    prn R1     ; Print current Fibonacci number
    add R1, R2 ; Compute next Fibonacci number
    mov R4, R1 ; Store temporary result
    mov R1, R2 ; Shift R1 to R2
    mov R2, R4 ; Shift R2 to new value
    dec R3     ; Decrease counter
    bne loop   ; Repeat if R3 > 0
stop
```

### 🏁 Output (`output.hex`):
```
0x0100  ; MOV R1, #0
0x0101  ; MOV R2, #1
0x010A  ; MOV R3, #10
0x0D01  ; PRN R1
0x0202  ; ADD R1, R2
0x0104  ; MOV R4, R1
0x0102  ; MOV R1, R2
0x0103  ; MOV R2, R4
0x0504  ; DEC R3
0x0901  ; BNE loop
0x0F00  ; STOP
```

## 📂 File Structure
```
/Assembler
│── header
│   ├── assembler.h      # The main header for the assembler
│   ├── labels.h         # Handles label definitions and lookups
│   ├── lib.h            # Provides utility functions
│   ├── opcode.h         # Stores opcode metadata
│   ├── preprocessing.h  # Functions for preprocessing macros and labels
│   ├── word.h           # Defines 24-bit word storage
│── src
│   ├── assembler.c      # Core assembler implementation
│   ├── labels.c         # Implementation of label handling
│   ├── lib.c            # Library function implementations
│   ├── main.c           # Entry point for the assembler
│   ├── opcode.c         # Opcode processing and metadata retrieval
│   ├── preprocessing.c  # Handles preprocessing operations
│   ├── word.c           # 24-bit word structure operations
│── inputs
│   ├── input.as         # Sample assembly input file
│── Makefile             # Build automation script
│── README.md            # Project documentation
```

## 🤝 Contributing
Contributions are welcome! Feel free to open an issue or submit a pull request.

## 📜 License
This project is licensed under the MIT License. 🔓

