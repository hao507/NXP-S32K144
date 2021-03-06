#include "S32K144.h" /* include peripheral declarations S32K144 */


void delay (unsigned int tiempo)
{
		LPTMR0->CMR=tiempo-1;
		LPTMR0->CSR=1;
		do{}while (((LPTMR0->CSR) & (1<<7))==0);
		LPTMR0->CSR=0;
}

int main(void)
{
	PCC->PCCn[PCC_PORTD_INDEX]=0x40000000; //1<<30
	PORTD->PCR[0]=0x00000100;
	//PORTD->PCR[15]=0x00000100;  //1<<8
	//PORTD->PCR[16]=0x00000100;
	PTD->PDDR=0x00018001;

	PCC->PCCn[PCC_LPTMR0_INDEX]=0x40000000;
	LPTMR0->CMR=500-1;			// 500 cuentas de 1 ms
	LPTMR0->PSR=5;				//Bypass preescaler, clock source=1 KHz (pag 1406)
	LPTMR0->CSR=(1<<6)+1;		//Hab Intr LPTRM0, Enable
	S32_NVIC->ISER[58/32]=(1<<(58%32));

        for(;;) {
			PTD->PDOR=0x00018000;
			delay(500);
			PTD->PDOR=0x00018001;
			delay(500);
        }

	return 0;
}