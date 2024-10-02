# CPU Emulator Project

## Overview

This project is a basic **CPU Emulator** based on a subset of the ARM architecture. The emulator can fetch, decode, and execute machine code instructions, simulating the behavior of a CPU. It includes virtual memory, register management, arithmetic operations, and branching instructions. The project follows a three-step approach: instruction decoding, basic emulation, and full emulation, as described in the assignment requirements.

## Features

- **Instruction Decoding and Disassembly**: Converts ARM machine code instructions to human-readable assembly language.
- **Basic Emulation**: Simulates basic operations like `MOV`, `ADD`, `SUB`, `CMP`, and other arithmetic and logical operations.
- **Full Emulation**: Supports memory access (LDR, STR), branching (B, BL), and status flag checks (Zero, Negative, Carry, Overflow).
- **Virtual Memory**: A 128KB virtual RAM is implemented for storing machine code and memory data.
- **Register Emulation**: Simulates 16 general-purpose registers (R0 to R15), including special registers like the stack pointer (R13), link register (R14), and program counter (R15).
- **Instruction Cycle Timing**: Tracks the number of CPU cycles consumed during instruction execution.

## Project Structure

### Main Components

1. **RAM and Registers Initialization**:
   - Initializes a virtual RAM of 128KB to store memory and instructions.
   - Initializes 16 registers with specific values for R13 (stack pointer) and R15 (program counter).

2. **Instruction Decoding**:
   - Fetches instructions from RAM and decodes hexadecimal machine code into binary, extracting opcode bits and operands for further processing.

3. **Instruction Execution**:
   - Executes decoded instructions, updating register and memory values.
   - Simulates arithmetic operations (`ADD`, `SUB`), logical operations (`MOV`, `CMP`), and memory access (`LDR`, `STR`).

4. **Flags and Conditions**:
   - Implements CPU flags (Zero, Negative, Carry, Overflow) based on instruction results. These flags are used for conditional execution and branching.

5. **Supported Instructions**:
   - Supports a subset of the ARM instruction set, including arithmetic, memory access, and branch instructions.

6. **Cycle Timing**:
   - Tracks the number of cycles each instruction takes and keeps a running total of elapsed time during the emulation.

### Supported Instructions

The emulator supports the following ARM instructions:

- **Arithmetic/Logical Instructions**:
  - `MOV`, `ADD`, `SUB`, `CMP`
- **Branching Instructions**:
  - `B`, `BL` (Branch and Branch with Link)
- **Memory Access Instructions**:
  - `LDR`, `STR`
- **Special Instructions**:
  - `SWI` (Software Interrupt)

### Flags

The emulator checks and sets the following CPU flags based on operation results:

- **Zero Flag**: Set when the result is zero.
- **Negative Flag**: Set when the most significant bit of the result is 1.
- **Carry Flag**: Set when an operation results in a carry or borrow.
- **Overflow Flag**: Set when an arithmetic operation overflows the signed result.

## How to Run

### Prerequisites

- A C++ compiler, such as `g++`, is required to compile the project.

### Compilation

To compile the code, use the following command:

```bash
g++ -o cpu_emulator cpu_emulator.cpp
