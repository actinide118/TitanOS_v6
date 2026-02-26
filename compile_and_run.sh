#/bin/sh
rm boot.bin
rm kernel.bin
rm kernel.elf
rm os-image.bin
rm -R bin
mkdir bin
nasm -f bin -o boot.bin boot.asm
make_boot() {
    dd if=/dev/zero of=os-image.bin bs=512 count=2880
    dd if=boot.bin of=os-image.bin bs=512 count=1 conv=notrunc
    dd if=kernel.bin of=os-image.bin bs=512 seek=1 conv=notrunc
}
run() {
    if test -f boot.bin && test -f kernel.bin; then
        make_boot
        #qemu-system-x86_64 -drive format=raw,file=os-image.bin #-no-reboot #-d int,cpu_reset -no-reboot -D debug.log
        qemu-img resize os-image.bin +5M
        qemu-system-i386 -drive format=raw,file=os-image.bin -no-reboot
    else
        echo "error during compiling"
        exit 1
    fi
}

case $1 in
"asm")
    nasm -f bin -o kernel.bin kernel.asm
    run
    ;;
"c")
    nasm Cloader.asm -f elf -o bin/Cloader.o
    nasm cpu/interrupt.asm -f elf -o bin/interrupt.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/vga.c -o bin/vga.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/ports.c -o bin/ports.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c util/util.c -o bin/util.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c cpu/idt.c -o bin/idt.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c cpu/isr.c -o bin/isr.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c kernel.c -o bin/kernel.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c cpu/timer.c -o bin/timer.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/keyboard.c -o bin/keyboard.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/ata.c -o bin/ata.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/keyboard2.c -o bin/keyboard2.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c event/evt_queu.c -o bin/evt_queu.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c applis/move.c -o bin/move.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c util/alea.c -o bin/alea.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c 2DGRTOS/main.c -o bin/2DGRTOS.o
    x86_64-elf-ld -m elf_i386 -o kernel.bin -Ttext 0x1000 bin/Cloader.o bin/vga.o bin/ports.o bin/util.o bin/idt.o bin/isr.o bin/interrupt.o bin/kernel.o bin/timer.o bin/keyboard2.o bin/ata.o bin/evt_queu.o bin/move.o bin/alea.o bin/2DGRTOS.o --oformat binary
    run
    ;;
"cdebug")
    nasm Cloader.asm -f elf -o bin/Cloader.o
    nasm cpu/interrupt.asm -f elf -o bin/interrupt.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/vga.c -o bin/vga.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/ports.c -o bin/ports.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c util/util.c -o bin/util.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c cpu/idt.c -o bin/idt.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c cpu/isr.c -o bin/isr.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c kernel.c -o bin/kernel.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c cpu/timer.c -o bin/timer.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/keyboard.c -o bin/keyboard.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/ata.c -o bin/ata.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/keyboard2.c -o bin/keyboard2.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c event/evt_queu.c -o bin/evt_queu.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c applis/move.c -o bin/move.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c util/alea.c -o bin/alea.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c 2DGRTOS/main.c -o bin/2DGRTOS.o
    x86_64-elf-ld -m elf_i386 -o kernel.bin -Ttext 0x1000 bin/Cloader.o bin/vga.o bin/ports.o bin/util.o bin/idt.o bin/isr.o bin/interrupt.o bin/kernel.o bin/timer.o bin/keyboard2.o bin/ata.o bin/evt_queu.o bin/move.o bin/alea.o bin/2DGRTOS.o --oformat binary
    x86_64-elf-ld -m elf_i386 -o kernel.elf -Ttext 0x1000 bin/Cloader.o bin/vga.o bin/ports.o bin/util.o bin/idt.o bin/isr.o bin/interrupt.o bin/kernel.o bin/timer.o bin/keyboard2.o bin/ata.o bin/evt_queu.o bin/move.o bin/alea.o bin/2DGRTOS.o
    if test -f boot.bin && test -f kernel.elf; then
        make_boot
    else
        echo "error during compiling"
        exit 1
    fi
    qemu-system-i386 -s -S -fda os-image.bin -d guest_errors,int &
    i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
    ;;
"disasm")
    nasm Cloader.asm -f elf -o bin/Cloader.o
    nasm cpu/interrupt.asm -f elf -o bin/interrupt.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/vga.c -o bin/vga.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/ports.c -o bin/ports.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c util/util.c -o bin/util.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c cpu/idt.c -o bin/idt.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c cpu/isr.c -o bin/isr.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c kernel.c -o bin/kernel.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c cpu/timer.c -o bin/timer.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/keyboard.c -o bin/keyboard.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/ata.c -o bin/ata.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c driver/keyboard2.c -o bin/keyboard2.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c event/evt_queu.c -o bin/evt_queu.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c applis/move.c -o bin/move.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c util/alea.c -o bin/alea.o
    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c 2DGRTOS/main.c -o bin/2DGRTOS.o
    x86_64-elf-ld -m elf_i386 -o kernel.bin -Ttext 0x1000 bin/Cloader.o bin/vga.o bin/ports.o bin/util.o bin/idt.o bin/isr.o bin/interrupt.o bin/kernel.o bin/timer.o bin/keyboard2.o bin/ata.o bin/evt_queu.o bin/move.o bin/alea.o bin/2DGRTOS.o --oformat binary
    ndisasm -b 32 kernel.bin >$2
    ;;
"clean") exit 0 ;;
*)
    echo "commande non supporté"
    exit 1
    ;;
esac
#if test ${1} = "asm"; then
#    nasm -f bin -o kernel.bin kernel.asm
#else
#    nasm Cloader.asm -f elf -o Cloader.o
#    echo "gcc"
#    x86_64-elf-gcc -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c kernel.c -o kernel.o
#    echo "ld"
#    x86_64-elf-ld -m elf_i386 -o kernel.bin -Ttext 0x1000 Cloader.o kernel.o --oformat binary
#fi
