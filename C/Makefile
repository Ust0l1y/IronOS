CC=gcc
AS=nasm
LD=gcc
CFLAGS=-m32 -ffreestanding -O2 -flto -nostdlib -fno-stack-protector -fno-pie -fno-pic -Isrc/kernel/inc
LDFLAGS=-m32 -Wl,-T,linker.ld -nostdlib -fuse-ld=bfd
QEMU=qemu-system-i386
ISO=os.iso
BIN=bin

SRC_C := $(shell find src -name "*.c")
OBJ_C := $(patsubst %.c, $(BIN)/%.o, $(SRC_C))

all: os.bin
	mkdir -p isofiles/boot/grub
	cp os.bin isofiles/boot/
	cp grub.cfg isofiles/boot/grub/
	grub-mkrescue -o $(ISO) isofiles

run: all
	$(QEMU) -cdrom $(ISO)

os.bin: $(BIN)/boot.o $(BIN)/lgdt.o $(BIN)/idts.o $(OBJ_C)
	$(LD) $(LDFLAGS) $^ -o $@

$(BIN)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)/boot.o: src/boot/boot.asm
	@mkdir -p $(dir $@)
	$(AS) -f elf32 $< -o $@

$(BIN)/lgdt.o: src/boot/lgdt.asm
	@mkdir -p $(dir $@)
	$(AS) -f elf32 $< -o $@

$(BIN)/idts.o: src/boot/idt.asm
	@mkdir -p $(dir $@)
	$(AS) -f elf32 $< -o $@

clean:
	rm -rf $(BIN) isofiles os.bin $(ISO)