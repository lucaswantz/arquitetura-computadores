#include <sys/io.h>
#include <gfx/video.h>

// Define as cores do projeto
static char color_black = 0x000000; // #000000
static char color_white = 0xFFFFFF; // #FFFFFF
static char color_debug = 0x00AA00; // #00AA00

// Define o tamanho do tabuleiro (ex 1024 X 768).
static int tamanho_tabuleiro_x = 1024;
static int tamanho_tabuleiro_y = 768;

// Controles
static int altura_barra = 200;
static int velocidade_controle = 20;


static int y1 = 234;
static int y2 = 234;
static int dy1 = 0;
static int dy2 = 0;

// Bola
static int velocidade_bola = 5;
static int x_bola = 512;
static int y_bola = 312;
static int dy_bola = 0;
static int dx_bola = 0;


int i = 0;

/*
Risquinhos do p1 começam no 482 e vão DECREMENTANDO de 10 em 10
Risquinhos do p2 começam em 540 e vão INCOREMENTANDO de 10 em 10 
*/
static int score_p1 = 482;
static int score_p2 = 540;

int COM = 0x3f8; /* COM1 */



void desenha_pontuacao_p1() {
    if (score_p1 > 100) {
        draw_square(score_p1, 15, 5, 25, color_black);
        score_p1 -= 10;
    }
}

void desenha_pontuacao_p2() {
    if (score_p2 < 924) {
        draw_square(score_p2, 15, 5, 25, color_black);
        score_p2 += 10;
    }
}

void desenha_player_1() {
    draw_square(60, y1, 20, altura_barra, color_white);
    y1 += dy1;

    draw_square(60, y1, 20, altura_barra, color_black);

    if (y1 < 0) {
        y1 = 0;
    }
    
    if (y1 > 568) {
        y1 = 568;
    }
}

void desenha_player_2() {
    draw_square(tamanho_tabuleiro_x - 80, y2, 20, altura_barra, color_white);
    y2 += dy2;

    draw_square(tamanho_tabuleiro_x - 80, y2, 20, altura_barra, color_black);
    if (y2 < 0) {
        y2 = 0;
    }

    if (y2 > 568) {
        y2 = 568;
    }
}

void desenha_bola() {
    draw_square(x_bola, y_bola, 30, 30, color_white); 

    /* Se a bola saiu do esquadro, ela é projetada no meio novamente */
    //   if(x_bola >= 974 || y_bola <= 50){
    //     x_bola = 512;    y_bola = 312;    dx_bola = 0;    dy_bola = 0;
    //   } 
    
    if(y_bola<0){
        y_bola = y_bola*(-1);
    }else{
        y_bola = y_bola;
    }

	if(x_bola<0) {
        x_bola = x_bola*(-1);
    } else {
        x_bola = x_bola;
    } 

    if(i == 1) {
        x_bola -= velocidade_bola;
        y_bola -= velocidade_bola;  
    } else if (i == 0) {
        x_bola += velocidade_bola;
        y_bola += velocidade_bola;
    }

    if(y_bola >= 600){
        i = 1;
    }

    draw_square(x_bola, y_bola, 30, 30, color_white);
}

int desenha_pontilhado() {
    for (int i = 1; i < tamanho_tabuleiro_y; i += 8) {
        draw_square(tamanho_tabuleiro_x / 2, i, 2, 2, color_black);
    }
}

// Vai ser executada a cada 18x por segundo, aproximadamente
void isr0(void) {
    desenha_player_1();
    desenha_player_2();

    desenha_pontilhado();

    desenha_pontuacao_p1();
    desenha_pontuacao_p2();

    desenha_bola();
    
    dx_bola += 1;
    dy_bola += 1;
}

// INTERRUPÇÃO QUE CUIDA DAS TECLAS PRESSIONADAS
void isr1(void) {
    char keycode;
    keycode = inb(0x60);

    // Se uma tecla foi pressionada
    if (keycode & 0x80) { 
        // Devolve o bit mais   
        keycode &= ~0x80; 

        // TECLA W
        if (keycode == 17) {
            dy1 = 0;
        }

        // TECLA S
        if (keycode == 31) {
            dy1 = 0;
        }

        // TECLA UP
        if (keycode == 72) {
            dy2 = 0;
        }

        // TECLA DOWN
        if (keycode == 80) {
            dy2 = 0;
        }
    } else {
        // TECLA W
        if (keycode == 17) {
            dy1 = -velocidade_controle;
        }

        // TECLA S
        if (keycode == 31) {
            dy1 = velocidade_controle;
        }

        // TECLA DOWN
        if (keycode == 80) {
            dy2 = velocidade_controle;
        }

        // TECLA UP
        if (keycode == 72) {
            dy2 = -velocidade_controle;
        }
    }
}

void usart_init(int base_addr) {
   outb(base_addr + 1, 0x00);    // Disable all interrupts
   outb(base_addr + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(base_addr + 0, 0x01);    // Set divisor to 1 (lo byte) 115000 baud
   outb(base_addr + 1, 0x00);    //                  (hi byte)
   outb(base_addr + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(base_addr + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(base_addr + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int serial_received(int PORT) {
   return inb(PORT + 5) & 1;
}

char read_serial(int PORT) {
   //while (serial_received() == 0);

   if (serial_received(PORT) == 0) {
      return 0;
   }

   return inb(PORT);
}

int is_transmit_empty(int PORT) {
   return inb(PORT + 5) & 0x20;
}

void usart_write(int PORT, unsigned char c) {
   while (is_transmit_empty(PORT) == 0);

   outb(PORT,c);
}

void usart_puts(char *str){
 
   while (*str != '\0'){
    usart_write(COM, *str);
 
    str++;
   }
}

int main(void) {

    // Desenha a tela principal
    draw_square(0, 0, tamanho_tabuleiro_x, tamanho_tabuleiro_y, color_black);
    usart_init(COM);
    is_transmit_empty(COM);
   
    while (1) {     
    }

    return 0;
}