#include "S32K144.h" /* include peripheral declarations S32K144 */
#define T 500000/125 //periodo en nano segundos dividido entre 125 nanosegundos, 125 = 8 MHz


#define adelante (1<<14)+(1<<7) //Seleccionamos pines 7 y 14;
#define atras (1<<17)+(1<<15) //Seleccionamos pines 15 y 17;
#define derecha (1<<17)+(1<<7) //Seleccionamos pines 7 y 17;
#define izquierda (1<<15)+(1<<14) //Seleccionamos pines 15 y 14;
#define alto (0<<17)+(0<<15)+(0<<14)+(0<<7)

//detener bandera LPIT0->TMR[0].TCTRL&=~(1<<0);

unsigned char duty_cycle=100;
unsigned char espejo_pin;
unsigned char i=0;
unsigned char cuentas=500;
unsigned long secuencia [4]={(adelante),(atras),(derecha),(adelante)};
unsigned long secuencia_canales_adc={(1<<6)+12,(1<<6)+9,(1<<6)+8};


/*
Activamos con pulsos el puente h
Motor derecho
7 adelante
15 atras

Motor izquierdo
14 adelante
17 atras

*/

void LPIT0_Ch0_IRQHandler (void)

{
	S32_NVIC->ISER[48/32]=(1<<(48%32));
	LPIT0->MSR=1;					//Borrar bandera
	LPIT0->TMR[0].TCTRL&=~(1<<0);  //Escribimos en TVAL estando detenido el timer
	//ADC0->SC1[0]=(1<<6)+12

	PTC->PTOR=secuencia[i++];
	if(i==3) i=0;

	if (espejo_pin==1)
	{
		espejo_pin=0;
		LPIT0->TMR[0].TVAL=(((100-(unsigned int)duty_cycle)*T)/100);
	}
	else
	{
		espejo_pin=1;
		LPIT0->TMR[0].TVAL=((((unsigned int)duty_cycle)*T)/100);
	}
	LPIT0->TMR[0].TCTRL|=(1<<0);		//Enable->Trigger

}
void LPIT0_Ch1_IRQHandler (void)
{
	S32_NVIC->ISER[48/32]=(1<<(48%32));
	LPIT0->MSR=1;					//Borrar bandera
	LPIT0->TMR[1].TCTRL&=~(1<<0);  //Escribimos en TVAL estando detenido el timer

	PTC->PTOR=(1<<15)+(1<<17); //Seleccionamos pines 15 y 17;

	if (espejo_pin==1)
	{
		espejo_pin=0;
		LPIT0->TMR[1].TVAL=(((100-(unsigned int)duty_cycle)*T)/100);
	}
	else
	{
		espejo_pin=1;
		LPIT0->TMR[1].TVAL=((((unsigned int)duty_cycle)*T)/100);
	}
	LPIT0->TMR[1].TCTRL|=(1<<0);		//Enable->Trigger
}

void LPIT0_AllCh_init(void)
{

	PCC->PCCn[PCC_LPIT_INDEX]=(2<<24);
    PCC->PCCn[PCC_LPIT_INDEX]|=(1<<30);
	SCG->SIRCDIV=(1<<8);  //8MHz/1

	LPIT0->MCR=1;		//M_CEN=1;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	LPIT0->TMR[0].TCTRL=(0<<24)+(1<<23)+(1<<17)+(1<<18);
	LPIT0->TMR[1].TCTRL=(0<<24)+(1<<23)+(1<<17)+(1<<18);
	LPIT0->TMR[2].TCTRL=(0<<24)+(1<<23)+(1<<17)+(1<<18);
	LPIT0->TMR[3].TCTRL=(0<<24)+(1<<23)+(1<<17)+(1<<18);

}
void PORTC_init(void)
{
	PCC->PCCn[PCC_PORTC_INDEX]=(1<<30);
	PORTC->PCR[7]=(1<<8);
	PORTC->PCR[15]=(1<<8);
	PORTC->PCR[17]=(1<<8);
	PORTC->PCR[14]=(1<<8);
	PTC->PDDR=(1<<7)+(1<<15)+(1<<17)+(1<<14);
	PTC->PDOR=0;
}
void ADC0_IRQHandler (void)
{
	ADC0->SC1[0]=(1<<6)+12;
	//if (ADC0->R[0]<0x40) PTD->PDOR=(1<<16)+1;  //prender el rojo
	//if ((ADC0->R[0]>=0x40) && (ADC0->R[0]<0x80)) PTD->PDOR=(1<<16)+(1<<15);  //prender led azul
    //if (ADC0->R[0]>=0x80)PTD->PDOR=(1<<15)+1; //led verde
}
void ADC0_init(void)

{
	PCC->PCCn[PCC_ADC0_INDEX]=(2<<24);
	PCC->PCCn[PCC_ADC0_INDEX]|=(1<<30);


	ADC0->CFG1=(2<<5);      //DIVCLK=4, Mode: 8 bits, ALTCLK1 proviene PCC
	//ADC0->SC1[0]=(1<<6)+12;	//hab Intr ADC, seleccionar canal 12 (pot)

	S32_NVIC->ISER[39/32]=(1<<(39%32));
}
void LPTMR0_IRQHandler (void)
{

	LPTMR0->CSR|=(1<<7); //apagar la bandera habilitar interrupcion

	//if(direccion) PTB->PDOR=secuencia[(i++)%4];
	//else PTB->PDOR=secuencia[(i--)%4];

}

void LPTMR0_init(cuentas)
{
	PCC->PCCn[PCC_LPTMR0_INDEX]=(1<<30); //prendemos bit 30, pag. 551
	LPTMR0->PSR=5; //Bypass preescaler, clock source= 1KHz
	LPTMR0->CMR=cuentas-1; // 100 cuentas de 1 ms
	//LPTMR0->CSR=(1<<6)+1; //habilita interrupcion LPTMR0, enable
	S32_NVIC->ISER[58/32]=(1<<(58%32));
}

int main(void)
{
	SCG->SIRCDIV=(1<<8);	//SIRCDIV2=8MHz/1=8MHz
	PORTC_init();
	LPIT0_AllCh_init();
	espejo_pin=0;


	//LPIT0->TMR[0].TVAL=(((100-(unsigned int)duty_cycle)*T)/100);
	//LPIT0->MIER=1;					//Interrupt Enable TMR0
	//LPIT0->TMR[0].TCTRL|=1;			//Hab conteo



	while (1);

	return 0;
}
