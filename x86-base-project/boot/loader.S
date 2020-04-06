; Definições para utilização no arquivo.
STACK_SIZE          equ 4096          ; Tamanho da pilha em bytes.
MAGIC_NUMBER        equ 0x1BADB002    ; Constante de identificação do multiboot.
FLAGS               equ 0x0           ; Flags do multiboot.
CHECKSUM            equ -MAGIC_NUMBER ; Checksum do multiboot.
                                      ; (magic + checksum + flags = 0).

; Início da seção text (código). A seção é alinhada em quatro bytes.
section .text
align 4

; Escreve o identificador do multiboot, as flags e o checksum no início da seção
; de código para que o multiboot possa identificar o binário corretamente.
dd MAGIC_NUMBER
dd FLAGS
dd CHECKSUM

; Define um símbolo global no binário que será utilizado como ponto de entrada
; identificado no arquivo do linker.
global loader

; Indica que existe uma função externa. A função main é definida em um arquivo
; de código fonte em C.
extern main

; Trecho que será executado, carregado pelo linker. É o ponto de entrada
; principal.
loader:
; Limpa as interrupções.
    cli

; Inicializa a pilha.
    mov esp, main_stack

; Chama a função principal, definida em um arquivo externo.
    call main

; Caso a função principal termine, desliga o sistema.
    hlt

; Início da seção bss (dados), utilizada para armazenamento dos dados das
; variáveis definidas nos arquivos de código C. O início da pilha é definido
; dentro desta seção e ocupa o tamanho definido por STACK_SIZE.
section .bss
align 4
main_stack:

