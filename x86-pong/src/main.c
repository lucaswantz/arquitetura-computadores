#include <sys/io.h>
#include <gfx/video.h>

const cor_preto = 0x000000; // #000000
const cor_branco = 0xffffff; // #ffffff
const cor_rosa = 0x660066; // #660066

static int x = 0;
static int y = 100;
static int dy = 0;
static int y2 = 100;
static int dy2 = 0;

void isr1(void)
{
     char keycode;

    /* 0xxx xxxx -> pressionamento de uma tecla. */
    /* 1xxx xxxx -> soltar a tecla. */
    keycode = inb(0x60);

    /* Testa se a tecla foi solta. Senão, foi pressionada. */
    //W e S movimentam player 1
    //E e D movimentam player 2
    if (keycode & 0x80) {
        keycode &= ~0x80;
        /* W */
        if (keycode == 17) {
            dy = 0;
        }

        /* S */
        if (keycode == 31) {
            dy = 0;
        }

        //seta cima
        if (keycode == 18) {
            dy2 = 0;
        }
        //seta baixo
        if (keycode == 32) {
            dy2 = 0;
        }
    } else {
        /* W */
        if (keycode == 17) {
            if(dy >= 0){
                dy = -1;
            }
            
        }

        /* S */
        if (keycode == 31) {
            if(dy < 718){
                dy = 1;
            }
        }

        //seta cima
        if (keycode == 18) {
            dy2 = -1;
        }

        //seta baixo
        if (keycode == 32) {
            dy2 = 1;
        }
        
    }
}

void draw_board(void){
    draw_square(512, 0, 10, 768, cor_preto);
}

void draw_player1(int y){
    if(y <= 0){    
        draw_square(256, 0, 10, 50, cor_preto);
    }else if(y > 718){
        draw_square(256, 718, 10, 50, cor_preto);
    }else{
        draw_square(256, y, 10, 50, cor_preto);
    }
}
void erase_player1(int y){
     
    draw_square(256, y, 10, 50, cor_rosa);
    
}
void erase_player2(int y){
     
    draw_square(768, y, 10, 50, cor_rosa);
    
}

void draw_player2(int y){
     if(y <= 0){
         draw_square(768, 0, 10, 50, cor_preto);
     }else if(y > 718){
         draw_square(768, 718, 10, 50, cor_preto);
     }else{
         draw_square(768, y, 10, 50, cor_preto);
     }
}

void draw_ball(int x, int y){
    if(x > 0 && x <1024 && y > 0 && y < 768){
        draw_square(x, y, 20, 20, cor_preto);
    }
}

int main(void)
{
    long int i = 0;
    draw_square(0, 0, 1024, 768, cor_rosa);
    draw_board();
    
    
    
    draw_ball(450, 300);

    while (1) {
        for (i = 0; i < 1e5; i++);
        
        erase_player1(y);
        y += dy;
        draw_player1(y);

        erase_player2(y2);
        y2 += dy2;
        draw_player2(y2);
      
    }

    return 0;
}

