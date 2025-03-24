# Custom Assembler

## Overview
This repository contains an assembler that translates an invented assembly language into machine code instructions in hexadecimal form. The assembler is designed for a custom CPU model with 24-bit words and supports a set of fundamental instructions, variable declarations, labels, and macros.

## Features
- Converts assembly code into hexadecimal machine code.
- Supports custom opcodes and `funct` values.
- Implements a register-based architecture with 24-bit words.
- Supports variable declaration, labels, and macros.
- Implements a comprehensive instruction set.

## Supported Instructions
The assembler supports the following instructions:

### Two-Operand Instructions
- `mov` (0, `EMPTY_FUNCT`)
- `cmp` (1, `EMPTY_FUNCT`)
- `add` (2, funct=1)
- `sub` (2, funct=2)
- `lea` (4, funct=0)

### One-Operand Instructions
- `clr` (5, funct=1)
- `not` (5, funct=2)
- `inc` (5, funct=3)
- `dec` (5, funct=4)
- `jmp` (9, funct=1)
- `bne` (9, funct=2)
- `jsr` (9, funct=3)
- `red` (12, `EMPTY_FUNCT`)
- `prn` (13, `EMPTY_FUNCT`)

### No-Operand Instructions
- `rts` (14, `EMPTY_FUNCT`)
- `stop` (15, `EMPTY_FUNCT`)

## Requirements
- C Compiler (e.g., `gcc`)
- Linux (Recommended for compatibility)

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/ExpDev-mul/Assembler.git
   cd Assembler
   ```
2. Compile the assembler:
   ```sh
   make
   ```

## Usage
To assemble a file:
```sh
./assembler input.asm output.hex
```

Example assembly file (`input.asm`):
```
mov R1, #10  ; Move immediate value 10 into register 1
mov R2, R1   ; Copy value from R1 to R2
add R3, R2   ; Add R2 to R3
```

Output (`output.hex`):
```
0x010A  ; MOV R1, #10
0x0201  ; MOV R2, R1
0x0302  ; ADD R3, R2
```

## File Structure
```
/Assembler
│── src
│   ├── assembler.c  # Main assembler implementation
│   ├── lexer.c      # Tokenizes input assembly code
│   ├── parser.c     # Parses assembly instructions
│   └── codegen.c    # Generates machine code
│── include
│   ├── assembler.h  # Header files
│── examples
│   ├── input.asm    # Sample assembly code
│── Makefile
│── README.md
```

## Roadmap
- [ ] Implement additional instructions (`mul`, `div`, `and`, etc.).
- [ ] Enhance error handling and reporting.
- [ ] Develop a simulator for execution testing.
- [ ] Create a GUI for easier usage.

## Contributing
Contributions are welcome! Feel free to open an issue or submit a pull request.

## License
This project is licensed under the MIT License.

