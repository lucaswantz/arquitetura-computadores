#include <sys/io.h>
#include <gfx/video.h>

// Define as cores do projeto
static char color_black = 0x000000; // #000000
static char color_white = 0xFFFFFF; // #FFFFFF
static char color_debug = 0x00AA00; // #00AA00

// Musica para tocar
static volatile int pause = 0;
static int musica_idx = 0;
//static int musica[] = { 0, 1, 2, 3, 4, 5, 6 }
static int musica[] = {2, 7, 2, 7, 3, 7, 4, 7, 4, 7, 3, 7, 2, 7, 1, 7, 0, 7, 0, 7, 1, 7, 2, 7, 2, 7, 1, 7, 1};

// Tabela de notas
//                         dó   ré   mi   fá   sol  lá   si   pausa
static long int tones[] = {523, 587, 659, 698, 783, 880, 987, 0};

// Define o endereço base da porta serial
static int COM = 0x3f8; /* COM1 */

// Define o tamanho do tabuleiro (ex 1024 X 768).
static int x_size_tabuleiro = 1024;
static int y_size_tabuleiro = 768;

// Indica o tamanho da barra
static int altura_barra = 200;

// Define a velocidade do movimento dos jogadores
static int velocidade_controle = 20;

// Indica a posição atual dos players
static int x_position_player_1 = 60;
static int y_position_player_1 = 234;
static int x_size_player_1 = 20;

static int x_position_player_2 = 944;
static int y_position_player_2 = 234;
static int x_size_player_2 = 20;

// Indica a movimentação dos players
static int dy_player_1 = 0;
static int dy_player_2 = 0;

// INFORMAÇÕES DA BOLA
// Indica o tamanho bola
static int x_size_bola = 20;
static int y_size_bola = 20;

// Indica a posição inicial da bola
static int x_initial_bola = 512;
static int y_initial_bola = 334;

// Indica a posição atual da bola
static int x_bola = 512;
static int y_bola = 334;

// Indica a movimentação da bola
static int dy_bola = 10;
static int dx_bola = 10;

int i = 0;

/*
Risquinhos do p1 começam no 482 e vão DECREMENTANDO de 10 em 10
Risquinhos do p2 começam em 540 e vão INCOREMENTANDO de 10 em 10
*/
static int score_p1 = 0;
static int score_p2 = 0;

//static int score_p1 = 482;
//static int score_p2 = 540;

void toca_musica(int type)
{
    int len = sizeof(musica) / sizeof(musica[0]);
    int nota = 0;
    long int tone = 0;

    switch (type)
    {
    case 0:
        nota = musica[musica_idx];
        tone = tones[nota];

        if (pause == 0)
        {
            musica_idx++;

            if (musica_idx >= len)
                musica_idx = 0;

            if (tone != 0)
            {
                play_sound(tone);
            }
            else
            {
                stop_sound();
            }
        }
        break;

    case 1:
        play_sound(tones[3]);
        play_sound(tones[1]);
        //stop_sound();
        break;

    case 2:
        if (pause == 0)
        {
            if (tone == 0)
                stop_sound();
        }
        break;

    default:
        break;
    }

    pause++;

    if (pause == 5)
        pause = 0;
}

void play_sound(long int freq)
{
    long int div;
    unsigned char tmp;

    div = 1193180 / freq;
    outb(0x43, 0xB6);
    outb(0x42, div);
    outb(0x42, div >> 8);

    tmp = inb(0x61);
    if ((tmp & 0x03) != 0x03)
        outb(0x61, tmp | 0x03);
}

void stop_sound()
{
    unsigned char tmp;
    tmp = inb(0x61);
    outb(0x61, tmp & 0xFC);
}

void desenha_pontuacao_p1()
{
    int position = 482;

    for (int i = 0; i < score_p1; i++)
    {
        draw_square(position, 15, 5, 25, color_white);
        position -= 10;
    }
}

void desenha_pontuacao_p2()
{
    int position = 540;

    for (int i = 0; i < score_p2; i++)
    {
        draw_square(position, 15, 5, 25, color_white);
        position += 10;
    }
}

void desenha_player_1()
{
    draw_square(x_position_player_1, y_position_player_1, 20, altura_barra, color_black);

    y_position_player_1 += dy_player_1;
    draw_square(x_position_player_1, y_position_player_1, 20, altura_barra, color_white);

    if (y_position_player_1 < 0)
        y_position_player_1 = 0;

    if (y_position_player_1 > 568)
        y_position_player_1 = 568;
}

void desenha_player_2()
{
    draw_square(x_position_player_2, y_position_player_2, 20, altura_barra, color_black);

    y_position_player_2 += dy_player_2;
    draw_square(x_position_player_2, y_position_player_2, 20, altura_barra, color_white);

    if (y_position_player_2 < 0)
        y_position_player_2 = 0;

    if (y_position_player_2 > 568)
        y_position_player_2 = 568;
}

int bola_bate_no_player_1()
{
    if (dx_bola > 0)
    {
        return 0;
    }

    // Posição de X for menor ou igual a posição do jogador
    if (x_bola <= (x_position_player_1 + x_size_player_1))
    {
        // Posicao y da bola for entre a posição do taco
        if ((y_bola + y_size_bola) > y_position_player_1 &&
            (y_bola) < (y_position_player_1 + altura_barra))
        {
            return 1;
        }
    }

    return 0;
}

int bola_bate_no_player_2()
{
    if (dx_bola < 0)
    {
        return 0;
    }

    // Posição de X for menor ou igual a posição do jogador
    if ((x_bola + x_size_bola) >= (x_position_player_2))
    {
        // Posicao y da bola for entre a posição do taco
        if ((y_bola + y_size_bola) > y_position_player_2 &&
            (y_bola) < (y_position_player_2 + altura_barra))
        {
            return 1;
        }
    }

    return 0;
}

void desenha_bola()
{
    draw_square(x_bola, y_bola, x_size_bola, y_size_bola, color_black);

    /* Se a bola saiu do esquadro, ela é projetada no meio novamente */
    //   if(x_bola >= 974 || y_bola <= 50){
    //     x_bola = 512;    y_bola = 312;    dx_bola = 0;    dy_bola = 0;
    //   }

    x_bola += dx_bola;
    y_bola += dy_bola;

    // CONTROLA OS LIMITES DO EIXO X
    if (x_bola <= 0)
    {
        x_bola = x_initial_bola;
        y_bola = y_initial_bola;
        score_p2++;
        toca_musica(1);
    }

    if (x_bola + x_size_bola >= x_size_tabuleiro)
    {
        x_bola = x_initial_bola;
        y_bola = y_initial_bola;
        score_p1++;
        toca_musica(1);
    }

    draw_square(x_bola, y_bola, x_size_bola, y_size_bola, color_white);

    // CONTROLA OS LIMITES DO EIXO Y
    if (y_bola <= 0)
        dy_bola *= -1;

    if (y_bola + y_size_bola >= y_size_tabuleiro)
        dy_bola *= -1;

    // CONTROLA O CONTATO COM OS JOGADORES
    if (bola_bate_no_player_1() || bola_bate_no_player_2())
        dx_bola *= -1;
}

int desenha_pontilhado()
{
    for (int i = 1; i < y_size_tabuleiro; i += 10)
    {
        draw_square(x_size_tabuleiro / 2, i, 2, 4, color_white);
    }
}

// INTERRUPÇÃO DO TIMER +/- 18X POR SEGUNDO
void isr0(void)
{
    //toca_musica(0);
    toca_musica(2);

    desenha_player_1();
    desenha_player_2();

    desenha_pontilhado();

    desenha_pontuacao_p1();
    desenha_pontuacao_p2();

    desenha_bola();
}

// INTERRUPÇÃO QUE CUIDA DAS TECLAS PRESSIONADAS
void isr1(void)
{
    char keycode;
    keycode = inb(0x60);

    // Se uma tecla foi pressionada
    if (keycode & 0x80)
    {
        // Desconta o bit mais significativo, para que consiga ler a tecla
        keycode &= ~0x80;

        // TECLA W
        if (keycode == 17)
            dy_player_1 = 0;

        // TECLA S
        if (keycode == 31)
            dy_player_1 = 0;

        // TECLA UP
        if (keycode == 72)
            dy_player_2 = 0;

        // TECLA DOWN
        if (keycode == 80)
            dy_player_2 = 0;
    }
    else
    {
        // TECLA W
        if (keycode == 17)
            dy_player_1 = -velocidade_controle;

        // TECLA S
        if (keycode == 31)
            dy_player_1 = velocidade_controle;

        // TECLA DOWN
        if (keycode == 80)
            dy_player_2 = velocidade_controle;

        // TECLA UP
        if (keycode == 72)
            dy_player_2 = -velocidade_controle;
    }
}

void usart_init(int base_addr)
{
    outb(base_addr + 1, 0x00); // Disable all interrupts
    outb(base_addr + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(base_addr + 0, 0x01); // Set divisor to 1 (lo byte) 115200 baud
    outb(base_addr + 1, 0x00); //                  (hi byte)
    outb(base_addr + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(base_addr + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(base_addr + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

int serial_received(int PORT)
{
    return inb(PORT + 5) & 1;
}

char read_serial(int PORT)
{
    //while (serial_received() == 0);

    if (serial_received(PORT) == 0)
        return 0;

    return inb(PORT);
}

int is_transmit_empty(int PORT)
{
    return inb(PORT + 5) & 0x20;
}

// Função para escrever um caracter
void usart_write(int PORT, unsigned char c)
{
    while (is_transmit_empty(PORT) == 0)
    {
    }

    outb(PORT, c);
}

// Função para escrever uma string
void usart_puts(char *str)
{

    while (*str != '\0')
    {
        usart_write(COM, *str);
        str++;
    }
}

// Inicializa o jogo
int main(void)
{
    // Desenha a tela principal
    draw_square(0, 0, x_size_tabuleiro, y_size_tabuleiro, color_black);

    // Inicializa para poder transmitir dados pro console
    usart_init(COM);
    is_transmit_empty(COM);

    while (1)
    {
    }

    return 0;
}
