[BITS 32]
global _start 

CODE_SEG equ 0x08
DATA_SEG equ 0x10

extern _Z11kernel_mainv
extern handle_irq

global irq_handler_asm

_start:		; protected mode
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov ebp, 0x00200000
	mov esp, ebp

	; Enable the A20 line
	in al, 0x92
	or al, 2
	out 0x92, al

	; Remap the master PIC
	mov al, 00010001b
	out 0x20, al ; Tell master PIC

	mov al, 0x20 ; Interrupt 0x20 is where ISR should start
	out 0x21, al

	mov al, 00000001b
	out 0x21, al

	; End remap of the master PIC
	
	call _Z11kernel_mainv

 	jmp $


irq_handler_asm:
	pushad
	cld    ; C code following the sysV ABI requires DF to be clear on function entry
	
	call handle_irq
	
	popad
	iret

times 512-($ - $$) db 0