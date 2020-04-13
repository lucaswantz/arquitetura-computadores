ENTRY(start)                /* Label que será executado primeiro. */

SECTIONS {
    . = 0x00100000;          /* Código deve ser carregado no endereço 1 MB. */

    .multiboot ALIGN (0x1000) :   /* Seção text alinhada em 4 kB. */
    {
	*(.multiboot)
    }

    .text ALIGN (0x1000) :   /* Seção text alinhada em 4 kB. */
    {
        *(.text)             /* Todas as seções de texto de todos os arquivos. */
    }

    .rodata ALIGN (0x1000) : /* Seção somente leitura alinhada em 4 kB. */
    {
        *(.rodata*)          /* Todas as seções somente leitura de todos os arquivos. */
    }

    .data ALIGN (0x1000) :   /* Seção data alinhada em 4 kB. */
    {
        *(.data)             /* Todas as seções data de todos os arquivos. */
    }

    .bss ALIGN (0x1000) :    /* Seção bss allinhada em 4 kB. */
    {
        *(COMMON)            /* Todas as seções COMMOM. */
        *(.bss)              /* Todas as seções bss. */
    }
}

