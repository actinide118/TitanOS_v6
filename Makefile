clean:
	cd bin && rm -rf *
	cd .. && rm -f os-image.bin

CPU_OBJS := $(patsubst ./cpu/%.c, ./bin/%.o, $(wildcard ./cpu/*.c))
APPLIS := $(patsubst ./applis/%.c, ./bin/%.o, $(wildcard ./applis/*.c))
UTILS := $(patsubst ./util/%.c, ./bin/%.o, $(wildcard ./util/*.c))
DRIVERS := $(patsubst ./driver/%.c, ./bin/%.o, $(wildcard ./driver/*.c))

./bin/%.o: ./cpu/%.c
	gcc -ffreestanding -m32 -fno-pie -fno-stack-protector -c $< -o $@

./bin/%.o: ./driver/%.c
	gcc -ffreestanding -m32 -fno-pie -fno-stack-protector -c $< -o $@


./bin/%.o: ./applis/%.c
	gcc -ffreestanding -m32 -fno-pie -fno-stack-protector -c $< -o $@

./bin/evt.o: ./event/evt_queu.c
	gcc -ffreestanding -m32 -fno-pie -fno-stack-protector -c $< -o $@

./bin/%.o: ./util/%.c
	gcc -ffreestanding -m32 -fno-pie -fno-stack-protector -c $< -o $@

./bin/Cloader.o: Cloader.asm
	nasm Cloader.asm -f elf -o bin/Cloader.o

./bin/interrupt.o: ./cpu/interrupt.asm
	nasm cpu/interrupt.asm -f elf -o bin/interrupt.o

./bin/2GRTOS.o: ./2DGRTOS/main.c
	gcc -ffreestanding -m32 -fno-pie -fno-stack-protector -c $< -o $@

./bin/kernel.o: kernel.c
	gcc -ffreestanding -m32 -fno-pie -fno-stack-protector -c $< -o $@

./bin/kernel.bin: ./bin/Cloader.o ./bin/interrupt.o ./bin/evt.o ./bin/2GRTOS.o ./bin/kernel.o $(CPU_OBJS) $(APPLIS) $(UTILS) $(DRIVERS)
	ld -o $@ -Ttext 0x1000 $^ --oformat binary -m elf_i386

./bin/boot.bin:
	nasm -f bin -o ./bin/boot.bin boot.asm

build: ./bin/boot.bin ./bin/kernel.bin
	dd if=/dev/zero of=os-image.bin bs=512 count=2880
	dd if=./bin/boot.bin of=os-image.bin bs=512 count=1 conv=notrunc
	dd if=./bin/kernel.bin of=os-image.bin bs=512 seek=1 conv=notrunc
	qemu-img resize os-image.bin +5M

run: build
	qemu-system-i386 -drive format=raw,file=os-image.bin -no-reboot

all: clean run