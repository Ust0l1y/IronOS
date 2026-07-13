N=nasm
Q=qemu-system-i386
run:
	$(N) src/entry.asm -f bin -o boot.bin
	$(Q) boot.bin

