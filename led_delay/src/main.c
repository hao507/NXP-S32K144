
#include "S32K144.h" /* include peripheral declarations S32K144 */

void delay (void)
{
unsigned long i=1000000;

do{}while (--i);
}

int main(void)
{
	//Activate clock PCC register description PCC_PORTD Chapter 29 page 633
	PCC->PCCn[PCC_PORTD_INDEX]=0x40000000;

	//Select port (
//	Mux) PORT_PCR Chapter 12  PORT_PCRn page 198
	PORTD->PCR[0]=0x00000100;
	PORTD->PCR[15]=0x00000100;
	PORTD->PCR[16]=0x00000100;

	//Port Data direction PDDR Activar GPIO Chapter 13 page 218
	//Activamos los pines de los puertos elegidos
	//16 green
	//15 red
	//0 blue
	PTD->PDDR=0x00018001;


        for(;;) {
        	//Port Data direction PDOR Chapter 13 page 218
        	//PTD->PDOR=0x00018000;
        	//verde, rojo, azul
        	PTD->PDOR=(1<<16);
        	delay();
        	PTD->PDOR=(1<<15);
        	delay();
        	PTD->PDOR=1;
        	delay();


        }

	return 0;
}
