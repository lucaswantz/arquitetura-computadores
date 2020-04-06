#include <sys/io.h>
#include <gfx/video.h>

static int x = 0;
static int y = 10;

void isr1(void)
{
    char keycode;

    keycode = inb(0x60);

    if (keycode >= 0) {
        draw_square(x, y, 50, 50, 0xFF0000);
        x += 10;
        draw_square(x, y, 50, 50, 0x0000FF);
    }
}

int main(void)
{
    draw_square(0, 0, 1024, 768, 0xFF0000);

    draw_square(20, 20, 20, 100, 0x000000);

    while (1) {
    }

    return 0;
}

