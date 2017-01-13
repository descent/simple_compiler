# test for the code generator
#!/bin/sh
cat h.s func.s op.s p.s  > r.s
as --32 -gstabs -o r.o r.s
ld -m elf_i386 -dynamic-linker /lib/ld-linux.so.2 -o r -lc r.o
