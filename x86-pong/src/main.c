#include <sys/io.h>
#include <gfx/video.h>

/* COM */
#define COM1 0x3F8
#define COM2 0x2F8

volatile static int tamanho_tabuleiro_x = 1024;
volatile static int tamanho_tabuleiro_y = 768;

volatile static int altura_barra = 150;
volatile static int largura_barra = 10;
volatile static int velocidade = 10;
volatile static int y1 = 300;
volatile static int y2 = 300;
volatile static int dy1 = 0;
volatile static int dy2 = 0;
volatile static int tmp_y1 = 300;
volatile static int tmp_y2 = 300;

/* Controles da bola */
volatile static int x_bola = 312;
volatile static int y_bola = 312;
volatile static int tmp_x_bola = 312;
volatile static int tmp_y_bola = 312;
volatile static int largura_bola = 10;
volatile static int altura_bola = 10;
volatile static int velocidade_bola = 10;

volatile static int iniciar = 0;
volatile static int jogador = 0;
volatile static int status = 0;

volatile static int sobe_desce = 0x00;

/*
Emite som quando pressionado
*/

volatile static int som_p1 = 500;
volatile static int som_p2 = 700;

void tocar_musica(long int frequencia)
{
    long int div;
    unsigned char tmp;

    div = 1193180 / frequencia;
    outb(0x43, 0xB6);
    outb(0x42, div);
    outb(0x42, div >> 8);

    tmp = inb(0x61);
    if ((tmp & 0x03) != 0x03)
    {
        outb(0x61, tmp | 0x03);
    }
}

void parar_musica(void)
{
    unsigned char tmp;
    tmp = inb(0x61);
    outb(0x61, tmp & 0xFC);
}

/* Direção da bola, onde 1=Esquerda e 0=Direita */
volatile static int direcao = 1;

/*
Risquinhos do p1 começam no 482 e vão DECREMENTANDO de 10 em 10
Risquinhos do p2 começam em 540 e vão INCOREMENTANDO de 10 em 10
*/

volatile int score_p1 = 482;
volatile int score_p2 = 540;

volatile int tmp_score_p1 = 482;
volatile int tmp_score_p2 = 540;

void desenha_score_p1()
{
    draw_square(score_p1, 25, 5, 25, 0xFFFFFF);
    score_p1 -= 10;
}

void desenha_score_p2()
{
    draw_square(score_p2, 25, 5, 25, 0xFFFFFF);
    score_p2 += 10;
}

void desenha_pontilhado()
{
    for (int i = 1; i < tamanho_tabuleiro_y; i += 8)
    {
        draw_square(tamanho_tabuleiro_x / 2, i, 2, 2, 0xFFFFFF);
    }
}

void desenha_p1()
{
    if (jogador == 1)
    {
        draw_square(60, y1, largura_barra, altura_barra, 0x000000);
        y1 += dy1;
        draw_square(60, y1, largura_barra, altura_barra, 0xFFFFFF);
        if (y1 < 0)
        {
            y1 = 0;
        }
        if (y1 > 568)
        {
            y1 = 568;
        }
    }
    else if (jogador == 2)
    {
        draw_square(60, y1, largura_barra, altura_barra, 0xFFFFFF);
    }
}

void desenha_p2()
{
    if (jogador == 1)
    {
        draw_square(tamanho_tabuleiro_x - 80, y2, largura_barra, altura_barra, 0x000000);
        y2 += dy2;
        draw_square(tamanho_tabuleiro_x - 80, y2, largura_barra, altura_barra, 0xFFFFFF);
        if (y2 < 0)
        {
            y2 = 0;
        }
        if (y2 > 568)
        {
            y2 = 568;
        }
    }
    else if (jogador == 2)
    {
        draw_square(tamanho_tabuleiro_x - 80, y2, largura_barra, altura_barra, 0xFFFFFF);
    }
}

void desenha_bola()
{
    if (jogador == 1)
    {
        draw_square(x_bola, y_bola, largura_bola, altura_bola, 0x000000);
        /* Olha se tocou na borda direita */
        if (x_bola + largura_bola >= tamanho_tabuleiro_x - 80)
        {
            /* Se não tocou na barra, incrementa o placar e redesenha a bola no meio */
            if (y_bola + largura_bola < y2 || y_bola > y2 + altura_barra)
            {
                desenha_score_p1();
                x_bola = 312;
                y_bola = 312;
            }
            else
            {
                direcao = 4;
            }
        }

        /* Olha se tocou na borda esquerda */
        if (x_bola <= 80)
        {
            /* Se não tocou na barra, incrementa o placar e redesenha a bola no meio */
            if (y_bola < y1 || y_bola > y1 + altura_barra)
            {
                desenha_score_p2();
                x_bola = 312;
                y_bola = 312;
                /* Se não tocou, muda a direção */
            }
            else
            {
                direcao = 0;
            }
        }

        if (y_bola + largura_bola >= tamanho_tabuleiro_y)
        {
            direcao = 3;
        }

        if (y_bola + largura_bola <= 20)
        {
            direcao = 1;
        }

        if (direcao == 1)
        {
            x_bola -= velocidade_bola; //de cima para baixo / esquerda
            y_bola += velocidade_bola;
        }
        else if (direcao == 0)
        {
            x_bola += velocidade_bola; //de cima para baixo / direita
            y_bola += velocidade_bola;
        }
        else if (direcao == 3)
        {
            x_bola += velocidade_bola; //de baixo para cima / esquerda
            y_bola -= velocidade_bola;
        }
        else if (direcao == 4)
        {
            x_bola -= velocidade_bola; //de baixo para cima / direita
            y_bola -= velocidade_bola;
        }

        draw_square(x_bola, y_bola, largura_bola, altura_bola, 0XFFFFFF);
    }
    else if (jogador == 2)
    {
        draw_square(x_bola, y_bola, largura_bola, altura_bola, 0X000000);
    }
}

/* Vai ser executada a cada 18x por segundo, aproximadamente */
void isr0(void)
{
    if (iniciar)
    {
        desenha_pontilhado();
        desenha_bola();
        desenha_p1();
        desenha_p2();
        if (jogador == 1)
        {
            envia_dados_P2();
        }
        else if (jogador == 2)
        {
            envia_dados_P1();
        }
    }
}

void isr1(void)
{
    char keycode;
    keycode = inb(0x60);

    if (keycode & 0x80)
    {
        keycode &= ~0x80;
        // W
        if (keycode == 17 && jogador == 1)
        {
            parar_musica();
            dy1 = 0;
        }

        // Seta cima
        if (keycode == 72 && jogador == 2)
        {
            parar_musica();
            dy2 = 0;
            sobe_desce = 0x00;
        }

        // S
        if (keycode == 31 && jogador == 1)
        {
            parar_musica();
            dy1 = 0;
        }

        // Seta baixo
        if (keycode == 80 && jogador == 2)
        {
            parar_musica();
            dy2 = 0;
            sobe_desce = 0x00;
        }
    }
    else
    {
        // S
        if (keycode == 31 && jogador == 1)
        {
            tocar_musica(som_p1);
            dy1 = velocidade;
        }

        // Seta baixo
        if (keycode == 80 && jogador == 2)
        {
            tocar_musica(som_p2);
            dy2 = velocidade;
            sobe_desce = 0x02;
        }

        // W
        if (keycode == 17 && jogador == 1)
        {
            tocar_musica(som_p1);
            dy1 = -velocidade;
        }

        // Seta cima
        if (keycode == 72 && jogador == 2)
        {
            tocar_musica(som_p2);
            dy2 = -velocidade;
            sobe_desce = 0x01;
        }

        // Espaço
        if (keycode == 57 && jogador == 0)
        {
            iniciar = 1;
            jogador = 1;
            write_serial(0x05);
        }
    }
}

void isr3(void)
{
    unsigned char dados = inb(COM2);

    if (status == 0)
    {
        if (dados == 0xFF)
        {
            status = 1;
        }
        else if (dados == 0x05)
        {
            iniciar = 1;
            jogador = 2;
            status = 0;
        }
    }
    else if (status == 1)
    {
        if (jogador == 1)
        {
            if (dados == 0x01)
            {
                dy2 = -velocidade;
                // draw_square(tamanho_tabuleiro_x - 80, y2, largura_barra, altura_barra, 0xFF0000);
                status = 0;
            }
            else if (dados == 0x02)
            {
                dy2 = velocidade;
                // draw_square(tamanho_tabuleiro_x - 80, y2, largura_barra, altura_barra, 0xFF0000);
                status = 0;
            }
            else if (dados == 0x00)
            {
                dy2 = 0;
            }
        }
        else if (jogador == 2)
        {
            tmp_y1 = dados << 8;
            status = 2;
        }
    }
    else if (status == 2)
    {
        tmp_y1 |= dados;
        status = 3;
    }
    else if (status == 3)
    {
        tmp_score_p1 = dados;
        status = 4;
    }
    else if (status == 4)
    {
        tmp_y2 = dados << 8;
        status = 5;
    }
    else if (status == 5)
    {
        tmp_y2 |= dados;
        status = 6;
    }
    else if (status == 6)
    {
        tmp_score_p2 = dados;
        status = 7;
    }
    else if (status == 7)
    {
        tmp_x_bola = dados << 8;
        status = 8;
    }
    else if (status == 8)
    {
        tmp_x_bola |= dados;
        status = 9;
    }
    else if (status == 9)
    {
        tmp_y_bola = dados << 8;
        status = 10;
    }
    else if (status == 10)
    {
        tmp_y_bola |= dados;
        setDados();
        status = 0;
    }
}

void setDados()
{
    draw_square(60, y1, largura_barra, altura_barra, 0x000000);
    draw_square(x_bola, y_bola, largura_bola, altura_bola, 0x000000);
    draw_square(tamanho_tabuleiro_x - 80, y2, largura_barra, altura_barra, 0x000000);

    y1 = tmp_y1;
    y2 = tmp_y2;

    score_p1 = tmp_score_p1 + 472;
    score_p2 = tmp_score_p2 + 530;

    x_bola = tmp_x_bola;
    y_bola = tmp_y_bola;

    draw_square(60, y1, largura_barra, altura_barra, 0x000000);
    draw_square(x_bola, y_bola, largura_bola, altura_bola, 0X0000FF);
    draw_square(tamanho_tabuleiro_x - 80, y2, largura_barra, altura_barra, 0x000000);
}

void envia_dados_P2()
{
    score_p1 = score_p1 - 472;
    score_p2 = score_p2 - 530;

    write_serial(score_p1); //score_p1
    write_serial(score_p2); //score_p2

    write_serial(0xFF);
    write_serial(y1 >> 8);
    write_serial(y1);
    write_serial(y2 >> 8);
    write_serial(y2);
    write_serial(x_bola >> 8);
    write_serial(x_bola);
    write_serial(y_bola >> 8);
    write_serial(y_bola);
}

void envia_dados_P1()
{
    write_serial(0xFF);
    write_serial(sobe_desce);
}

void usart_init(int base_addr)
{
    outb(base_addr + 1, 0x00);
    outb(base_addr + 3, 0x80);
    outb(base_addr + 0, 0x01);
    outb(base_addr + 1, 0x00);
    outb(base_addr + 3, 0x03);
    outb(base_addr + 2, 0xC7);
    outb(base_addr + 4, 0x0B);
    outb(base_addr + 1, 0x01);
}

int is_transmit_empty(int base_addr)
{
    return inb(base_addr + 5) & 0x20;
}

void usart_write(int base_addr, char dados)
{
    while (is_transmit_empty(base_addr) == 0)
    {
    }

    outb(base_addr, dados);
}

void usart_puts(int base_addr, char *str)
{
    while (*str)
    {
        usart_write(base_addr, *str++);
    }
}

void write_serial(char dados)
{
    usart_write(COM2, dados);
}

int main(void)
{
    draw_square(0, 0, tamanho_tabuleiro_x, tamanho_tabuleiro_y, 0x000000);
    usart_init(COM1);
    usart_init(COM2);
    while (1)
    {
    }
    return 0;
}
