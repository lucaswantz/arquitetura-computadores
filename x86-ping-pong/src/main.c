#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

/* Controle do x e y da bola */
volatile int xBola = 0;
volatile int yBola = 0;

/* Controle dos pontos */
volatile int pontosPlayer1 = 0;
volatile int pontosPlayer2 = 0;
volatile int corPonto = 9;

/* coleta dos potenciometros */
volatile unsigned char buffer[3];
volatile unsigned int index = 0;
volatile unsigned int pot1 = 0;
volatile unsigned int pot2 = 0;
volatile int pot1Atual = 0;
volatile int pot1Swap = 0;
volatile int pot2Atual = 0;
volatile int pot2Swap = 0;

/* Sentido da bola 0(esqueda), 1(direita) 
	 0(subindo) e 1(descendo) */
volatile int sentidoBolaX = 1;
volatile int sentidoBolaY = 1;

/* Configuração das barras */
volatile int barraPlayer1[5];
volatile int barraPlayer2[5];

/* Modo de jogo 0 single e 1 dual player */
volatile int modoDeJogo = 1;
volatile int yBolaSwap = 0;

// TIMER quando passar de 100ms 
ISR(TIMER3_COMPA_vect){

	/* Caminhar da bola*/
	if (xBola == 39) {

		pontosPlayer1++;

		switch (pontosPlayer1)
		{
			case 1:

			desenhar1px(0, 0, corPonto);
			desenharBola(xBola, yBola, 7);
			xBola = 21;
			yBola = 14;	
			break;

			case 2:

			desenhar1px(2, 0, corPonto);
			desenharBola(xBola, yBola, 7);
			xBola = 21;
			yBola = 14;
			break;

			case 3:
			desenhar1px(4, 0, corPonto);
			desenharTelaFim();
			exit(0);
			break;

		}

	} else if ((xBola == 37 && yBola == barraPlayer2[0])
		|| (xBola == 37 && yBola == barraPlayer2[1]) 
		|| (xBola == 37 && yBola == barraPlayer2[2])
		|| (xBola == 37 && yBola == barraPlayer2[3])
		|| (xBola == 37 && yBola == barraPlayer2[4])) {

		sentidoBolaX = 0;

	} else if (xBola == 0) {

		pontosPlayer2++;

		switch (pontosPlayer2)
		{
			case 1:

			desenhar1px(39, 0, corPonto);
			desenharBola(xBola, yBola, 7);
			xBola = 21;
			yBola = 14;	
			break;

			case 2:

			desenhar1px(37, 0, corPonto);
			desenharBola(xBola, yBola, 7);
			xBola = 21;
			yBola = 14;
			break;

			case 3:
			desenhar1px(35, 0, corPonto);
			desenharTelaFim();
			exit(0);
			break;

		}

	} else if ((xBola == 2 && yBola == barraPlayer1[0])
		|| (xBola == 2 && yBola == barraPlayer1[1]) 
		|| (xBola == 2 && yBola == barraPlayer1[2])
		|| (xBola == 2 && yBola == barraPlayer1[3])
		|| (xBola == 2 && yBola == barraPlayer1[4])) {

		sentidoBolaX = 1;

	}

	if (yBola == 28) {

		sentidoBolaY = 1;

	} else if (yBola == 1) {

		sentidoBolaY = 0;

	}

	desenharBola(xBola, yBola, 7);

	if (sentidoBolaX == 1) {

		xBola++;

	} else if (sentidoBolaX == 0) {

		xBola--;

	}

	if (sentidoBolaY == 0) {

		yBola++;

	} else if (sentidoBolaY == 1) {

		yBola--;

	}

	desenharBola(xBola, yBola, 0);

	/* Define se o jogo vai ser single ou duas mode */
	switch (modoDeJogo)
	{

		case 0:

		if ((pot1Atual != pot1Swap) || (pot2Atual != pot2Swap)) {

			apagarPlayer(1);
			apagarPlayer(2);
			desenharPlayer(1, pot1Atual, 5, 1);
			desenharPlayer(2, pot2Atual, 5, 1);

			pot1Swap = pot1Atual;
			pot2Swap = pot2Atual;

		}			
		break;

		default:

		/* modo single player */
		if ((pot1Atual != pot1Swap)) {

			apagarPlayer(1);
			desenharPlayer(1, pot1Atual, 5, 1);
			pot1Swap = pot1Atual;

		}

		if ((yBola == (yBolaSwap + 2)) || (yBola == (yBolaSwap - 2))) {

			yBolaSwap = yBola;
			apagarPlayer(2);
			desenharPlayer(2, yBolaSwap, 5, 1);
			
		}

		break;

	}

}

// Inciaialaiza o teimer 2
void init_timer3(int tempo) {

	//Configurando os registradores do timer
	TCCR3A = 0x00;
	TCCR3B = 0x00;
	TCCR3C = 0x00;

	//Precisa disso porque a contagem é muito rápida (aproximandamente 4 ms para contar de 0 até FFFF)
	TCCR3B |= (1 << 2) | (1 << 0);

	//Configura o timer para modo CTC
	TCCR3B |= (1 << 3);

	//Habilitar a interrupção de overflow do Timer 1 (quando chegar no valor máximo)
	TIMSK3 = 1 << 1;

	 // Os ciclos definem o tempo da interrupção casa ciclo equivale a 100ms
	OCR3A = (1562*tempo);
}

void usart_init0(void) {
	//Configurar o baud rate (taxa de transmissao e recepção dos dados pela porta serial)
	int baud = 103;

		//Dois registradores uma com parte alta e outra com parte baixa
		UBRR0H = baud >> 8; //Desloca os 8 primeiros bits
		UBRR0L = baud & 0x00FF; //Zera a parte alta

		/* Habilitar o modo da USART: transmissao e recepção dos dados  */
		UCSR0B = (1 << 3) | (1 << 4);

		//Para compatibilidade com futuros dispositivos colocamos esse bit com 0
		//Zeramos esse bit para garantir que somente a interrupção guardará
		// 1 quando tiver a primeira interrupção
		//UCSR0A &= ~(1 << 5);

		//Habilita as interrupções de recebimento
		UCSR0B |= (1 << 7);

		/* Configurar o tamanho dos frames, número de bits de parada e a paridade
			frame: 8 bits
			sem paridade
			1 bit de parada (stop bit)
		 */
		UCSR0C = (1 << 2) | (1 << 1);

		 //UCSR0B |= (1 << RXCIE0);

		// sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
	}
	void usart_init1(int taxa) {
		//Configurar o baud rate (taxa de transmissao e recepção dos dados pela porta serial)
		unsigned short baud = ((F_CPU / 16) / (taxa) - 1);

		//Dois registradores uma com parte alta e outra com parte baixa
		//unsigned short ubrr0 = ((F_CPU / 16) / (baud) - 1);
		UBRR1H = baud >> 8; //Desloca os 8 primeiros bits
		UBRR1L = baud & 0x00FF; //Zera a parte alta

		/* Habilitar o modo da USART: transmissao e recepção dos dados  */
		UCSR1B = (1 << 3) | (1 << 4);

		//Para compatibilidade com futuros dispositivos colocamos esse bit com 0
		//Zeramos esse bit para garantir que somente a interrupção guardará
		// 1 quando tiver a primeira interrupção
		//UCSR0A &= ~(1 << 5);

		//Habilita as interrupções de recebimento
		UCSR1B |= (1 << 7);

		/* Configurar o tamanho dos frames, número de bits de parada e a paridade
			frame: 8 bits
			sem paridade
			1 bit de parada (stop bit)
		 */
		UCSR1C = (1 << 2) | (1 << 1);

		 //UCSR1B |= (1 << RXCIE0);

		  sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
		}

		void usart_putc1(char c){
		/* Verificar se pode enviar um novo dado */
			while (!(UCSR1A & (1 << UDRE1)));

			UDR1 = c;

	    // Enquanto a transmissao não estiver finalizada
			while (!(UCSR1A & (1 << TXC1)));
		}

		char get_char0(void){
		//Verifica se tem uma informacao para ler
			if (UCSR0A & (1 << 7)) {
				return UDR0;
			}

		//Senao existem dados para ler retorna -1
			return -1;
		}

		void usart_putc0(char c){
		/* Verificar se pode enviar um novo dado */
			while (!(UCSR0A & (1 << UDRE0)));

			UDR0 = c;

	    // Enquanto a transmissao não estiver finalizada
			while (!(UCSR0A & (1 << TXC0)));
		}

		void usart_puts0(char *str){
			while (*str) {
				usart_putc0(*str++);
			}
		}

		void usart_puts1(char *str){
			while (*str) {
				usart_putc1(*str++);
			}
		}

		ISR(USART1_RX_vect){

			/* Pegar o retorno do UDR1*/
			// usart_putc0(UDR1);

			/* Coleta dos potenciometros */
			if(UDR1 == 0x54) {
				index = 0;
			} else {  

				buffer[index ++] = UDR1;
				if(index == 3) {
					pot1 = ((buffer[1] & 0x03) << 8) | (buffer[2] & 0xFF);
					pot2 = ((buffer[0] & 0x0F) << 6) | ((buffer[1] & 0xFC) >> 2);

					pot1Atual = (pot1 * 30) / 1023;
					pot2Atual = (pot2 * 30) / 1023; 

				}
			} 
		}

		/* Fução para pintar a tela*/
		void corTela(int bordas, int centro) {

		usart_putc1(0x26);
		usart_putc1(0x78);
		usart_putc1(centro & 0xFF); // x
		
		
		usart_putc1(0x26); // y        
		usart_putc1(0x72); // H
		usart_putc1(0 & 0xFF); // w
		usart_putc1(1 & 0xFF); // c
		usart_putc1(28 & 0xFF); // h
		usart_putc1(40 & 0xFF); // w
		usart_putc1(bordas & 0xFF); // Color

	}

	/* Desenha a bola*/
	void desenharBola(int x, int y, int cor) {

		usart_putc1(0x26);
		usart_putc1(0x72);
		usart_putc1(x & 0xFF); // x
		usart_putc1(y & 0xFF); // y
		usart_putc1(1 & 0xFF); // h
		usart_putc1(1 & 0xFF); // w
		usart_putc1(cor & 0xFF); // Color

		xBola = x;
		yBola = y;

	}

	void desenharPlayer(int player, int y, int a, int l) {
		
		int x = 1;
		int i;

		if (player == 1) {

			x = 1;
		} else {
			x = 38;
		}

		if (y >= 24) {

			y = 24;
		}

		usart_putc1(0x26);
		usart_putc1(0x72);
		usart_putc1(x & 0xFF); // x
		usart_putc1(y & 0xFF); // y
		usart_putc1(a & 0xFF); // h
		usart_putc1(l & 0xFF); // w
		usart_putc1(0x04); // Color

		/* Proteção da barra quando a bola bate */
		if (player == 1) {

			for (i = 0; i < 5; i++) {


				barraPlayer1[i] = y;
				y++;
			} 
		} else {

			for (i = 0; i < 5; i++) {

				barraPlayer2[i] = y;
				y++;
			} 

		}
	}

	void apagarPlayer(int player) {
		
		int x = 38;
		
		if (player == 1) {

			x = 1;

		}

		usart_putc1(0x26);
		usart_putc1(0x72);
		usart_putc1(x & 0xFF); // x
		usart_putc1(1 & 0xFF); // y
		usart_putc1(28 & 0xFF); // h
		usart_putc1(1 & 0xFF); // w
		usart_putc1(0x07); // Color

	}

	void desenhar1px(int x, int y, int cor) {
		
		usart_putc1(0x26);
		usart_putc1(0x72);
		usart_putc1(x & 0xFF); // x
		usart_putc1(y & 0xFF); // y
		usart_putc1(1 & 0xFF); // h
		usart_putc1(1 & 0xFF); // w
		usart_putc1(cor & 0xFF); // Color

	}

	void desenharTelaFim(void) {

		/* Letra F */
		desenhar1px(11,12,9);
		desenhar1px(11,13,9);
		desenhar1px(11,14,9);
		desenhar1px(11,15,9);
		desenhar1px(11,16,9);
		desenhar1px(11,17,9);
		desenhar1px(12,12,9);
		desenhar1px(13,12,9);
		desenhar1px(12,14,9);
		
		/* Letra I */
		desenhar1px(16,12,9);
		desenhar1px(16,13,9);
		desenhar1px(16,14,9);
		desenhar1px(16,15,9);
		desenhar1px(16,16,9);
		desenhar1px(16,17,9);

		/* Letra M */
		desenhar1px(19,12,9);
		desenhar1px(19,13,9);
		desenhar1px(19,14,9);
		desenhar1px(19,15,9);
		desenhar1px(19,16,9);
		desenhar1px(19,17,9);
		desenhar1px(20,12,9);
		desenhar1px(21,12,9);
		desenhar1px(21,13,9);
		desenhar1px(21,14,9);
		desenhar1px(22,12,9);
		desenhar1px(23,12,9);
		desenhar1px(23,13,9);
		desenhar1px(23,14,9);
		desenhar1px(23,15,9);
		desenhar1px(23,16,9);
		desenhar1px(23,17,9);

	}

	int main(void){
		usart_init0(); // Inicializa a porta serial 0
		usart_init1(9600);//  Inicializa a porta serial 1

		/* Pinta o fundo e as bordas centro e bordas */
		corTela(7,4);
		
		/* Desenha a bola inicial */
		desenharBola(20,8,0);

		/* Desenha o player o y é de cima para baixo e o tamanho também 
		se no y colocar 2, ele começa na linha 3 até a 7 */
		desenharPlayer(1, pot1Atual, 5, 1);
		pot1Swap = pot1Atual;
		desenharPlayer(2, pot2Atual, 5, 1);
		pot2Swap = pot2Atual;	

		/* Inicializa o timer em 100ms */
		init_timer3(1);
		sei();

		/* Define se vai ser single ou dual mode */
		if (pot1 < 10) {

			modoDeJogo = 0;
		} else {

			modoDeJogo = 1;
		}

		while (1) {

		}

		return 0;
	}