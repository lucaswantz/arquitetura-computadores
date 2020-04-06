#include <sys/io.h>

// Video pointer
static unsigned short * const vidptr = (unsigned short *) 0xB8000;

// Não funcionará quando tem mais que um caracter
char *keymap_normal[] = {
    ' ',' ','1','2','3','4','5','6','7','8','9','0','-','=',' ',
    ' ','q','w','e','r','t','y','u','i','o','p','`','[','\n',' ',
    'a','s','d','f','g','h','j','k','l','ç','^','\'','@',']',
    'z','x','c', 'v', 'b', 'n', 'm',',','.',';',' ','*',' ',' ',' ', 
    '<F1>','<F2>','<F3>','<F4>','<F5>','<F6>','<F7>','<F8>','<F9>','<F10>',
    ' ',' ','7','8','9','-','4','5','6','+','1','2','3','0',',',
    ' ',' ','\\','<F11>','<F12>',' ',' ',' ',' ',' ',' ',' ',
    '\n','<CTRL>','/'};

unsigned char rtc_seq(void){
    unsigned char sec;

    // segundos
    outb(0x70, 0x00);
    sec = inb(0x71);
    return sec;
}

unsigned char rtc_min(void){
    unsigned char min;

    // minutos 
    outb(0x70, 0x02);
    min = inb(0x71);
    return min;   
}

unsigned char rtc_hour(void){
    unsigned char hora;

    // hora
    outb(0x70, 0x04);
    hora = inb(0x71);
    return hora;
}

unsigned char rtc_year(void){
    unsigned char ano;

    // ano
    outb(0x70, 0x09);
    ano = inb(0x71);
    return ano;
}

void write_char(char c, int x, int y, unsigned char fg, unsigned char bg){
    if (x >= 80)
        x = 79;

    if (y >= 25)
        y = 24;
    
    vidptr[y * 80 + x] = (bg << 12) | (fg << 8) | c;
}

int keyboard_read(void){
    int key = -1;
    unsigned char status;

    status = inb(0x64);

    if (status & (1 << 0))
       key = inb(0x60);

    return key;
}

int main(void)
{
    int x = 0;
    int segundo;
    int minuto;
    int hora;
    int ano;

    write_char('A', 40, 12, 10, 14);
    write_char('u', 41, 12, 10, 14);
    write_char('l', 42, 12, 10, 14);
    write_char('a', 43, 12, 10, 14);

    while (1) {
        int key = keyboard_read();
        if (key != -1){
            write_char(keymap_normal[key], x++, 0, 10, 1);
        }

        segundo = rtc_seq();
        minuto = rtc_min();
        hora = rtc_hour();
        ano = rtc_year();

        if(hora < 3){
            hora = hora + 0x21;
        } else {
            hora = hora - 0x03;
        }

        unsigned char dezena;
        unsigned char unidade;

        // Hora
        dezena = ((hora & 0xF0) >> 4) + 0x30;
        unidade = (hora & 0x0F) + 0x30;
        write_char(dezena, 17, 5, 0, 15);
        write_char(unidade, 18, 5, 0, 15);

        write_char(':', 19, 5, 0, 15);

        // Minuto
        dezena = ((minuto & 0xF0) >> 4) + 0x30;
        unidade = (minuto & 0x0F) + 0x30;
        write_char(dezena, 20, 5, 0, 15);
        write_char(unidade, 21, 5, 0, 15);

        write_char(':', 22, 5, 0, 15);

        // Segundo
        dezena = ((segundo & 0xF0) >> 4) + 0x30;
        unidade = (segundo & 0x0F) + 0x30;
        write_char(dezena, 23, 5, 0, 15);
        write_char(unidade, 24, 5, 0, 15);

        // Ano
        dezena = ((ano & 0xF0) >> 4) + 0x30;
        unidade = (ano & 0x0F) + 0x30;
        write_char('2', 26, 5, 0, 15);
        write_char('0', 27, 5, 0, 15);
        write_char(dezena, 28, 5, 0, 15);
        write_char(unidade, 29, 5, 0, 15);
    }

    return 0;
}
