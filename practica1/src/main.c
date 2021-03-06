/*Jos� Abraham P�rez Mart�nez A01633926
You can change the velocity of the motor if you modify the value of the unsigned long i in delay function
fast = 50000
slow = 300000
Values of the module ULN2003
PTC17 = IN1
PTC14 = IN2
PTC15 = IN3
PTC16 = IN4

*/
#include "S32K144.h" /* include peripheral declarations S32K144 */

void delay (void)
{
	unsigned long i=50000;
	do{}while (i--);
}

void CW()
{

	  PTB->PDOR=(1<<17);
	  delay();
	  PTB->PDOR=(1<<14);
	  delay();
	  PTB->PDOR=(1<<15);
	  delay();
	  PTB->PDOR=(1<<16);
	  delay();
}

void CCW()
{
	PTB->PDOR=(1<<16);
	delay();
	PTB->PDOR=(1<<15);
	delay();
	PTB->PDOR=(1<<14);
	delay();
	PTB->PDOR=(1<<17);
	delay();
}
int main(void)
{
        //CGC Activate clock PCC
		PCC->PCCn[PCC_PORTB_INDEX]=0x40000000;

		//Select port (Mux)
		PORTB->PCR[17]=0x00000100;
		PORTB->PCR[16]=0x00000100;
		PORTB->PCR[15]=0x00000100;
		PORTB->PCR[14]=0x00000100;

		//Pin activate
		PTB->PDDR=0x0003C000;

		//CGC Activate clock PCC
		PCC->PCCn[PCC_PORTC_INDEX]=0x40000000;
		//Select port (Mux)
		PORTC->PCR[13]=0x00000100;

        for(;;) {
        	//Input
        	if ((PTC->PDIR & (0x00002000))==0) CW();
        	else CCW();

        }

	return 0;
}
