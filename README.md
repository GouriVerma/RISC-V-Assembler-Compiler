# RISC-V Assembler and Simulator

This project implements a RISC-V assembler and simulator in C++. It provides functionality to parse RISC-V assembly code, generate machine code, and simulate the execution of RISC-V instructions.

## Features

- Parsing of RISC-V assembly instructions
- Generation of machine code from assembly
- Simulation of RISC-V instruction execution
- Support for various instruction types (R-type, I-type, S-type, B-type, U-type, J-type)
- Implementation of a 5-stage pipeline (Fetch, Decode, Execute, Memory, Writeback)
- Operand forwarding for improved performance

## Key Components

1. **Instruction Parsing**: The `parseLine` function tokenizes assembly instructions.

2. **Instruction Encoding**: The `Instr` class handles the encoding of different instruction types.

3. **Control Unit**: The `ControlWord` class generates control signals based on the opcode.

4. **ALU**: The `ALU` class performs arithmetic and logical operations.

5. **Pipeline Stages**: Implemented as separate functions (fetch, decode, execute, memoryAccess, writeBack).

6. **Pipeline Registers**: Classes like `IFReg`, `IFID`, `IDEX`, `EXMO`, and `MOWB` represent pipeline registers.

7. **Memory**: Simulated instruction memory (`IM`) and data memory (`DM`).

8. **Register File**: General-purpose registers (`GPR`) and a semaphore mechanism for tracking register usage.

## Usage

1. Prepare a RISC-V assembly file.
2. Compile the C++ code.
3. Run the executable, which will read the assembly file and simulate its execution.

## Implementation Details

- The simulator implements a 5-stage pipeline with operand forwarding.
- It handles data hazards and control hazards.
- The ALU supports various operations based on function codes.
- Memory access is simulated for load and store operations.
- The program includes detailed logging of each pipeline stage for debugging and analysis.

## Limitations and Future Improvements

- Currently supports a subset of RISC-V instructions.
- Could be extended to support more complex RISC-V extensions.
- Potential for adding a graphical user interface for better visualization of the pipeline stages.


