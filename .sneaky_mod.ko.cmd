cmd_/home/bitnami/sneaky_mod.ko := ld -r -m elf_x86_64 -T /usr/src/linux-headers-3.13.0-79-generic/scripts/module-common.lds --build-id  -o /home/bitnami/sneaky_mod.ko /home/bitnami/sneaky_mod.o /home/bitnami/sneaky_mod.mod.o