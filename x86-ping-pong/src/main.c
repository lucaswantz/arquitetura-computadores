#include <sys/io.h>
#include <gfx/video.h>
#include <sys/kmap.h>

const COR_BOLA   = #40b9ff;
const COR_FUNDO  = #000000;
const COR_LINHA  = #FFFFFF;
const COR_PLAYER = #FFFFFF;

static int x = 0;
static int y = 10;

static int dy = 0;
static int dx = 0;

static int t = 0;

void isr1(void)
{
    char keycode;

    /* 0xxx xxxx -> pressionamento de uma tecla. */
    /* 1xxx xxxx -> soltar a tecla. */
    keycode = inb(0x60);

    /* Testa se a tecla foi solta. Senão, foi pressionada. */
    if (keycode & 0x80) {
        keycode &= ~0x80;

        if (keycode == 16) {
            dx = 0;
        }

        if (keycode == 17) {
            dy = 0;
        }

        if (keycode == 31) {
            dx = 0;
        }
    } else {
        /* Q */
        if (keycode == 16) {
            dx = -1;
        }

        /* W */
        if (keycode == 17) {
            dy = 2;
        }

        /* S */
        if (keycode == 31) {
            dx = 2;
        }
    }

    /*
    if (keycode >= 0) {
        draw_square(x, y, 50, 50, 0xFF0000);
        x += 10;
        draw_square(x, y, 50, 50, 0x0000FF);
    }
    */
}

void draw_campo() {
    draw_square(0, 0, 1024, 768, COR_FUNDO);
    draw_square(0, ((1024/2) - 1), 2, 768, COR_LINHA);
}



int main(void)
{
    long int i = 0;

    
    draw_square(20, 20, 20, 100, 0x000000);

    while (1) {
        for (i = 0; i < 1e5; i++);

        
        draw_campo()
        //draw_square(0, 0, 1024, 768, COR_FUNDO);

        //draw_square(x, y, 50, 50, 0xFF0000);
        //y += dy;
        //x += dx;
        //draw_square(x, y, 50, 50, 0x0000FF);
    }

    return 0;
}