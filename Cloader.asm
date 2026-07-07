;==========================================
; File: Cloader.asm
; Description: code where the bootloader jump
;
; Author: Titouan(actinide118)
;==========================================

global _start;
[bits 32]
_start:
  [extern start_kernel]
  call start_kernel
handler:
  hlt
  jmp handler
 

