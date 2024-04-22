#include <xc.h>
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 16000000

//constant values
#define T_clk           62.5 //[ns]
#define V_pulse         3.3 // [V]
#define R_r             1.00212 // [MOM] 1MOM < R_r < 10 MOm
#define V_threshold     2.18 //0,66*V_pulse
#define DAC             170 //DAC register value for mesurment, 2.18 V
#define LOG_VALUE       1.080593783165 //-ln(1 - V_threshold/V_pulse)
#define WSP_A           -6.5643818437e-7
#define WSP_B           0.0029126242209
#define WSP_C           -1.2504845490698
#define WSP_D           162.6680448334

//variables
volatile char end_conv, receive, usart_data;
volatile double time_value;
volatile char time_value_l,time_value_h;
volatile double C, Ccorr;
char DAC_n;

//prototypes
//initialize
void init_pic(void);
//usart
void WriteUSART(char data);
void WriteStringUSART(char* data);
void WriteStringCUSART(const char* data);
char ReadUSART(void);
//measurment
void measurmentT(void);
//discharge
void discharge(void);
//calculations
double calculateT(double t1, double Rr, double log);
double calculateCcorr(double t1, double A, double B, double C, double D);
//exposition
void expose_result(void);

//interrupt services
void __interrupt() ISR(void)
{
  //comparator
  if(PIR2bits.C1IF)
    {
        time_value_h=TMR1H;
        time_value_l=TMR1L;
        end_conv=0; //after rising edge on comparator gate stop timer, interrupt from comparator stops conversion
        PIR2bits.C1IF=0; //clear flag
    }  
  
  //usart
  if(PIR1bits.RCIF)
   {
    usart_data = ReadUSART(); //write received data to usart_data
    WriteUSART(usart_data);
    receive=1; //set receive for main function
    PIR1bits.RCIF=0; //clear flag
   }
  
  //IOC SW3(RB1)
  if(IOCBFbits.IOCBF1)
  {
      while(!PORTBbits.RB1){LATBbits.LATB4=0;} //diode light when hold button 
      LATBbits.LATB4=1; //switch of diode
      IOCBF=0x00; //clear flag
  }
  
  //IOC SW2(RB2)
  if(IOCBFbits.IOCBF2)
  {
      while(!PORTBbits.RB2){LATBbits.LATB3=0;} //diode light when hold button
      LATBbits.LATB3=1; //switch off diode 
      IOCBF=0x00; //clear flag
  }
}

//main function
void main() 
{
    //initalize
    init_pic();
    receive=0;
    end_conv=0;
    
    DAC_n=DAC;
    
    //Menu
    WriteStringCUSART((const char*)"\n\rPraca inzynierska - Damian Fryza\n\r");
    WriteStringCUSART((const char*)"------------------------------------\n\r");
    WriteStringCUSART((const char*)"Menu:\n\r");
    WriteStringCUSART((const char*)"1-pojedynczy pomiar pojemnosci\n\r");
    WriteStringCUSART((const char*)"2-pomiar wielokrotny pojemnosci excel\n\r");
    WriteStringCUSART((const char*)"3-pomiar wielokrotny pojemnosci matlab\n\r");
    WriteStringCUSART((const char*)"r-wartosc DAC\n\r");
    WriteStringCUSART((const char*)"+-zwieksz wartosc napiecia wyjsciowego DAC\n\r");
    WriteStringCUSART((const char*)"--zmniejsz wartosc napiecia wyjsciowego DAC\n\r");
    WriteStringCUSART((const char*)">>");
    
    //infinite loop
    while(1)
    {
        if(receive)
        {
            receive=0;
            char string[80];
            if(usart_data=='1')
            {
                measurmentT();
                time_value=(time_value_h<<8)+time_value_l;
                C=calculateT(time_value,R_r,LOG_VALUE); //calculate C in [pF]
                Ccorr=calculateCcorr(time_value,WSP_A,WSP_B,WSP_C,WSP_D); //calculate Ccorr in [pF]
                
                expose_result();
            }
            else if(usart_data=='2')//excel
            {
                WriteStringCUSART((const char*)"\n\r");
                for(int i=0; i<240; i++)
                {
                    measurmentT();
                    time_value=(time_value_h<<8)+time_value_l;
                    C=calculateT(time_value,R_r,LOG_VALUE); //calculate C in [pF]
                    Ccorr=calculateCcorr(time_value,WSP_A,WSP_B,WSP_C,WSP_D); //calculate Ccorr in [pF]
                    
                    sprintf(string,"%f",C);
                    WriteStringCUSART(string);
                    
                    sprintf(string,"%f",C-Ccorr);
                    WriteStringCUSART(string);
                    
                    sprintf(string,"%f",time_value*T_clk*0.001);
                    WriteStringCUSART(string);
                    WriteStringCUSART((const char*)"\n\r");
                }
                WriteStringCUSART((const char*)">>");
            }
            else if(usart_data=='3')//matlab
            {
                WriteStringCUSART((const char*)"\n\rX=[");
                for(int i=0; i<240; i++)
                {
                    measurmentT();
                    time_value=(time_value_h<<8)+time_value_l;
                    C=calculateT(time_value,R_r,LOG_VALUE); //calculate C in [pF]
                    Ccorr=calculateCcorr(time_value,WSP_A,WSP_B,WSP_C,WSP_D); //calculate Ccorr in [pF]
                    
                    sprintf(string,"%f",C);
                    WriteStringCUSART(string);
                    
                    sprintf(string,"%f",C-Ccorr);
                    WriteStringCUSART(string);
                    
                    sprintf(string,"%f",time_value*T_clk*0.001);
                    WriteStringCUSART(string);
                    WriteStringCUSART((const char*)"\n\r");
                }
                WriteStringCUSART((const char*)"];\n\r");
                WriteStringCUSART((const char*)">>");
            }
            else if(usart_data=='r')
            {
                sprintf(string,"%d",DAC_n);
                WriteStringCUSART((const char*)"\n\rDAC=");
                WriteStringUSART(string);
                WriteStringCUSART((const char*)"\n\r");
                WriteStringCUSART((const char*)">>");
            }
            else if(usart_data=='+')
            {
                DAC_n++;
                DAC1CON1=DAC_n;
                sprintf(string,"%d",DAC_n);
                WriteStringCUSART((const char*)" DAC+= ");
                WriteStringUSART(string);
                WriteStringCUSART((const char*)"\n\r");
                WriteStringCUSART((const char*)">>");
            }
            else if(usart_data=='-')
            {
                DAC_n--;
                DAC1CON1=DAC_n;
                sprintf(string,"%d",DAC_n);
                WriteStringCUSART((const char*)" DAC-= ");
                WriteStringUSART(string);
                WriteStringCUSART((const char*)"\n\r");
                WriteStringCUSART((const char*)">>");
            }
            else
            {
                WriteStringCUSART((const char*)"Zly wybor\n\r");
                WriteStringCUSART((const char*)">>");
            }
        }
    }
}

//initalize functions
void init_pic(void)
{
	//internal oscyllator 16 MHz
	OSCCON=0x78; 
	
    //ports
    TRISA=0xF6; //RA3, RA0 output
    TRISB=0xE7; //RB4 and RB3 outputs - leds
    TRISC=0xBF; //RC6 output - TxD
    TRISD=0xFF; //port D as input
    TRISE=0xFF; //port E as input

    ANSELA=0x02; //RA1 analog
    ANSELB=0x00;
    ANSELC=0x00;
    ANSELD=0x00;
    ANSELE=0x00; 
    
    LATAbits.LATA3=0; //dont generate pulse at beginning
    LATAbits.LATA0=0; //ground
    LATBbits.LATB3=1; //switch off leds
    LATBbits.LATB4=1; //switch off leds

    //interupts
    INTCON=0xD8; //GIE=1, PEIE=1, INTE=1, IOCIE=1, INTF=0, IOCIF=0
    
    PIE1=0x20; //RCIE=1, received interrupt on
    PIE2=0x20; //C1IE=1, comparator interrupt on
    PIE3=0x00;
    PIE4=0x00;
    
    PIR1=0x00; //USART RECEIVE FLAG
    PIR2=0x00; //COMPARATOR 1 FLAG   
    
    IOCBP=0x00;
    IOCBN=0x06; // interrupt on change for RB1 and RB2
    IOCBF=0x00; //interrupt on change flag

    //usart
    TXSTA=0x20; //TX9=0, TXEN=1, SYNC=0, SENDB=0, BRGH=0,
    RCSTA=0x90; //SPEN=1, RX9=0, CREN=1
    BAUDCON=0x00;
    SPBRG=((_XTAL_FREQ/64)/9600) - 1; // OSC/BAUD_RATE/64 - 1 (25 dec)(19 hex)

    //comparators
    CM2CON0bits.C2ON=0;
    CM3CON0bits.C3ON=0;
    CM4CON0bits.C4ON=0;// CM2, CM3, CM4 disabled
    
    CM1CON0=0x14; //CM1 disabled(enabled 80), polarity inverted, 
				  //internal output, latency filter disabled, 
				  //histeresis disabled, normal power high speed, asynchronus
    CM1CON1=0xA9; //rising edge interrupt, + DAC1 output, - C1IN1-(RA1)

    //dac
    DAC1CON0=0xA0; //DAC1 enabled(DAC1EN=1), external output RA1, 
				   //Positive Vdd, Negativ Vss
    DAC1CON1=DAC; //DAC1 output voltage

    //timer
    T1CON=0x44; //system clock, prescaler 1:1, dedicated circut disabled, 
				//asynchronus, timer stop
    T1GCON=0x82; // timer 1 gate disabled, counting when gate is low, 
				 //gate source is rising edge of comparator1 
    TMR1L=0x00; 
    TMR1H=0x00; //clear timer1
    
}

//usart functions
void WriteUSART(char data)
{
  while(!PIR1bits.TXIF);  // Is the transmit shift register empty
  TXREG = data; 	   // Write the data byte to the USART
  
  return;
}

void WriteStringUSART(char* data)
{
  do
  {  // Transmit a byte
    while(!TXSTAbits.TRMT);  // Is the transmit shift register empty
    TXREG = *data;	     // Write the data byte to the USART
  } while( *data++ );
  
  return;
}

void WriteStringCUSART(const char* data)
{
  do
  {  // Transmit a byte
    while(!TXSTAbits.TRMT);  // Is the transmit shift register empty
    TXREG = *data;	     // Write the data byte to the USART
  } while( *data++ );
  
  return;
}

char ReadUSART(void)
{
  while(!PIR1bits.RCIF); // Is the receive shift register empty
  return RCREG;   // Return the received data
}

//mesurment functions
void measurmentT(void)
{
    discharge();
    DAC1CON0bits.DAC1EN=1; //DAC1 enable
    DAC1CON1=DAC; //DAC1 output voltage

    TMR1L=0x00; 
    TMR1H=0x00; //clear timer1
    
    T1CONbits.TMR1ON=1; //switch on timer1
    LATAbits.LATA3=1; //start generating pulse
    CM1CON0bits.C1ON=1; //comparator on
    
    end_conv=1;
    while(end_conv);

    T1CONbits.TMR1ON=0; //switch off timer1
    LATAbits.LATA3=0; //stop generating pulse
    CM1CON0bits.C1ON=0; //comparator off
    DAC1CON0bits.DAC1EN=0; //dac off
    
    return;
}

//discharge function
void discharge(void)
{
    TRISA=0xF4; //RA3, RA1, RA0 as outputs
    ANSELA=0x00; //RA1 digital
    
    LATAbits.LATA0=0; //ground
    LATAbits.LATA1=0; //for discharge RA1 need to be gnd
    LATAbits.LATA3=0; //dont generate pulse
    
    LATBbits.LATB3=0;
    __delay_ms(1000); //wait for discharge
    LATBbits.LATB3=1;
    
    TRISA=0xF6; //RA3, RA0 output
    ANSELA=0x02; //RA1 analog
    LATAbits.LATA0=0; //ground
    LATAbits.LATA3=0; //dont generate pulse
    
    return;
}

//calculate functions
double calculateT(double t1, double Rr, double log)
{
    double Cx;
    
    Cx=(double)(t1*T_clk);
    Cx=(double)Cx/(Rr*log);
    Cx=Cx*0.001;
    
    return Cx;
}

double calculateCcorr(double t1, double A, double B, double C, double D)
{
    double Ccorr;
    double time=t1*T_clk*0.001;
    
    Ccorr=D;
    Ccorr=Ccorr+C*time;
    Ccorr=Ccorr+B*time*time;
    Ccorr=Ccorr+A*time*time*time;
    
    return Ccorr;
}

//value exposition
void expose_result(void)
{
    char string[80];
    
    WriteStringCUSART((const char*)"\n\rt_h=");
    sprintf(string,"%d",time_value_h);
    WriteStringCUSART(string);
    WriteStringCUSART((const char*)"\n\r");
    
    WriteStringCUSART((const char*)"t_l=");
    sprintf(string,"%d",time_value_l);
    WriteStringCUSART(string);
    WriteStringCUSART((const char*)"\n\r");
    
    WriteStringCUSART((const char*)"t=");
    sprintf(string,"%d",time_value);
    WriteStringCUSART(string);
    WriteStringCUSART((const char*)"\n\r");
    
    WriteStringCUSART((const char*)"t_us=");
    sprintf(string,"%lf",time_value*T_clk/1000);
    WriteStringCUSART(string);
    WriteStringCUSART((const char*)"\n\r");

    WriteStringCUSART((const char*)"C=");
    sprintf(string,"%lf",C);
    WriteStringCUSART(string);
    WriteStringCUSART((const char*)" pF\n\r");
    
    WriteStringCUSART((const char*)"Ccorr=");
    sprintf(string,"%lf",Ccorr);
    WriteStringCUSART(string);
    WriteStringCUSART((const char*)" pF\n\r");
    
    WriteStringCUSART((const char*)"C-Ccorr=");
    sprintf(string,"%lf",C-Ccorr);
    WriteStringCUSART(string);
    WriteStringCUSART((const char*)" pF\n\r");
    
    WriteStringCUSART((const char*)">>");
    
    return;
}