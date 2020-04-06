#ifndef IO_H
#define IO_H

/*
 * Retorna o valor da porta de entrada e saída especificada pelo endereco do
 * primeiro parametro da funcao.
 */
extern unsigned char inb(unsigned short port);

/*
 * Escreve o valor especificado pelo segundo parametro da funcao no endereco da
 * porta de entrada e saida indicada pelo primeiro parametro.
 */
extern void outb(unsigned short port, unsigned char data);

#endif /* IO_H */

