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

Run script with 'build' option for clean build.
Run script with 'clean' option to clean directories.

Run script with 'run' option to run Bochs.
When GRUB loads, boot with 'kernel 200+blocksize' followed by 'boot' command,
where blocksize is the size of the 'kernel' binary in 512-byte blocks.
