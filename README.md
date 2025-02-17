# riscv-ndn-extension
Small set of custom instructions for accelerating SHA256 and SipHash on RISC-V. Created for completion of the Computer Architecture course (EECE7352) in NEU.

## File organization

**/sha256** This folder contains pure C and inline assembly modified benchmarks for SHA256, used in gathering results for the project report.

**/siphash** This folder contains pure C and inline assembly modified benchmarks for SipHash, used in gathering results for the project report.

**/src** This folder contains modified files that are originally from the riscv-gnu-toolchain and gem5 source code. The files riscv-opc.h and riscv-opc.c are from the toolchain and they enable the cross compiler to omit the custom instructions into binaries, provided we use inline assembly in the C code. The MinorCPU.py file is the Python configuration interface to the gem5 MinorCPU model and this modified version includes a custom functional unit. For the custom timings in this file to take effect, we have to fix a small issue in the /gem5/src/cpu/minor/func_unit.cc file, where the line #if THE_ISA == ARM_ISA is present, we need to modify it as #if (THE_ISA == ARM_ISA) || (THE_ISA == RISCV_ISA) so that the condition for the function that finds custom timings includes the RISC-V ISA as well. The registers.hh file is from the RISC-V ISA model in gem5 and defines registers; the modified file includes the internal registers needed for the custom functional units. Unfortunately this was done by modifying a core model file in this way, because the gem5 documentation was not clear if the Python interface could be utilized to implement these internal registers. The run_massbench.sh bash script file is there as an example of a script that helps run a series of benchmarking tasks. It is not intended to be run directly; make sure that you manage all the paths according to your host machine.
