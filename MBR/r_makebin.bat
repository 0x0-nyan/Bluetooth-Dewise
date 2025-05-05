nasm -f bin uboot.asm -o uboot.boot
"C:\Program Files\qemu\qemu-system-x86_64.exe" -drive format=raw,file=uboot.boot
pause