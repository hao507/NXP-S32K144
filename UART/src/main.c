#include "S32K144.h" /* include peripheral declarations S32K144 */

unsigned char mensaje1[]={11,12,13,14,15,16,17,0};  // 'H'<> "H"
unsigned char i=0;

void LPUART1_RxTx_IRQHandler(void){
	LPUART1->DATA=mensaje1[0];
    if(mensaje1[i]==0) LPUART1->CTRL&=~(1<<23);


}

int main(void)
{
	PCC->PCCn[PCC_PORTC_INDEX]=1<<30;
	PORTC->PCR[7]=2<<8;						//LPUART1 TX
	PORTC->PCR[6]=2<<8;						//LPUART1 RX

	SCG->SIRCDIV=1<<8;						//SIRCDIV2: 8 MHz/1

    PCC->PCCn[PCC_LPUART1_INDEX]=2<<24;		//SIRCDIV2
	PCC->PCCn[PCC_LPUART1_INDEX]|=1<<30;
	LPUART1->BAUD|=52;						//BAUD_SRG=CLK_UART/(16*9600)
	LPUART1->CTRL|=(1<<19)+(1<<18)+(1<<23);			//TE=RE=1

    S32_NVIC->ISER[33/32]=(1<<(33%32));


	while (1);
	return 0;
}
