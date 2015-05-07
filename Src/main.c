/*
 * cartel_usart.c
 *
 * Created: 17/09/2012 08:15:34 p.m.
 *  Author: Christian
 */ 

#define	__need_NULL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#include "USART.h"

#include "cartel.h"
#include "cartel_comm.h"

//Fuentes disponibles incluidas
#include "font_base.h"
#include "ArialBlack_14.h"
#include "BanckGothicLtBT_8.h"
#include "BroadwayBT_8.h"

#define POS_REG_X	7
#define POS_REG_Y	27
#define POS_CRON_X	7
#define POS_CRON_Y	14

uint8_t f_timer=0;
uint8_t f_cro_pause=0; //actua sobre ambos cronos
uint16_t cont_timer_bri=0;

int main(void)
{	int16_t posx1=1,posy1=1,aux16,aux16_1=1,aux16_2=1;
	char str[100],auxstr[20];
	char* tok;
	uint8_t color=BLACK;
	uint8_t *font_vec[5];
	uint8_t bri_duty=100;

	uint16_t set_cr_prim=180;
	uint16_t set_cr_sec=30;
	uint8_t set_regr=5;
	uint16_t cont_cro_pri=0;
	uint16_t cont_cro_sec=0;
	uint8_t cont_cro_regr=0;
	uint8_t f_cro_pri=0;
	uint8_t f_cro_sec=0;
	uint8_t f_cro_regr=0;
	uint8_t ds10=0;

	CtInit();
	CtCommInit();

	sei();

	//timer 1
	OCR1A=25000; //tics para 0.1 seg, a Xtal de 16M
	TCCR1A=0b00000000;
	TCCR1B=0b00001011; //CTC, prescaler a 64, timer arrancado

	//timer 0
	OCR0=50; //tics para 0.2 mseg, a Xtal de 16M
	TCCR0=0b00001011; //CTC, prescaler a 64, timer arrancado

	TIMSK=0b00010010;//habilita interrupciones timer 0 y 1

	//carga de los vectores de fuentes
	font_vec[0]=Font_Base;
	font_vec[1]=arialblack_14;
	font_vec[2]=BanckGothicLtBT_8;
	font_vec[3]=BroadwayBT_8;
	font_vec[4]=0;

	CtClear();
	CtUpdate();

	//bucle prinsipal
	while(1)
		{
		 if(USARTRxCount())
			 {switch(CtCommDecoder())
				 {case 0:				break;
				  case CMD_HELP:	CtCommPrint(MSG_HELP);
										break;
				  case CMD_INVALID:	CtCommPrint(MSG_INV_COMM);
				  						break;
				  case INVALID_DATA:	CtCommPrint(MSG_INV_DATA);
										break;
				  case CMD_BRI_UP:	bri_duty+=5;
				  	  	  	  	  	if(bri_duty>100)
				  	  	  	  	  		bri_duty=100;
				  	  	  	  	  	break;
				  case CMD_BRI_DW:	if(bri_duty-5>=0)
					  	  	  	  	  	bri_duty-=5;
				  	  	  	  	  	else
				  	  	  	  	  		bri_duty=0;
				  	  	  	  	  	break;
				  case CMD_UPDATE:	//CtUpdate();
										CtCommPrint(MSG_OK);
				  				  		break;
				  case CMD_CLEAR:	CtClear();
				  						CtCommPrint(MSG_OK);
				  				  		break;
				  case CMD_CRONO:	if(!f_cro_pri)
							  	  	  {cont_cro_pri=set_cr_prim;
							  	  	   cont_cro_regr=set_regr;
							  	  	   f_cro_regr=1;
							  	  	   f_cro_pri=1;
							  	  	   f_cro_sec=0;
							  	  	   f_cro_pause=0;
							  	  	   f_timer=1;
							  	  	   ds10=8; //fuerza una primer entrada al bucle de cronometros
							  	  	   //CtUpdate();
							  	  	  }
									else
									  {f_cro_pri=0;
									   f_cro_sec=0;
									  }

				  					CtCommPrint(MSG_OK);
				  					break;
				  case CMD_CRO_SEC:	  if(f_cro_sec)
				  	  	  	  	  	  	  f_cro_sec=0;
				  	  	  	  	  	  else
				  	  	  	  	  	  	  {f_cro_sec=1;
				  	  	  	  	  	  	   cont_cro_sec=set_cr_sec;
				  	  	  	  	  	  	  }
				  	  	  	  	  	  CtCommPrint(MSG_OK);
					  	  	  	  	  break;
				  case CMD_CRO_PAUSE: if(f_cro_pause)
					  	  	  	  	  	  f_cro_pause=0;
				  	  	  	  	  	  else
				  	  	  	  	  		  f_cro_pause=1;
				  	  	  	  	  	  CtCommPrint(MSG_OK);
					  	  	  	  	  break;
				  case CMD_GOTO:		aux16_1=atoi(USARTGetStr(','));
										aux16_2=atoi(USARTGetStr('\r'));
										if(aux16_1==0 || aux16_2==0)
											CtCommPrint(MSG_INV_DATA);
										else
											{posx1=aux16_1;
											posy1=aux16_2;
											CtGoto(posx1,posy1);
											CtCommPrint(MSG_OK);
											}
										break;
				  case CMD_CRO_CONF:	set_cr_prim=atoi(USARTGetStr(','))*60+atoi(USARTGetStr(','));
				  	  	  	  	  	  	set_cr_sec=atoi(USARTGetStr(','));
				  						set_regr=atoi(USARTGetStr('\r'));
				  						CtCommPrint(MSG_OK);
				  						break;
				  case CMD_FONT_SEL:	aux16=((uint8_t)USARTGetChar())-0x30;
											if(aux16>0 && aux16<=5 && font_vec[aux16-1])
												{CtSelectFont((PGM_P)font_vec[aux16-1], color);
												 CtCommPrint(MSG_OK);
												}
											else
												CtCommPrint(MSG_INV_FONT);
											break;
				  case CMD_TEXT:		if(USARTIfOver())
												CtCommPrint(MSG_OVER);
											else
												{strcpy(str,USARTGetStr('\r'));
												 CtPuts(str,posx1,posy1);
												 CtCommPrint(MSG_OK);
												}
											break;
				  case CMD_POINT:		CtSetDot(posx1,posy1,color);
											CtCommPrint(MSG_OK);
											break;
				  case CMD_INVERT:		CtInvert();
											CtCommPrint(MSG_OK);
											break;
				  case CMD_LINE:		switch(USARTGetChar())
												{case 'v':	CtLineV(posy1,posx1,posx1+atoi(USARTGetStr('\r'))-1,color);
															CtCommPrint(MSG_OK);
															break;
												 case 'h':	CtLineH(posx1,posy1,posy1+atoi(USARTGetStr('\r'))-1,color);
															CtCommPrint(MSG_OK);
															break;
												 default: CtCommPrint(MSG_INV_DATA); break;
												}
											break;
				  case CMD_RECT:		aux16=atoi(USARTGetStr(',')); //ancho
											aux16_1=atoi(USARTGetStr(','));	//alto
											aux16_2=atoi(USARTGetStr('\r'));	//lleno-vacio
											if(-16>aux16_1 || aux16_1>16 || aux16_1==0 || -CT_ROW_DOTS>aux16 || aux16>CT_ROW_DOTS || aux16==0
													|| (aux16_2!=0 && aux16_2!=1))
												 CtCommPrint(MSG_INV_DATA);
											else
												{CtRec(posx1,posy1,posx1+aux16_1-1,posy1+aux16-1,color, aux16_2);
												 CtCommPrint(MSG_OK);
												}
											break;
				  case CMD_OP:		if(USARTIfOver())
								  CtCommPrint(MSG_OVER);
								else
								  {strcpy(str,USARTGetStr('\r'));
								   CtClear();
								   CtSelectFont((PGM_P)font_vec[0],BLACK);
								   tok=strtok(str,",");
								   if(tok!=NULL)
								     CtPuts(tok,4,3);
								   tok=strtok(NULL,",");
								   if(tok!=NULL)
								     CtPuts(tok,15,3);
								   //CtUpdate();
								   CtCommPrint(MSG_OK);
								  }
								break;
				  default:					break;
				 }
			 }

		 //bucle de cronometros
		 if(f_cro_pri && f_timer)
		   {char s_cro_pri[7];
		    char s_cro_sec[4];
		    char s_cro_regr[4];

			f_timer=0;
		    if(++ds10==9)//cada un segundo
		      {ds10=0;
		       CtClear();

		       if(f_cro_regr)//regresiva para empezar
		    	   {CtSelectFont((PGM_P)font_vec[1],BLACK);

		    	    itoa(cont_cro_regr,s_cro_regr,10);

		    	    CtPuts(s_cro_regr,POS_REG_X,POS_REG_Y);
		    	    if(cont_cro_regr==0)
		    	    	f_cro_regr=0;
		    	    else
		    	    	cont_cro_regr--;
		    	   }
		       else
		    	   strcpy(s_cro_regr,"0");

		       if(f_cro_regr==0)//calcula y muestra cro primario
		    	   {itoa(cont_cro_pri/60,str,10);
		    	    itoa(cont_cro_pri/60,s_cro_pri,10);

		    	    strcat(str,":");
		    	    strcat(s_cro_pri,",");

		    	    if(cont_cro_pri%60<10 && cont_cro_pri%60>0)
		    	    	strcat(str,"0");

		    	    strcat(str,itoa(cont_cro_pri%60,auxstr,10));
		    	    strcat(s_cro_pri,itoa(cont_cro_pri%60,auxstr,10));

		    	    if(cont_cro_pri%60==0)
		    	    	strcat(str,"0");

				    if(cont_cro_pri!=0)
				    	cont_cro_pri--;

		    	    CtSelectFont((PGM_P)font_vec[1],BLACK);
		    	    CtPuts(str,POS_CRON_X,POS_CRON_Y);
		    	   }
		       else
		    	   strcpy(s_cro_pri,"0,0");

		       if(f_cro_sec && f_cro_pri && cont_cro_pri!=0)//calcula y muestra cro secundario
		    	   {itoa(cont_cro_sec,s_cro_sec,10);

		    	    CtSelectFont((PGM_P)font_vec[0],BLACK);
		    	    CtPuts(s_cro_sec,POS_CRON_X+3,48);

		    	    if(cont_cro_sec!=0)//se mantiene en cero, aunque el primario siga decrementandose.
		    	    	cont_cro_sec--;
		    	   }
		       else
		    	   strcpy(s_cro_sec,"0");

		       //CtUpdate();

		       str[0]=0;
		       strcat(str,s_cro_pri);
		       strcat(str,",");
		       strcat(str,s_cro_sec);
		       strcat(str,",");
		       strcat(str,s_cro_regr);
		       strcat(str,"\r");
		       USARTSendStrAndWait(str);

		      }
		   }

		 //bucle de refresco de pantalla
		 if(cont_timer_bri>0)
			 {volatile uint16_t aux=cont_timer_bri;
			  cont_timer_bri=0;
			  CtDuty(bri_duty , aux*200);
			 }
		}
}

ISR(TIMER1_COMPA_vect)
{if(!f_cro_pause)
	f_timer=1;
}

ISR(TIMER0_COMP_vect)
{cont_timer_bri++; //incrementa cada 0.2 mseg
}
