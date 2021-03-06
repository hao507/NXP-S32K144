#include "S32K144.h" /* include peripheral declarations S32K144 */

//string GPIO
//unsigned long secuencia[4]={0x00004000,0x00008000, 0x00010000, 0x00020000};
unsigned long secuencia[4]={0x00020000,0x00008000, 0x00004000, 0x00010000};

void delay (void)
{
unsigned long i=1000000;
do{}while (i--);
}

int main(void)
{
unsigned char i;
//Activate clock PCC register description PCC_PORTB Chapter 29 page 633
		PCC->PCCn[PCC_PORTB_INDEX]=0x40000000;

		//Select port (Mux) PORT_PCR Chapter 12  PORT_PCRn page 198
		PORTB->PCR[17]=0x00000100;
		PORTB->PCR[16]=0x00000100;
		PORTB->PCR[15]=0x00000100;
		PORTB->PCR[14]=0x00000100;

		//Activamos los pines de los puertos elegidos
		PTB->PDDR=0x0003C000;

		// CGC Activate clock PCC register description PCC_PORTB Chapter 29 page 633
		PCC->PCCn[PCC_PORTC_INDEX]=0x40000000;
		//Select port (Mux) PORT_PCR Chapter 12  PORT_PCRn page 198
		PORTC->PCR[13]=0x00000100;

        for(;;) {
        	//PDIR Input PDOR output

        	if ((PTC->PDIR & (0x00002000))==0) PTB->PDOR=secuencia[(i++)%4];
        	else PTB->PDOR=secuencia[(i--)%4];
        	delay();
        }

	return 0;
}
