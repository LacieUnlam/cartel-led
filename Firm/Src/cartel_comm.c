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
 static uint8_t waited_command=CMD_NONE;
 char aux;

 if(USARTIfOver())
 	{waited_char=0;
	 waited_command=CMD_NONE;
	 CtCommPrint(MSG_OVER);
 	}

 switch(waited_char)
	 {case 0:	switch(waited_command)
					    {case CMD_NONE: switch(USARTGetChar())
							                  {case '?':  waited_char='\r';
							                              waited_command=CMD_HELP;
							                              break;
							                   case 'u':	waited_char='\r';
							                              waited_command=CMD_UPDATE;
							                              break;
							                   case 'b':  waited_char='\r';
							                              waited_command=CMD_CLEAR;
							                              break;
							                   case 'g':  waited_char='\r';
							                              waited_command=CMD_GOTO;
							                              break;
							                   case 'd':  waited_char='\r';
							                              waited_command=CMD_CRO_CONF;
							                              break;
							                   case 'c':  waited_char='\r';
							                              waited_command=CMD_CRONO;
							                              break;
							                   case 's':  waited_char='\r';
							                              waited_command=CMD_CRO_SEC;
							                              break;
							                   case 'e':  waited_char='\r';
							                              waited_command=CMD_CRO_PAUSE;
							                              break;
							                   case 'f':  waited_char='\r';
							                              waited_command=CMD_FONT_SEL;
							                              break;
							                   case 't':  waited_char='\r';
							                              waited_command=CMD_TEXT;
							                              break;
							                   case 'p':  waited_char='\r';
							                              waited_command=CMD_POINT;
							                              break;
							                   case 'i':  waited_char='\r';
							                              waited_command=CMD_INVERT;
							                              break;
							                   case 'l':  waited_char='\r';
							                              waited_command=CMD_LINE;
							                              break;
							                   case 'r':  waited_char='\r';
							                              waited_command=CMD_RECT;
							                              break;
							                   case 'o':  waited_char='\r';
							                              waited_command=CMD_OP;
							                              break;
							                   case '+':  waited_char=0;
							                              return CMD_BRI_UP;
							                              break;
							                   case '-':  waited_char=0;
							                              return CMD_BRI_DW;
							                              break;
							                   case '1':  waited_char='\r';
							                              waited_command=CMD_PRG_MSG1;
							                              break;
							                   case '2':  waited_char='\r';
							                              waited_command=CMD_PRG_MSG2;
							                              break;
							                   case 'm':  waited_char='\r';
							                              waited_command=CMD_PRG_EXE;
							                              break;
							                   case '\r': waited_char=0;
							                              return CMD_INVALID;
							                              break;
							                  }
										          break;
					    }
				    break;
	  case '\r':  switch(waited_command)
					        {case CMD_HELP:  waited_command=CMD_NONE;
										               waited_char=0;
										               if(((char)USARTGetChar())=='\r')
										                 return CMD_HELP;
										               else
										                 return CMD_INVALID;
										               break;
					         case CMD_UPDATE:waited_command=CMD_NONE;
										               waited_char=0;
										               if(((char)USARTGetChar())=='\r')
										                 return CMD_UPDATE;
										               else
										                 return CMD_INVALID;
										               break;
					         case CMD_CLEAR: waited_command=CMD_NONE;
										               waited_char=0;
										               if(((char)USARTGetChar())=='\r')
										                 return CMD_CLEAR;
										               else
										                 return CMD_INVALID;
										               break;
					         case CMD_GOTO:  aux=(char)usart_db[usart_index_end-1];
										               if(aux=='\r')
										                 {waited_char=0;
										                  waited_command=CMD_NONE;
										                  return CMD_GOTO;
										                 }
										               else
										                 if(!isdigit(aux)&&(aux!=',')&&(aux!='-')&&(aux!='+'))
										                   {waited_char=0;
										                    waited_command=CMD_NONE;
										                    return INVALID_DATA;
										                   }
										               break;
					         case CMD_CRO_CONF:  aux=(char)usart_db[usart_index_end-1];
										                     if(aux=='\r')
										                       {waited_char=0;
										                        waited_command=CMD_NONE;
										                        return CMD_CRO_CONF;
										                       }
										                     else
										                       if(!isdigit(aux)&&(aux!=','))
										                         {waited_char=0;
										                          waited_command=CMD_NONE;
										                          return INVALID_DATA;
										                         }
										                     break;
					         case CMD_CRONO:	waited_command=CMD_NONE;
										                waited_char=0;
										                if(((char)USARTGetChar())=='\r')
										                  return CMD_CRONO;
										                else
										                  return CMD_INVALID;
										                break;
					         case CMD_CRO_SEC:	waited_command=CMD_NONE;
					   	   	   	   	   	   	   	waited_char=0;
					   	   	   	   	   	   	   	if(((char)USARTGetChar())=='\r')
					   	   	   	   	   	   	   		return CMD_CRO_SEC;
					   	   	   	   	   	   	   	else
					   	   	   	   	   	   	   		return CMD_INVALID;
					   	   	   	   	   	   	   	break;
					         case CMD_CRO_PAUSE:	waited_command=CMD_NONE;
					                              waited_char=0;
					                              if(((char)USARTGetChar())=='\r')
					                                return CMD_CRO_PAUSE;
					                              else
					                                return CMD_INVALID;
					                              break;
					         case CMD_FONT_SEL: aux=(char)usart_db[usart_index_end-1];
											                if(aux=='\r')
											                  {waited_char=0;
											                   waited_command=CMD_NONE;
											                   return CMD_FONT_SEL;
											                  }
											                break;
					         case CMD_TEXT: 	  aux=(char)usart_db[usart_index_end-1];
					                            if(aux=='\r')
					                              {waited_char=0;
					                               waited_command=CMD_NONE;
					                               return CMD_TEXT;
					                              }
					                            break;
					         case CMD_POINT:  waited_command=CMD_NONE;
					                          waited_char=0;
					                          if(((char)USARTGetChar())=='\r')
					                            return CMD_POINT;
					                          else
					                            return CMD_INVALID;
					                          break;
					         case CMD_INVERT:	  waited_command=CMD_NONE;
											                waited_char=0;
											                if(((char)USARTGetChar())=='\r')
											                  return CMD_INVERT;
											                else
											                  return CMD_INVALID;
											                break;
					         case CMD_LINE:	  aux=(char)usart_db[usart_index_end-1];
											              if(aux=='\r')
											                {waited_char=0;
											                 waited_command=CMD_NONE;
											                 return CMD_LINE;
											                }
											              break;
					         case CMD_RECT:	  aux=(char)usart_db[usart_index_end-1];
					                          if(aux=='\r')
					                            {waited_char=0;
					                             waited_command=CMD_NONE;
					                             return CMD_RECT;
					                            }
					                          break;
					         case CMD_OP: 	  aux=(char)usart_db[usart_index_end-1];
					                          if(aux=='\r')
					                            {waited_char=0;
					                             waited_command=CMD_NONE;
					                             return CMD_OP;
					                            }
					                          break;
					         case CMD_PRG_MSG1: aux=(char)usart_db[usart_index_end-1];
					                            if(aux=='\r')
					                              {waited_char=0;
					                               waited_command=CMD_NONE;
					                               return CMD_PRG_MSG1;
					                              }
					                            break;
					         case CMD_PRG_MSG2: aux=(char)usart_db[usart_index_end-1];
					                            if(aux=='\r')
					                              {waited_char=0;
					                               waited_command=CMD_NONE;
					                               return CMD_PRG_MSG2;
					                              }
					                            break;
					         case CMD_PRG_EXE:  waited_command=CMD_NONE;
					                            waited_char=0;
					                            if(((char)USARTGetChar())=='\r')
					                              return CMD_PRG_EXE;
					                            else
					                              return CMD_INVALID;
					                            break;
					         default:			waited_command=CMD_NONE;
										            waited_char=0;
										            break;
					        }
				        break;
	   default:	break;
	  }

 return CMD_NONE;
}

void CtCommPrint(uint8_t msg)
{switch(msg)
	{case MSG_HELP: 		USARTSendStrAndWait_P(PSTR("Programa de control de cartel led. Ver 1 \
														\r u: actualizar\
														\r b: borrar\
														\r gxxx,yyy: goto. xxx:pos x, yyy:pos y\
                            \r dmm,ss,ccc,s: config.  MinCP,SegCP,SegCS,SegRegr\
														\r c: cronómetro (5seg + 3 min)\
														\r s: cronómetro secundario ascendente\
														\r e: pausar cronómetros (principal y secundario)\
														\r fx: fuente. x:1 - 5, pueden o no estar definidas\
														\r tx: texto.\
	                          \r ox: Oponentes, separados por coma.\
														\r p: punto\
														\r i: invertir\
														\r lv-hxxx: linea.  v:vertical, h:horizontal, xxx:longitud\
														\r rxxx,yyy,z: rectángulo.  xxx:ancho, yyy:alto, z:0-vacio 1-lleno\
														\r +: aumenta brillo\
														\r -: disminuye brillo\
	                          \r 1x: texto en eeprom 1.\
	                          \r 2x: texto en eeprom 2.\
                            \r m: reproducir\\detener texto en eeprom\
														\r ?: ayuda"));
							        USARTSendStrAndWait_P(PSTR("\r"));
							        break;
	 case MSG_INV_COMM: 	USARTSendStrAndWait_P(PSTR("(error) comando no válido\r")); break;
	 case MSG_OK: 			USARTSendStrAndWait_P(PSTR("ok\r")); break;
	 case MSG_INV_DATA: 	USARTSendStrAndWait_P(PSTR("(error) datos no válidos\r")); break;
	 case MSG_INV_FONT: 	USARTSendStrAndWait_P(PSTR("(error) fuente no disponible\r")); break;
	 case MSG_OVER: 		USARTSendStrAndWait_P(PSTR("(error) la cadena es demasiado larga\r")); break;
	 default:				USARTSendStrAndWait_P(PSTR("(error interno) codigo de mensaje comm no válido\r")); break;
	}

 USARTStartRx();
}
