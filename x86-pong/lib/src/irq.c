#include <sys/io.h>

static void __default_isr(void) { }

void isr0(void) __attribute__ ((weak, alias("__default_isr")));
void isr1(void) __attribute__ ((weak, alias("__default_isr")));
void isr2(void) __attribute__ ((weak, alias("__default_isr")));
void isr3(void) __attribute__ ((weak, alias("__default_isr")));
void isr4(void) __attribute__ ((weak, alias("__default_isr")));
void isr5(void) __attribute__ ((weak, alias("__default_isr")));
void isr6(void) __attribute__ ((weak, alias("__default_isr")));
void isr7(void) __attribute__ ((weak, alias("__default_isr")));
void isr8(void) __attribute__ ((weak, alias("__default_isr")));
void isr9(void) __attribute__ ((weak, alias("__default_isr")));
void isr10(void) __attribute__ ((weak, alias("__default_isr")));
void isr11(void) __attribute__ ((weak, alias("__default_isr")));
void isr12(void) __attribute__ ((weak, alias("__default_isr")));
void isr13(void) __attribute__ ((weak, alias("__default_isr")));
void isr14(void) __attribute__ ((weak, alias("__default_isr")));
void isr15(void) __attribute__ ((weak, alias("__default_isr")));

void irq0_handler(void)
{
    isr0();
    outb(0x20, 0x20); //EOI
}
 
void irq1_handler(void)
{
    isr1();
    outb(0x20, 0x20); //EOI
}
 
void irq2_handler(void)
{
    isr2();
    outb(0x20, 0x20); //EOI
}
 
void irq3_handler(void)
{
    isr3();
    outb(0x20, 0x20); //EOI
}
 
void irq4_handler(void)
{
    isr4();
    outb(0x20, 0x20); //EOI
}
 
void irq5_handler(void)
{
    isr5();
    outb(0x20, 0x20); //EOI
}
 
void irq6_handler(void)
{
    isr6();
    outb(0x20, 0x20); //EOI
}
 
void irq7_handler(void)
{
    isr7();
    outb(0x20, 0x20); //EOI
}
 
void irq8_handler(void)
{
    isr8();
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI          
}
 
void irq9_handler(void)
{
    isr9();
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}
 
void irq10_handler(void)
{
    isr10();
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}
 
void irq11_handler(void)
{
    isr11();
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}
 
void irq12_handler(void)
{
    isr12();
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}
 
void irq13_handler(void)
{
    isr13();
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}
 
void irq14_handler(void)
{
    isr14();
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}
 
void irq15_handler(void)
{
    isr15();
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

