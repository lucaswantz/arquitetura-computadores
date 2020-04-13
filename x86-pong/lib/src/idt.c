#include <sys/io.h>

#define IDT_SIZE        256
#define PIC_1_CTRL      0x20
#define PIC_2_CTRL      0xA0
#define PIC_1_DATA      0x21
#define PIC_2_DATA      0xA1

extern int load_idt();
extern int irq0();
extern int irq1();
extern int irq2();
extern int irq3();
extern int irq4();
extern int irq5();
extern int irq6();
extern int irq7();
extern int irq8();
extern int irq9();
extern int irq10();
extern int irq11();
extern int irq12();
extern int irq13();
extern int irq14();
extern int irq15();

struct idt_entry {
    unsigned short offset_lowerbits;
    unsigned short selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short offset_higherbits;
} __attribute__((packed));

static struct idt_entry idt_table[256];

void idt_init(void)
{
    unsigned long irq0_address;
    unsigned long irq1_address;
    unsigned long irq2_address;
    unsigned long irq3_address;          
    unsigned long irq4_address; 
    unsigned long irq5_address;
    unsigned long irq6_address;
    unsigned long irq7_address;
    unsigned long irq8_address;
    unsigned long irq9_address;          
    unsigned long irq10_address;
    unsigned long irq11_address;
    unsigned long irq12_address;
    unsigned long irq13_address;
    unsigned long irq14_address;          
    unsigned long irq15_address;         
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    /* remapping the PIC */
    /* ICW1 - begin initialization */
    outb(PIC_1_CTRL, 0x11);
    outb(PIC_2_CTRL, 0x11);

    /* ICW2 - remap offset address of idt_table */
    /*
    * In x86 protected mode, we have to remap the PICs beyond 0x20 because
    * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
    */
    outb(PIC_1_DATA, 0x20);
    outb(PIC_2_DATA, 0x28);

    outb(PIC_1_DATA, 0x04);
    outb(PIC_2_DATA, 0x02);

    /* ICW4 - environment info */
    outb(PIC_1_DATA, 0x01);
    outb(PIC_2_DATA, 0x01);

    /* Mask interrupts */
    outb(PIC_1_DATA, 0x00);
    outb(PIC_2_DATA, 0x00);

    irq0_address = (unsigned long)irq0; 
    idt_table[32].offset_lowerbits = irq0_address & 0xffff;
    idt_table[32].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[32].zero = 0;
    idt_table[32].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[32].offset_higherbits = (irq0_address & 0xffff0000) >> 16;

    irq1_address = (unsigned long)irq1; 
    idt_table[33].offset_lowerbits = irq1_address & 0xffff;
    idt_table[33].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[33].zero = 0;
    idt_table[33].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[33].offset_higherbits = (irq1_address & 0xffff0000) >> 16;

    irq2_address = (unsigned long)irq2; 
    idt_table[34].offset_lowerbits = irq2_address & 0xffff;
    idt_table[34].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[34].zero = 0;
    idt_table[34].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[34].offset_higherbits = (irq2_address & 0xffff0000) >> 16;

    irq3_address = (unsigned long)irq3; 
    idt_table[35].offset_lowerbits = irq3_address & 0xffff;
    idt_table[35].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[35].zero = 0;
    idt_table[35].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[35].offset_higherbits = (irq3_address & 0xffff0000) >> 16;

    irq4_address = (unsigned long)irq4; 
    idt_table[36].offset_lowerbits = irq4_address & 0xffff;
    idt_table[36].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[36].zero = 0;
    idt_table[36].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[36].offset_higherbits = (irq4_address & 0xffff0000) >> 16;

    irq5_address = (unsigned long)irq5; 
    idt_table[37].offset_lowerbits = irq5_address & 0xffff;
    idt_table[37].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[37].zero = 0;
    idt_table[37].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[37].offset_higherbits = (irq5_address & 0xffff0000) >> 16;

    irq6_address = (unsigned long)irq6; 
    idt_table[38].offset_lowerbits = irq6_address & 0xffff;
    idt_table[38].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[38].zero = 0;
    idt_table[38].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[38].offset_higherbits = (irq6_address & 0xffff0000) >> 16;

    irq7_address = (unsigned long)irq7; 
    idt_table[39].offset_lowerbits = irq7_address & 0xffff;
    idt_table[39].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[39].zero = 0;
    idt_table[39].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[39].offset_higherbits = (irq7_address & 0xffff0000) >> 16;

    irq8_address = (unsigned long)irq8; 
    idt_table[40].offset_lowerbits = irq8_address & 0xffff;
    idt_table[40].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[40].zero = 0;
    idt_table[40].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[40].offset_higherbits = (irq8_address & 0xffff0000) >> 16;

    irq9_address = (unsigned long)irq9; 
    idt_table[41].offset_lowerbits = irq9_address & 0xffff;
    idt_table[41].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[41].zero = 0;
    idt_table[41].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[41].offset_higherbits = (irq9_address & 0xffff0000) >> 16;

    irq10_address = (unsigned long)irq10; 
    idt_table[42].offset_lowerbits = irq10_address & 0xffff;
    idt_table[42].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[42].zero = 0;
    idt_table[42].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[42].offset_higherbits = (irq10_address & 0xffff0000) >> 16;

    irq11_address = (unsigned long)irq11; 
    idt_table[43].offset_lowerbits = irq11_address & 0xffff;
    idt_table[43].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[43].zero = 0;
    idt_table[43].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[43].offset_higherbits = (irq11_address & 0xffff0000) >> 16;

    irq12_address = (unsigned long)irq12; 
    idt_table[44].offset_lowerbits = irq12_address & 0xffff;
    idt_table[44].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[44].zero = 0;
    idt_table[44].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[44].offset_higherbits = (irq12_address & 0xffff0000) >> 16;

    irq13_address = (unsigned long)irq13; 
    idt_table[45].offset_lowerbits = irq13_address & 0xffff;
    idt_table[45].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[45].zero = 0;
    idt_table[45].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[45].offset_higherbits = (irq13_address & 0xffff0000) >> 16;

    irq14_address = (unsigned long)irq14; 
    idt_table[46].offset_lowerbits = irq14_address & 0xffff;
    idt_table[46].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[46].zero = 0;
    idt_table[46].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[46].offset_higherbits = (irq14_address & 0xffff0000) >> 16;

    irq15_address = (unsigned long)irq15; 
    idt_table[47].offset_lowerbits = irq15_address & 0xffff;
    idt_table[47].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    idt_table[47].zero = 0;
    idt_table[47].type_attr = 0x8e; /* INTERRUPT_GATE */
    idt_table[47].offset_higherbits = (irq15_address & 0xffff0000) >> 16;

    /* fill the idt_table descriptor */
    idt_address = (unsigned long)idt_table ;
    idt_ptr[0] = (sizeof (struct idt_entry) * 256) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16 ;

    load_idt(idt_ptr);
}

