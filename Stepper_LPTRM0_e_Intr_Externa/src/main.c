/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "S32K144.h"
//unsigned long secuencia [4]={0X00000001,0X00000002,0X00000003,0X00000004};

//unsigned long secuencia [4]={0X00020000,0X00004000,0X00008000,0X00010000};
unsigned long secuencia [4]={(1<<17),(1<<14),(1<<15),(1<<16)};

unsigned char i=0;
unsigned char direccion=0;
void LPTMR0_IRQHandler (void)
{

	LPTMR0->CSR|=(1<<7); //apagar la bandera

	if(direccion) PTB->PDOR=secuencia[(i++)%4];
	else PTB->PDOR=secuencia[(i--)%4];

}

void PORTC_IRQHandler (void)
{
	PORTC->PCR[13]|=1<<24; //apagar bandera isf (w1c)
	direccion=~direccion; //negador a nivel de bits
}

int main(void)
{
	PCC->PCCn[PCC_PORTB_INDEX]=0x40000000;

			//Select port (Mux)
			PORTB->PCR[17]=0x00000100;
			PORTB->PCR[16]=0x00000100;
			PORTB->PCR[15]=0x00000100;
			PORTB->PCR[14]=0x00000100;

			//Pin activate
			PTB->PDDR=0x0003C000;

	PCC->PCCn[PCC_PORTC_INDEX]=0X40000000; //1<<30
	PORTC->PCR[13]=(9<<16)+(1<<8); //interrupcion flanco de salida, gpio
	S32_NVIC->ISER[61/32]=1<<(61%32); //PortC interrupt

	PTD->PDDR=0X00018001;

	PCC->PCCn[PCC_LPTMR0_INDEX]=(1<<30); //prendemos bit 30, pag. 551

	LPTMR0->PSR=5; //Bypass preescaler, clock source= 1KHz
	LPTMR0->CMR=100-1; // 100 cuentas de 1 ms
	LPTMR0->CSR=(1<<6)+1; //habilita interrupcion LPTMR0, enable

	S32_NVIC->ISER[58/32]=(1<<(58%32));




        for(;;) {

        }


	return 0;
}

