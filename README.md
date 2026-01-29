# complicated but easy UEFI Application

which can run on your laptop or PC motherboard BIOS
before loading an OS

## to build .efi
- `make`
This will build an EFI file
- `make run`
This will build .efi and run on QEMU VM.
Yes the prerequisite was QEMU and some BIOS files
which are stored in `/usr/share/edk2/x64/` if you are on Unix
and your system has UEFI by default.

## Prerequisites
- `qemu`
- `OVMF_VARS.fd` `OVMF_CODE.fd` BIOS files. Make a bios dir in the root of the
project folder and put those files there.
