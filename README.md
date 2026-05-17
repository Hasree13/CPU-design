# RISC-V Pipelined CPU Simulator

A C++ implementation of a **5-stage pipelined RISC-V processor simulator** with:

- Operand Forwarding
- Hazard Detection
- Register Locking
- Branch & Jump Handling
- Assembler for converting RISC-V assembly to binary machine code

This project simulates the execution of RISC-V instructions through a pipelined datapath and demonstrates how operand forwarding minimizes stalls caused by data hazards.

---

## Features

### Processor Pipeline

Implements a classic 5-stage RISC-V pipeline:

1. **IF** – Instruction Fetch  
2. **ID** – Instruction Decode  
3. **EX** – Execute  
4. **MEM** – Memory Access  
5. **WB** – Write Back  

### Hazard Handling

* **Operand Forwarding:** The simulator forwards ALU results from later pipeline stages to reduce unnecessary stalls.
* **Load-Use Hazard Detection:** Detects cases where forwarding is insufficient and inserts stalls when required.
* **Register Locking:** Tracks register dependencies to maintain correct execution order.

### Supported Instruction Types

**R-Type:**
- `add`
- `sub`
- `mul`
- `div`
- `rem`
- `and`
- `or`
- `xor`
- `sll`
- `srl`
- `sra`
- `slt`
- `sltu`

**I-Type:**
- `addi`
- `andi`
- `ori`
- `xori`
- `slti`
- `sltiu`
- `slli`
- `srli`
- `srai`

**Load/Store:**
- `lw`
- `sw`

**Branch Instructions:**
- `beq`
- `bne`
- `blt`
- `bge`
- `bltu`
- `bgeu`

**Jump Instructions:**
- `jal`
- `jalr`

**Pseudo Instructions:**
- `mv`
- `li`

---

## File Descriptions

### `assembler.cpp`
Converts RISC-V assembly instructions into binary machine code.
**Input**
Reads assembly code from: `inputAssemblyCode.txt`
**Output**
Generates binary instructions in: `outputBinaryCode.txt`

### `datapath.cpp`
Main pipelined processor simulator with:
* Operand forwarding
* Hazard detection
* Memory operations
* Pipeline registers
* ALU control logic
Reads binary instructions from: `outputBinaryCode.txt`

### Sample Input Files
The repository contains sample assembly programs:
* `inputcode_1ifeven`: Assembly code for program that outputs 1 if input is an even number, else 0.
* `inputcode_checkprime`: Assembly code for program that checks if the input is a prime number.
* `inputcode_factorial`: Assembly code for program that computes factorial of given input number.
* `inputcode_sumofNnumbers`: Assembly code for program that computes sum of given first N numbers.

---

## How to Run

1. Put a RISC-V input assembly code in the `inputAssemblyCode.txt` file. (Refer to the above section to know which RISC-V instructions are understood by the processor.)

2. Compile and run the assembler:
   ```bash
   g++ assembler.cpp -o assembler
   ./assembler
   ```
   This would produce a 32-bit binary machine code for each assembly instruction in the `outputBinaryCode.txt` file.

3. Compile and run the processor simulator:
   ```bash
   g++ datapath.cpp -o simulator
   ./simulator
   ```
   This reads input from the `outputBinaryCode.txt` file and executes the machine instructions through the 5-stage CPU simulator and the output of the program is obtained.

---

## Future Improvements

These are the possible extensions that can be made:
* Branch prediction
* Cache simulation
* Superscalar execution
* Out-of-order execution
* Support for more RISC-V instructions

---

Developed as part of Computer Architecture course project for academic and learning purposes.
