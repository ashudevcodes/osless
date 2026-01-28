TARGET = main.efi

CC		= gcc
LD		= ld
OBJCOPY	= objcopy


EFI_INC		= /usr/include/efi 
EFI_INC_X86	= /usr/include/efi/x86_64 
EFI_LIB		= /usr/lib 
EFI_LDS		= /usr/lib/elf_x86_64_efi.lds
EFI_CRT0	= /usr/lib/crt0-efi-x86_64.o

CFLAGS = -fno-stack-protector -fpic -fshort-wchar -mno-red-zone \
		 -I$(EFI_INC) -I$(EFI_INC_X86) -DEFI_FUNCTION_WRAPPER -c

LDFLAGS = -nostdlib -znocombreloc -T $(EFI_LDS) -shared \
		  -Bsymbolic -L $(EFI_LIB) -l:libgnuefi.a -l:libefi.a

all: $(TARGET)

main.o: main.c 
	$(CC) $(CFLAGS) main.c -o main.o

main.so: main.o 
	$(LD) $(EFI_CRT0) main.o $(LDFLAGS) -o main.so

$(TARGET): main.so 
	$(OBJCOPY) -j .text          \
		-j .sdata               \
		-j .data                \
		-j .rodata              \
		-j .dynamic             \
		-j .dynsym              \
		-j .rel                 \
		-j .rela                \
		-j .reloc               \
		--target=efi-app-x86_64 \
		main.so                 \
		$(TARGET)

setup: $(TARGET)
	mkdir -p esp/EFI/BOOT
	cp $(TARGET) esp/EFI/BOOT/BOOTX64.EFI

run: setup
	qemu-system-x86_64 -cpu qemu64 \
		-drive if=pflash,format=raw,unit=0,file=./bios/OVMF_CODE.fd,readonly=on \
		-drive if=pflash,format=raw,unit=1,file=./bios/OVMF_VARS.fd \
		-drive format=raw,file=fat:rw:esp \
		-net none \
		-nographic \
		-serial mon:stdio

clean: 
	rm -rf *.o *.so *.efi esp
