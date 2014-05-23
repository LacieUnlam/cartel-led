/*
 * cartel_comm.c
 *
 *  Created on: 24/09/2012
 *      Author: Christian
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <ctype.h>

#include "usart.h"

#include "cartel_comm.h"

void CtCommInit()
{USARTInit(19200,USART_ASYNC_MODE,USART_8_B_SIZE,USART_PAR_DISABLE,USART_1_STOP,USART_2X_DISABLE);
 USARTSendStrAndWait_P(PSTR("Programa de control de cartel led. Ver:0 \
							\r? para ayuda\r"));
 USARTStartRx();
}

uint8_t CtCommDecoder()
{static uint8_t waited_char=0;
 static uint8_t waited_command=COMMAND_NONE;
 char aux;

 if(USARTIfOver())
 	{waited_char=0;
	 waited_command=COMMAND_NONE;
	 CtCommPrint(MSG_OVER);
 	}

 switch(waited_char)
	 {case 0:	switch(waited_command)
					{case COMMAND_NONE: switch(USARTGetChar())
											{case '?':	waited_char='\r';
														waited_command=COMMAND_HELP;
														break;
											 case 'u':	waited_char='\r';
														waited_command=COMMAND_UPDATE;
														break;
											 case 'b':  waited_char='\r';
														waited_command=COMMAND_CLEAR;
														break;
											 case 'g':  waited_char='\r';
											 			waited_command=COMMAND_GOTO;
											 			break;
											 case 'c':  waited_char='\r';
											 			waited_command=COMMAND_CRONO;
											 			break;
											 case 's':  waited_char='\r';
											 			waited_command=COMMAND_CRO_SEC;
											 			break;
											 case 'f':  waited_char='\r';
											 			waited_command=COMMAND_FONT_SEL;
											 			break;
											 case 't':  waited_char='\r';
											 			waited_command=COMMAND_TEXT;
											 			break;
											 case 'p':  waited_char='\r';
											 			waited_command=COMMAND_POINT;
											 			break;
											 case 'i':  waited_char='\r';
											 			waited_command=COMMAND_INVERT;
											 			break;
											 case 'l':  waited_char='\r';
											 			waited_command=COMMAND_LINE;
											 			break;
											 case 'r':  waited_char='\r';
											 			waited_command=COMMAND_RECT;
											 			break;
											 case 'o':  waited_char='\r';
											 			waited_command=COMMAND_OP;
											 			break;
											 case '\r': waited_char=0;
														return COMMAND_INVALID;
														break;
											}
										break;
					}
				break;
	  case '\r':  switch(waited_command)
					  {case COMMAND_HELP:  waited_command=COMMAND_NONE;
										   waited_char=0;
										   if(((char)USARTGetChar())=='\r')
											   return COMMAND_HELP;
										   else
											   return COMMAND_INVALID;
										   break;
					   case COMMAND_UPDATE:waited_command=COMMAND_NONE;
										   waited_char=0;
					  					   if(((char)USARTGetChar())=='\r')
					  							return COMMAND_UPDATE;
					  					   else
												return COMMAND_INVALID;
					  					   break;
					   case COMMAND_CLEAR: waited_command=COMMAND_NONE;
										   waited_char=0;
					  					   if(((char)USARTGetChar())=='\r')
					  							return COMMAND_CLEAR;
					  					   else
												return COMMAND_INVALID;
					  					   break;
					   case COMMAND_GOTO:  aux=(char)usart_db[usart_index_end-1];
										   if(aux=='\r')
												{waited_char=0;
												 waited_command=COMMAND_NONE;
												 return COMMAND_GOTO;
												}
										   else
											   if(!isdigit(aux)&&(aux!=',')&&(aux!='-')&&(aux!='+'))
												   {waited_char=0;
													waited_command=COMMAND_NONE;
													return INVALID_DATA;
												   }
										   break;
					   case COMMAND_CRONO:	waited_command=COMMAND_NONE;
								waited_char=0;
								if(((char)USARTGetChar())=='\r')
								    return COMMAND_CRONO;
								else
								    return COMMAND_INVALID;
								break;
					   case COMMAND_CRO_SEC:	waited_command=COMMAND_NONE;
					   	   	   	   	   	   	   	waited_char=0;
					   	   	   	   	   	   	   	if(((char)USARTGetChar())=='\r')
					   	   	   	   	   	   	   		return COMMAND_CRO_SEC;
					   	   	   	   	   	   	   	else
					   	   	   	   	   	   	   		return COMMAND_INVALID;
					   	   	   	   	   	   	   	break;
					   case COMMAND_FONT_SEL: aux=(char)usart_db[usart_index_end-1];
											  if(aux=='\r')
												  {waited_char=0;
												   waited_command=COMMAND_NONE;
												   return COMMAND_FONT_SEL;
												  }
											  break;
					   case COMMAND_TEXT: 	  aux=(char)usart_db[usart_index_end-1];
											  if(aux=='\r')
												  {waited_char=0;
												   waited_command=COMMAND_NONE;
												   return COMMAND_TEXT;
												  }
											  break;
					   case COMMAND_POINT:	  waited_command=COMMAND_NONE;
											  waited_char=0;
											  if(((char)USARTGetChar())=='\r')
												  return COMMAND_POINT;
											  else
												  return COMMAND_INVALID;
											  break;
					   case COMMAND_INVERT:	  waited_command=COMMAND_NONE;
											  waited_char=0;
											  if(((char)USARTGetChar())=='\r')
												  return COMMAND_INVERT;
											  else
												  return COMMAND_INVALID;
											  break;
					   case COMMAND_LINE:	  aux=(char)usart_db[usart_index_end-1];
											  if(aux=='\r')
												  {waited_char=0;
												   waited_command=COMMAND_NONE;
												   return COMMAND_LINE;
												  }
											  break;
					   case COMMAND_RECT:	  aux=(char)usart_db[usart_index_end-1];
											  if(aux=='\r')
												  {waited_char=0;
												   waited_command=COMMAND_NONE;
												   return COMMAND_RECT;
												  }
											  break;
					   case COMMAND_OP: 	  aux=(char)usart_db[usart_index_end-1];
					   			  if(aux=='\r')
					   			    {waited_char=0;
					   			     waited_command=COMMAND_NONE;
					   			     return COMMAND_OP;
					   			    }
					   			  break;
					   default:				  waited_command=COMMAND_NONE;
											  waited_char=0;
											  break;
					  }
				  break;
	  default:	break;
	 }

 return COMMAND_NONE;
}

void CtCommPrint(uint8_t msg)
{switch(msg)
	{case MSG_HELP: 		USARTSendStrAndWait_P(PSTR("Programa de control de cartel led. Ver 1 \
														\r u: actualizar\
														\r b: borrar\
														\r gxxx,yyy: goto. xxx:pos x, yyy:pos y\
														\r c: cronómetro\
														\r fx: fuente. x:1 - 5, pueden o no estar definidas\
														\r tx: texto.\
	                                                                                                        \r ox: Oponentes, separados por coma.\
	                                                                                                        \r s: contador secundario \
														\r p: punto\
														\r i: invertir\
														\r lv-hxxx: linea.  v:vertical, h:horizontal, xxx:longitud\
														\r rxxx,yyy,z: rectángulo.  xxx:ancho, yyy:alto, z:0-vacio 1-lleno\
														\r ?: ayuda"));
							USARTSendStrAndWait_P(PSTR("\r"));
							break;
	 case MSG_INV_COMM: 	USARTSendStrAndWait_P(PSTR("(error) comando no válido\r"));
							break;
	 case MSG_OK: 			USARTSendStrAndWait_P(PSTR("ok\r"));
	 						break;
	 case MSG_INV_DATA: 	USARTSendStrAndWait_P(PSTR("(error) datos no válidos\r"));
	 						break;
	 case MSG_INV_FONT: 	USARTSendStrAndWait_P(PSTR("(error) fuente no disponible\r"));
	 						break;
	 case MSG_OVER: 		USARTSendStrAndWait_P(PSTR("(error) la cadena es demasiado larga\r"));
	 						break;
	 default:				USARTSendStrAndWait_P(PSTR("(error interno) codigo de mensaje comm no válido\r"));
							break;
	}

 USARTStartRx();
}
