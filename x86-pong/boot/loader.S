bits 32

; Definições para utilização no arquivo.
STACK_SIZE          equ 8 * 1024 * 1024         ; Tamanho da pilha em bytes.
MAGIC_NUMBER        equ 0x1BADB002              ; Constante de identificação do multiboot.
FLAGS               equ 0x7                     ; Flags do multiboot.
CHECKSUM            equ -(MAGIC_NUMBER + FLAGS) ; Checksum do multiboot (magic + checksum + flags = 0).

MODE_TYPE           equ 0
WIDTH               equ 1024
HEIGHT              equ 768
DEPTH               equ 32

HEADER_ADDR         equ 0
LOAD_ADDR           equ 0
LOAD_END_ADDR       equ 0
BSS_END_ADDR        equ 0
ENTRY_ADDR          equ 0

; Escreve o identificador do multiboot, as flags e o checksum no início da seção
; de código para que o multiboot possa identificar o binário corretamente.
section .multiboot
dd MAGIC_NUMBER
dd FLAGS
dd CHECKSUM
dd HEADER_ADDR
dd LOAD_ADDR
dd LOAD_END_ADDR
dd BSS_END_ADDR
dd ENTRY_ADDR
dd MODE_TYPE
dd WIDTH
dd HEIGHT
dd DEPTH

resb 5*13

; Início da seção text (código). A seção é alinhada em quatro bytes.
section .text
align 4

; Define um símbolo global no binário que será utilizado como ponto de entrada
; identificado no arquivo do linker.
global start

; Indica que existe uma função externa. A função main é definida em um arquivo
; de código fonte em C. IDT para carregar os descritores das interrupções.
extern main
extern idt_init

; Trecho que será executado, carregado pelo linker. É o ponto de entrada
; principal.
start:
    lgdt [gdtr]                 ; Load our own GDT, the GDTR of Grub may be invalid

    jmp CODE32_SEL:.setcs       ; Set CS to our 32-bit flat code selector
.setcs:
    mov ax, DATA32_SEL          ; Setup the segment registers with our flat data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

; Inicializa a pilha.
    mov esp, main_stack        ; set stack pointer

; Inicializa os descritores de interrupção
    call idt_init

; Chama a função principal, definida em um arquivo externo.
    call main

; If we get here just enter an infinite loop
endloop:
    hlt                         ; halt the CPU
    jmp endloop

; Macro to build a GDT descriptor entry
%define MAKE_GDT_DESC(base, limit, access, flags) \
    (((base & 0x00FFFFFF) << 16) | \
    ((base & 0xFF000000) << 32) | \
    (limit & 0x0000FFFF) | \
    ((limit & 0x000F0000) << 32) | \
    ((access & 0xFF) << 40) | \
    ((flags & 0x0F) << 52))

section .data
align 4
gdt_start:
    dq MAKE_GDT_DESC(0, 0, 0, 0); null descriptor
gdt32_code:
    dq MAKE_GDT_DESC(0, 0x00ffffff, 10011010b, 1100b)
                                ; 32-bit code, 4kb gran, limit 0xffffffff bytes, base=0
gdt32_data:
    dq MAKE_GDT_DESC(0, 0x00ffffff, 10010010b, 1100b)
                                ; 32-bit data, 4kb gran, limit 0xffffffff bytes, base=0
end_of_gdt:

gdtr:
    dw end_of_gdt - gdt_start - 1
                                ; limit (Size of GDT - 1)
    dd gdt_start                ; base of GDT

CODE32_SEL equ gdt32_code - gdt_start
DATA32_SEL equ gdt32_data - gdt_start

; Início da seção bss (dados), utilizada para armazenamento dos dados das
; variáveis definidas nos arquivos de código C. O início da pilha é definido
; dentro desta seção e ocupa o tamanho definido por STACK_SIZE.
section .bss
resb STACK_SIZE
main_stack:

