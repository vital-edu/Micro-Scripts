#include <msp430g2553.h>
#include <legacymsp430.h>

#define RX BIT1
#define TX BIT2
#define BTN BIT3
#define LED1 BIT0
#define LED2 BIT6
#define START_MEDICINE 20
#define START_DOSAGE BIT4
#define END_DOSAGE BIT5

volatile unsigned char count = 0, blink;
volatile unsigned char microsegs;
volatile unsigned char state = 0;
volatile unsigned char medicine_count = START_MEDICINE;
 
// Função que fornece um atraso de x micro-segundos
void Atraso(volatile unsigned int x);

void display(volatile unsigned char number)
{
  P2OUT = 0b00000000;
  if (number == 0) {
    P2OUT = 0b00111111;
  } else if (number == 1) {
    P2OUT = 0b00000110;
  } else if (number == 2) {
    P2OUT = 0b01011011;
  } else if (number == 3) {
    P2OUT = 0b01001111;
  } else if (number == 4) {
    P2OUT = 0b01100110;
  } else if (number == 5) {
    P2OUT = 0b01101101;
  } else if (number == 6) {
    P2OUT = 0b01111101;
  } else if (number == 7) {
    P2OUT = 0b00000111;
  } else if (number == 8) {
    P2OUT = 0b01111111;
  } else if (number == 9) {
    P2OUT = 0b01101111;
  } else {
    // erro?
    P2OUT = 0b01111001;
  }
}
 
 
int main(void)
{
        // Desligar Watchdog Timer
        WDTCTL = WDTPW + WDTHOLD;
        P2DIR = 0b11111111; // tudo no p2 é output
        P2SEL &= ~(BIT6 | BIT7); //2.6 e 2.7 como output
        // MCLK e SMCLK @ 1 MHz
        BCSCTL1 = CALBC1_1MHZ;
        DCOCTL = CALDCO_1MHZ;

        // Habilitar pinos para entrada e saída UART
        P1SEL2 = P1SEL = RX+TX;
        // Saída digital para pinos dos LEDs
        P1DIR = LED1+LED2;
        P1DIR |= (BIT4 + BIT5);
        // Habilitar resistor de pull-up no pino do botão
        // e habilitar interrupção por borda de descida
        P1IE = P1IES = P1OUT = P1REN = BTN;
       
        // UART: desabilitar paridade, transmitir um byte
        // na ordem LSB->MSB, um bit de stop
        UCA0CTL0 = 0;
        // UART: utilizar SMCLK
        UCA0CTL1 = UCSSEL_2;
        // Baud rate de 9600 bps, utilizando oversampling
        UCA0BR0 = 6;
        UCA0BR1 = 0;
        UCA0MCTL = UCBRF_8 + UCOS16;
       
        // Habilitar interrupção por recebimento de byte via UART
        IE2 |= UCA0RXIE;
        for(;;)
        {
                // Entrar em modo de baixo consumo, habilitar interrupções
                _BIS_SR(LPM0_bits+GIE);
                // Após o recebimento de um byte via UART,
                // a proxima linha de codigo sera executada
               
                // Piscar o LED2 "blink" vezes
                blink = 2;
                while(blink)
                {
                        P1OUT ^= LED2;
                        Atraso(250);
                        P1OUT ^= LED2;
                        Atraso(250);
                        blink--;
                }
                // Apagar flag de interrupção via porta P1,
                // provavelmente setada devido ao bounce do botao
                P1IFG = 0;
                // Reabilitar interrupções da porta P1
                P1IE = BTN;
        }
        return 0;
}
 
// Interrupção da porta P1, chamada quando
// se aperta o botão da Launchpad
interrupt(PORT1_VECTOR) Send_Data(void)
{
  // Acender LED1, indicando começo do uso da UART
  P1OUT |= LED1;
  while((IFG2 & UCA0TXIFG) == 0);
  P1OUT &= ~(BIT5);
  P1OUT &= ~(BIT4);
  P1OUT &= ~(LED1);
  // Aguardar UART estar pronta para poder enviar um byte
  // Incrementar variavel count e envia-la via UART
  UCA0TXBUF = 49;
  if(medicine_count < 1) {
    // ACABOU O ESTOQUE
  } else {
    medicine_count -= 1;
  }
  //display(count);
  // Atraso(5000);
  // Desabilitar interrupções da porta P1
  P1IE = P1IFG = 0;
}
 
// Interrupção por recebimento de byte via UART
interrupt(USCIAB0RX_VECTOR) Receive_Data(void)
{
        // Guardar valor recebido via UART na variavel blink
        blink = UCA0RXBUF;
        blink -= 48;

        //BLINK = AÇÃO A SER FEITA
        if(blink == 9) {
          // RESETA O TOTAL DE REMÉDIOS
          medicine_count = START_MEDICINE;
        } else if(blink == 8) {
          // INICIO DO HORARIO QUE PODE TOMAR O REMEDIO
          P1OUT |= START_DOSAGE;
        } else if(blink == 7) {
          P1OUT &= ~(START_DOSAGE);
          // FIM DO HORARIO QUE PODE TOMAR O REMEDIO
        } else if(medicine_count > 0) {
          // tem remedios no estoque
          P1OUT |= END_DOSAGE;
        }

        display(blink);
        // Retornar ao main() sem modo de baixo consumo
        LPM0_EXIT;
        // Apagar LED1, indicando fim do uso da UART
        P1OUT &= ~LED1;
}
 
// Função que fornece um atraso de x micro-segundos
void Atraso(volatile unsigned int x)
{
        TACCR0 = 999;
        TACTL = TASSEL_2 + ID_0 + MC_1 + TAIE;
        microsegs = x;
        // Entrar em modo de baixo consumo
        _BIS_SR(LPM0_bits+GIE);
}
 
// Interrupção habilitada na função Atraso()
interrupt(TIMER0_A1_VECTOR) TA1_ISR(void)
{
        if(microsegs) microsegs--;
        else
        {
                TACTL &= ~(MC0+MC1);
                LPM0_EXIT;
        }
        TACTL &= ~TAIFG;
}
