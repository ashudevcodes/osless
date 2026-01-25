#!/bin/bash

set -xe

# qemu-system-x86_64 -bios ./OVMF.4m.fd \
#   -drive file=fat:rw:EFI,format=raw \
#   -nographic \
#   -net none \
#   -serial mon:stdio

gcc main.c                           \
  -c                                 \
  -fno-stack-protector               \
  -fpic                              \
  -fshort-wchar                      \
  -mno-red-zone                      \
  -I /usr/include/efi/x86_64/ 		 \
  -I /usr/include/efi/x86_64/ 		 \
  -DEFI_FUNCTION_WRAPPER             \
  -o main.o

ld main.o                         	\
  /usr/lib/crt0-efi-x86_64.o 		\
  -nostdlib                      	\
  -znocombreloc                  	\
  -T /usr/lib/elf_x86_64_efi.lds 	\
  -shared                        	\
  -Bsymbolic                     	\
  -L /usr/lib/		              	\
  -l:libgnuefi.a                 	\
  -l:libefi.a                    	\
  -o main.so

objcopy -j .text          \
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
  main.efi

mkdir -p builds
mv main.efi main.so main.o ./builds
mkdir -p esp
cp ./builds/main.efi ./esp

qemu-system-x86_64 -cpu qemu64 \
  -drive if=pflash,format=raw,unit=0,file=./bios/OVMF_CODE.fd,readonly=on \
  -drive if=pflash,format=raw,unit=1,file=./bios/OVMF_VARS.fd \
  -drive format=raw,file=fat:rw:esp \
  -net none \
  -nographic
