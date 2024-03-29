ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start
    nop

; ; FAT16 Header
; OEMIdentifier           db 'KATARINA'
; BytesPerSector          dw 0x200
; SectorsPerCluster       db 0x80
; ReservedSectors         dw 200
; FATCopies               db 0x02
; RootDirEntries          dw 0x40
; NumSectors              dw 0x00
; MediaType               db 0xF8
; SectorsPerFat           dw 0x100
; SectorsPerTrack         dw 0x20
; NumberOfHeads           dw 0x40
; HiddenSectors           dd 0x00
; SectorsBig              dd 0x773594

; ; Extended BPB (Dos 4.0)
; DriveNumber             db 0x80
; WinNTBit                db 0x00
; Signature               db 0x29
; VolumeID                dd 0xD105
; VolumeIDString          db 'KATARINABOO'
; SystemIDString          db 'FAT16   '
    

start:
    jmp 0:step2
step2:
    cli
    mov ax, 0x0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

.load_protected:
	cli 
	lgdt[gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp CODE_SEG:load32

gdt_start:
gdt_null:
	dd 0x0
	dd 0x0

gdt_code: 			; CS should point to this
	dw 0xffff		; segment limit frist 0 - 15 bits
	dw 0			; Base first 0-15 bits
	db 0			; Base 16-23 bits
	db 0x9a			; Access byte
	db 11001111b	; High 4 bit flags and the low 4 bit flags
	db 0			; Base 24-31 bits

; offset 0x10
gdt_data:			; DS, SS, ES, FS, GS
	dw 0xffff		; segment limit frist 0 - 15 bits
	dw 0			; Base first 0-15 bits
	db 0			; Base 16-23 bits
	db 0x92			; Access byte
	db 11001111b	; High 4 bit flags and the low 4 bit flags
	db 0			; Base 24-31 bits

gdt_end:
gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start

[BITS 32]
load32:
	mov eax, 1			; starting sector
	mov ecx, 100		; number of sectors
	mov edi, 0x0100000	; where to load sectors
	call ata_lba_read

	jmp CODE_SEG:0x0100000 ; 1MB

ata_lba_read:
	mov ebx, eax
	; Send the highest 8 bits of lba to hard disk controller
	shr eax, 24
	or eax, 0xE0	; Selects the master drive
	mov dx, 0x1F6
	out dx, al
	; Finished sending the highest 8 bits of the lba

	; Send the total sectors to read
	mov eax, ecx
	mov dx, 0x1F2
	out dx, al
	; Finished sending the total sectors to read

	; Send more bits of the lba
	mov eax, ebx
	mov dx, 0x1F3
	out dx, al
	; Finished sending more bits od the lba

	mov dx, 0x1F4
	mov eax, ebx		; Restore the backup lba
	shr eax, 8
	out dx, al

	mov dx, 0x1F5
	mov eax, ebx
	shr eax, 16
	out dx, al

	mov dx, 0x1F7
	mov al, 0x20
	out dx, al

.next_sector:
	push ecx

.try_again:
	mov dx, 0x1F7
	in al, dx
	test al, 8
	jz .try_again

	mov ecx, 256
	mov dx, 0x1F0
	rep insw
	pop ecx

	loop .next_sector	; ecx--

	; End of reading sectors into memory
	ret

times 446 - ($ - $$) db 0


; MBR partition table

db 0		; BootFlag
db 0		; CHSBegin
db 0		; CHSBegin
db 0		; CHSBegin
db 0x0B		; TypeCode
db 0		; CHSEnd
db 0		; CHSEnd
db 0		; CHSEnd
db 0xF4		; LBABegin 1f4h = 500d
db 0x01		; LBABegin
db 0x0		; LBABegin
db 0x0		; LBABegin
db 0x00		; NumberOfSectors
db 0xA0		; NumberOfSectors
db 0x0		; NumberOfSectors
db 0x0		; NumberOfSectors

db 0		; BootFlag
db 0		; CHSBegin
db 0		; CHSBegin
db 0		; CHSBegin
db 0x0B		; TypeCode
db 0		; CHSEnd
db 0		; CHSEnd
db 0		; CHSEnd
db 0xF4		; LBABegin 1f4h = 500d
db 0x01		; LBABegin
db 0x0		; LBABegin
db 0x0		; LBABegin
db 0x00		; NumberOfSectors
db 0xA0		; NumberOfSectors
db 0x0		; NumberOfSectors
db 0x0		; NumberOfSectors

db 0		; BootFlag
db 0		; CHSBegin
db 0		; CHSBegin
db 0		; CHSBegin
db 0x0B		; TypeCode
db 0		; CHSEnd
db 0		; CHSEnd
db 0		; CHSEnd
db 0xF4		; LBABegin 1f4h = 500d
db 0x01		; LBABegin
db 0x0		; LBABegin
db 0x0		; LBABegin
db 0x00		; NumberOfSectors
db 0xA0		; NumberOfSectors
db 0x0		; NumberOfSectors
db 0x0		; NumberOfSectors

db 0		; BootFlag
db 0		; CHSBegin
db 0		; CHSBegin
db 0		; CHSBegin
db 0x0B		; TypeCode
db 0		; CHSEnd
db 0		; CHSEnd
db 0		; CHSEnd
db 0xF4		; LBABegin 1f4h = 500d
db 0x01		; LBABegin
db 0x0		; LBABegin
db 0x0		; LBABegin
db 0x00		; NumberOfSectors
db 0xA0		; NumberOfSectors
db 0x0		; NumberOfSectors
db 0x0		; NumberOfSectors

dw 0xAA55






