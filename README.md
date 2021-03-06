toyOS
=====

A toy OS to practice OS fundamentals, implemented in Bochs. Bochs is an x86 emulator.

Difference between emulation and virtualization:

With emulation, the entire architecture is "emulated." With virtualization, only some
trapping instructions in x86 are emulated; the remaining instructions are executed
directly on bare metal.

Difference between emulation and simulation:

With emulation, an object is constructed to appear and "behave" like a target, but the
internal workings may be different from the target. With simulation, an object will be
an exact "replica" of the target.

How to Run
==========

Need Bochs, GCC and Netwide Assembler (NASM)

Run GNU 'make' command.
Run script with 'make' option to build floppy image.

Run script with 'run' option to run Bochs.
When GRUB loads, simply boot the kernel by hitting 'Enter' key. Enjoy!

Run script with 'clean' option to clean working tree.
