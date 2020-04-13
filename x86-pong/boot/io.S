section .text

global inb
global outb

; args:
;	int 	I/O port number
inbx:
	mov edx, [esp + 4]
	inb al, edx
	ret

; Escreve um valor no barramento de I/O na porta indicada.
; 	int	I/O port number
;	int	value
outbx:
	mov edx, [esp + 4]
	mov al, [esp + 8]
	outb dx, al
	ret

