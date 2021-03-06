#include "E:\Embedded\tm4c123gh6pm.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

void SystemInit() {}

void UART0_Init(void);
void UART1_Init();
void uart0_write(char c);
char uart1_read(void);

void getdata(float* N, float* E);
float chartonum(char a[10], char l);
float mintodeg(float x);

float difference(float i, float j);
float distance(float x, float y);

void LED_Init(void);

















int main()
{

	float N1, N2, E1, E2;
	int i, j;
	float d = 0;

	UART0_Init();
	UART1_Init();
	while (1) {

	    getdata(&N1, &E1);
		getdata(&N2, &E2);

		d += distance(difference(N1, N2), difference(E1, E2));

	}

}






void LED_Init(void)
{
	SYSCTL_RCGCGPIO_R |= 0x20;
	while ((SYSCTL_PRGPIO_R & 0x20) == 0) {};
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R |= 0x0E;
	GPIO_PORTF_DIR_R |= 0X0E;
	GPIO_PORTF_DEN_R |= 0x0E;
	GPIO_PORTF_AMSEL_R &= ~0x0E;
	GPIO_PORTF_AFSEL_R &= ~0x0E;
	GPIO_PORTF_PCTL_R &= ~0x0000FFF0;

}




void uart0_write(char c)
{
	while ((UART0_FR_R & 0x20) != 0);
	UART0_DR_R = c;

}



char uart1_read(void)
{
	while ((UART1_FR_R & 0x10) != 0);
	return (UART1_DR_R & 0xFF);

}



float difference(float i, float j) {
	float dif;
	if (i > j) {
		dif = i - j;
	}
	else {
		dif = j - i;
	}

	return dif;



}



float mintodeg(float x) {
	float minx = x / 100;
	int xint = minx;
	float deg = minx - xint;
	deg = deg / 60 + minx;
	printf("%f", deg);
	return deg;
}



float chartonum(char a[10], char l) {
	float x = 0;
	int i, j;
	int temp;
	if (l == 'n') { //g for longitude
		j = 4;
	}
	else {
		j = 3;
	}


	for (i = 0; i < 10; i++) {
		if (a[i] == '.') {
			i++;
		}
		temp = a[i] - '0';
		//x += x ;
		x += (temp * pow(10, (j)));
		j--;
	}
	x = mintodeg(x);
	return(x);
}

float distance(float x, float y) {
	float d = sqrt((x* x) + (y* y));
	return(d);
}






void UART0_Init(void) {

	{
		SYSCTL_RCGCUART_R |= 0x00000001;
		SYSCTL_RCGCGPIO_R |= 0x01;
		UART0_CTL_R &= ~0X01;

		UART0_LCRH_R |= 0X0000070;
		UART0_IBRD_R = 104;
		UART0_FBRD_R = 11;
		UART0_CTL_R |= 0X00000301;

		GPIO_PORTA_AFSEL_R |= 0X02;
		GPIO_PORTA_PCTL_R = ((GPIO_PORTA_PCTL_R &= ~0X000000FF) | 0X00000011);
		GPIO_PORTA_DEN_R |= 0X03;

	}
}

void UART1_Init()
{
	SYSCTL_RCGCUART_R |= 0x02;
	while ((SYSCTL_PRUART_R & 0x02) == 0);
	SYSCTL_RCGCGPIO_R |= 0x02;
	while ((SYSCTL_PRGPIO_R & 0x02) == 0);

	GPIO_PORTB_AFSEL_R = 0X03;
	GPIO_PORTB_AMSEL_R = 0X00;
	GPIO_PORTB_PCTL_R = 0X00000000;
	GPIO_PORTB_PCTL_R = 0X00000011;
	GPIO_PORTB_DEN_R |= 0X03;


	UART1_CTL_R &= ~0X01;
	UART1_LCRH_R |= 0X0000070;
	UART1_IBRD_R = 104;
	UART1_FBRD_R = 11;
	UART1_CTL_R |= 0X301;




}






//gps read func







void getdata(float* N, float* E) {

		int i, j;

		char arr[7], arr1[26];
		char arrN[9], arrE[10];

		char ggp[6] = { 'G','P','G','L','L','A' };
		char m;

		m = uart1_read();

		if (m == '$') {
			m = uart1_read();

			for (i = 0; i < 6; i++) {
				arr[i] = uart1_read();
				if (arr[i] == ggp[i] & i == 5) {

					for (i = 0; i < 25; i++) {
						arr[i] = uart1_read();
					}

					for (j = 0; j < 8; j++)
					{
						arrN[j] = arr[i + 1];
					}
					for (j = 0; j < 9; j++)
					{
						arrE[j] = arr1[i + 16];
					}




				}
				else {
					break;
				}
			}
		}


	


	*N = chartonum(arrN, 'n');
	*E = chartonum(arrE, 'e');

}



