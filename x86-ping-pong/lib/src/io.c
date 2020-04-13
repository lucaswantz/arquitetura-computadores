#include <sys/io.h>

unsigned char inb(unsigned short port)
{
    unsigned char ret;

    asm("inb %1, %0" : "=a" (ret) : "dN" (port));

    return ret;
}

void outb(unsigned short port, unsigned char data)
{
    asm("outb %1, %0" : : "dN" (port), "a" (data));
}

