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

#define POS_REG_X	4
#define POS_REG_Y	27
#define POS_CRON_X	4
#define POS_CRON_Y	14

uint8_t f_timer=0;
uint8_t f_crono_pause=0; //actua sobre ambos cronos

int main(void)
{	int16_t posx1=1,posy1=1,aux16,aux16_1=1,aux16_2=1;
	char str[100],auxstr[20];
	char* tok;
	uint8_t color=BLACK;
	uint8_t *font_vec[5];
	uint16_t crono=0;
	uint16_t crono_sec=0;
	uint8_t f_crono=0;
	uint8_t f_crono_sec=0;
	uint8_t ds10=0;

	CtInit();
	CtCommInit();

	sei();

	//timer 1
	OCR1A=25000; //tics para 0.1 seg, a Xtal de 16M
	TCCR1A=0b00000000;
	TIMSK=0b00010000;
	TCCR1B=0b00001011; //CTC, prescaler a 64, timer arrancado

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
				  case CMD_UPDATE:	CtUpdate();
										CtCommPrint(MSG_OK);
				  				  		break;
				  case CMD_CLEAR:	CtClear();
				  						CtCommPrint(MSG_OK);
				  				  		break;
				  case CMD_CRONO:	if(!f_crono)
							  	  	  {crono=184; //3 minutos y 5 segundos
							  	  	   f_timer=0;
							  	  	   f_crono=1;
							  	  	   f_crono_sec=0;
							  	  	   f_crono_pause=0;
							  	  	   ds10=0;
							  	  	   CtSelectFont((PGM_P)font_vec[1],BLACK);
							  	  	   CtClear();
							  	  	   CtPuts("5",POS_REG_X,POS_REG_Y);
							  	  	   CtUpdate();
							  	  	  }
									else
									  {f_crono=0;
									   f_crono_sec=0;
									  }

				  					CtCommPrint(MSG_OK);
				  					break;
				  case CMD_CRO_SEC:	  if(f_crono_sec)
				  	  	  	  	  	  	  f_crono_sec=0;
				  	  	  	  	  	  else
				  	  	  	  	  	  	  {f_crono_sec=1;
				  	  	  	  	  	  	   crono_sec=0;
				  	  	  	  	  	  	  }
				  	  	  	  	  	  CtCommPrint(MSG_OK);
					  	  	  	  	  break;
				  case CMD_CRO_PAUSE: if(f_crono_pause)
					  	  	  	  	  	  f_crono_pause=0;
				  	  	  	  	  	  else
				  	  	  	  	  		  f_crono_pause=1;
				  	  	  	  	  	  CtCommPrint(MSG_OK);
					  	  	  	  	  break;
				  case CMD_GOTO:	aux16_1=atoi(USARTGetStr(','));
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
								     CtPuts(tok,1,2);
								   tok=strtok(NULL,",");
								   if(tok!=NULL)
								     CtPuts(tok,9,2);
								   CtUpdate();
								   CtCommPrint(MSG_OK);
								  }
								break;
				  default:					break;
				 }
			 }

		 //función de cronometro
		 if(f_crono && f_timer)
		   {char s_cro_serial[12];
			f_timer=0;
		    if(++ds10==9)//cada un segundo
		      {ds10=0;
		       CtClear();
		       if(crono>180)
		    	   //regresiva para empezar
		    	   {CtPuts(itoa(crono-180,str,10),POS_REG_X,POS_REG_Y);
		    	    strcpy(s_cro_serial,str);//para enviar a la pc cuenta de 5 seg.
		    	   }
		       else
		    	   {//calcula y muestra cro primario
		    	    itoa(crono/60,str,10);
		    	    strcat(str,":");
		    	    if(crono%60<10 && crono%60>0)
		    	    	strcat(str,"0");
		    	    strcat(str,itoa(crono%60,auxstr,10));
		    	    if(crono%60==0)
		    	    	strcat(str,"0");

		    	    CtSelectFont((PGM_P)font_vec[1],BLACK);
		    	    CtPuts(str,POS_CRON_X,POS_CRON_Y);

		    	    strcpy(s_cro_serial,str);//para enviar a la pc el cronometro.

		    	    //calcula y muestra cro secundario
		    	    if(f_crono_sec)
		    	    	{itoa(crono_sec++,str,10);
		    	    	 CtSelectFont((PGM_P)font_vec[0],BLACK);
		    	    	 CtPuts(str,POS_CRON_X+3,48);

		    	    	 strcat(s_cro_serial,",");
		    	    	 strcat(s_cro_serial,str);//agrega el contador sec. para enviar a la pc.
		    	    	}
		    	   }
		       CtUpdate();

		       USARTSendStrAndWait(s_cro_serial);

		       if(crono!=0)
		    	   crono--;
		       else
		    	   f_crono=0;
		      }
		   }
		}
}

ISR(TIMER1_COMPA_vect)
{if(!f_crono_pause)
	f_timer=1;
}
