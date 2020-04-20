#include <sys/io.h>
#include <gfx/video.h>

// Define o tamanho do tabuleiro (ex 1024 X 768).
static int tabuleiro_x = 1024;
static int tabuleiro_y = 768;

// Controles
static int altura_barra = 200;
static int velocity = 20;

static int y1 = 234;
static int y2 = 234;
static int dy1 = 0;
static int dy2 = 0;

int x_bola = 512;
static int y_bola = 312;
static int dy_bola = 0;
static int dx_bola = 0;

int i = 0;

/*
Risquinhos do p1 começam no 482 e vão DECREMENTANDO de 10 em 10
Risquinhos do p2 começam em 540 e vão INCOREMENTANDO de 10 em 10 
*/
int score_p1 = 482;
int score_p2 = 540;

int COM = 0x3f8; /* COM1 */

int velocidade = 5;

void desenha_score_p1() {

  if (score_p1 > 100) {
    draw_square(score_p1, 15, 5, 25, 0x000000);
    score_p1 -= 10;
  }
}

void desenha_score_p2() {

  if (score_p2 < 924) {
    draw_square(score_p2, 15, 5, 25, 0x000000);
    score_p2 += 10;
  }
}

void desenha_p1() {
  draw_square(60, y1, 20, altura_barra, 0xFF0000);
  y1 += dy1;
  draw_square(60, y1, 20, altura_barra, 0x000000);
  if (y1 < 0) {
    y1 = 0;
  }
  if (y1 > 568) {
    y1 = 568;
  }
}

void desenha_p2() {
  draw_square(tabuleiro_x - 80, y2, 20, altura_barra, 0xFF0000);
  y2 += dy2;
  draw_square(tabuleiro_x - 80, y2, 20, altura_barra, 0x000000);
  if (y2 < 0) {
    y2 = 0;
  }
  if (y2 > 568) {
    y2 = 568;
  }
}

void desenha_bola() {
  
  draw_square(x_bola, y_bola, 30, 30, 0XFF0000); 

  /* Se a bola saiu do esquadro, ela é projetada no meio novamente */
//   if(x_bola >= 974 || y_bola <= 50){
//     x_bola = 512;    y_bola = 312;    dx_bola = 0;    dy_bola = 0;
//   } 
   if(y_bola<0){
      y_bola= y_bola*(-1);
      }else{
         y_bola= y_bola;
      }
	if(x_bola<0){x_bola= x_bola*(-1);}else{x_bola= x_bola;} 

  if(i == 1){
              x_bola -= velocidade;
      y_bola -= velocidade;  
       } else if(i == 0){
      x_bola += velocidade;
      y_bola += velocidade;
  }

  if(y_bola >= 600){
     i = 1;
  }

   
  draw_square(x_bola, y_bola, 30, 30, 0X0000FF);
}


int desenha_pontilhado() {
  for (int i = 1; i < tabuleiro_y; i += 8) {
    draw_square(tabuleiro_x / 2, i, 2, 2, 0x000000);
  }
}

/* Vai ser executada a cada 18x por segundo, aproximadamente */
void isr0(void) {
    desenha_p1();
    desenha_p2();
    desenha_pontilhado();
    desenha_score_p1();
    desenha_score_p2();
    desenha_bola();
    dx_bola += 1;
    dy_bola += 1;

}

void isr1(void) {
  char keycode;
  keycode = inb(0x60);

  if (keycode & 0x80) { // Verifica se a tecla foi pressionada
    keycode &= ~0x80; // Devolve o bit mais   
    if (keycode == 17) { // W 
      dy1 = 0;
    }
    if (keycode == 72) { // Seta para cima
      dy2 = 0;
    }
    if (keycode == 31) { // S
      dy1 = 0;
    }
    if (keycode == 80) { // Seta para baixo
      dy2 = 0;
    }

  } else {
    if (keycode == 31) { // S
      dy1 = velocity;
    }
    if (keycode == 80) { // Seta para baixo
      dy2 = velocity;
    }
    if (keycode == 17) { // W 
      dy1 = -velocity;
    }
    if (keycode == 72) { // Seta para cima
      dy2 = -velocity;
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
   draw_square(0, 0, tabuleiro_x, tabuleiro_y, 0xFF0000);
   usart_init(COM);
   is_transmit_empty(COM);
   
  while (1) {
     
  }

  return 0;
}