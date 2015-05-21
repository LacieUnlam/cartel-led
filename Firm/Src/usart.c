
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "USART.h"


/**********************************************************
 * 			funciones sobre el DB
 **********************************************************/

void USARTIndexersClr()
{usart_index_init=0;
 usart_index_end=0;
}

uint8_t USARTIfDBFull()
{if(usart_index_end<USART_DB_LENGTH)
	return 0;
 else
	return 1;
}


/**********************************************************
 * 			funciones para transmision
 **********************************************************/

//*****Funciones para inicio de transmision

void USARTStartTx()
{usart_state=USART_STATE_TX;
 UDR=usart_db[usart_index_init];
 UCSRB|=_BV(UDRIE);
}

void USARTStartTxAndWait()
{USARTStartTx();
 while(usart_state!=USART_STATE_NONE);
}


//*****Funciones para carga secuencial del DB

void USARTAddByteToDB(uint8_t data)
{usart_db[usart_index_end++]=data;
}

void USARTAddWordToDB(uint16_t data)
{USARTAddByteToDB((uint8_t)(data&0x00FF));
 USARTAddByteToDB((uint8_t)((data>>8)&0x00FF));
}

void USARTAddCharToDB(char c)
{usart_db[usart_index_end++]=c;
}

void USARTAddStrToDB(char* str)
{uint8_t i=0;
 while(str[i]&&(!USARTIfDBFull()))
	usart_db[usart_index_end++]=(uint8_t) str[i++];
}

void USARTAddStrToDB_P(char* str)
{char c;

 while((c=pgm_read_byte(str))&&(!USARTIfDBFull()))
	{usart_db[usart_index_end++]=(uint8_t) c;
	 str++;
	}
}


//*****Funciones para transmisión de cadenas

uint8_t USARTSendStr(char* str)
{uint8_t wait_flag=0; //indica si hubo o no que realizar un envio con espera de retorno por exceso en la cadena.

 while(usart_state!=USART_STATE_NONE); //espera que termine cualquier operación en curso

 USARTIndexersClr();
 while(*str!=0)
	{if(!USARTIfDBFull())
		  {usart_db[usart_index_end++]=(uint8_t) *str;
		   str++;
		  }
	 else
		 {USARTStartTxAndWait();
		  USARTIndexersClr();
		  wait_flag=1;
		 }
	}
 if(usart_index_end)//si queda datos en db, son enviados
	USARTStartTx();

 return wait_flag;
}

uint8_t USARTSendStr_P(char* str)
{uint8_t wait_flag=0; //indica si hubo o no que realizar un envio con espera de retorno por exceso en la cadena.
 char c;

 while(usart_state!=USART_STATE_NONE); //espera que termine cualquier operación en curso

 USARTIndexersClr();
 while((c=pgm_read_byte(str)))
	{if(!USARTIfDBFull())
		  {usart_db[usart_index_end++]=(uint8_t) c;
		   str++;
		  }
	 else
		 {USARTStartTxAndWait();
		  USARTIndexersClr();
		  wait_flag=1;
		 }
	}
 if(usart_index_end)//si queda datos en db, son enviados
	USARTStartTx();

 return wait_flag;
}

void USARTSendStrAndWait(char *str)
{while(usart_state!=USART_STATE_NONE); //espera que termine cualquier operación en curso

 USARTIndexersClr();
 while(*str!=0)
	{if(!USARTIfDBFull())
		  {usart_db[usart_index_end++]=(uint8_t) *str;
		   str++;
		  }
	 else
		 {USARTStartTxAndWait();
		  USARTIndexersClr();
		 }
	}
 if(usart_index_end)//si queda datos en db, son enviados
	USARTStartTxAndWait();
}

void USARTSendStrAndWait_P(char *str)
{char c;

 while(usart_state!=USART_STATE_NONE); //espera que termine cualquier operación en curso

 USARTIndexersClr();
 while((c=pgm_read_byte(str)))
	{if(!USARTIfDBFull())
		  {usart_db[usart_index_end++]=(uint8_t) c;
		   str++;
		  }
	 else
		 {USARTStartTxAndWait();
		  USARTIndexersClr();
		 }
	}
 if(usart_index_end)//si queda datos en db, son enviados
	USARTStartTxAndWait();
}



/**********************************************************
 * 			funciones para recepcion
 **********************************************************/

//*****Inicializa para una recepcion

void USARTStartRx()
{while(usart_state!=USART_STATE_NONE); //espera que termine cualquier operación en curso

 USARTIndexersClr();
 usart_rx_over=0;
}

//*****Informacion sobre la recepcion

uint8_t USARTRxCount()
{return (usart_index_end-usart_index_init);
}

uint8_t USARTIfOver()
{uint8_t j=usart_rx_over;
 usart_rx_over=0;
 return j;
}


//*****Recuperacion de los datos recibidos

char USARTGetChar()
{char j=(char) usart_db[usart_index_init++];

 if(usart_index_init==usart_index_end)
	 USARTIndexersClr();

 return j;
}

uint8_t USARTGetByte()
{uint8_t j=usart_db[usart_index_init++];

 if(usart_index_init==usart_index_end)
	 USARTIndexersClr();

 return j;
}

uint16_t USARTGetWord()
{uint16_t aux=0;
 aux|=usart_db[usart_index_init++];
 aux|=((usart_db[usart_index_init++]<<8)&0xFF00);

 if(usart_index_init==usart_index_end)
	 USARTIndexersClr();

 return aux;
}

char* USARTGetStr(char end_ch)
{char *str =(char*) (usart_db+usart_index_init);

 while((char)(*(usart_db+usart_index_init++))!=end_ch)//busca el caracter que indique el fin de linea
	 if(usart_index_init==usart_index_end)//llego al final de los datos recibidos sin encontrar el caracter de fin de linea
		 {USARTIndexersClr();
		  return 0;
		 }

 *(usart_db+usart_index_init-1)=0;//reemplaza el caracter que indique el fin de linea por 0.

 if(usart_index_init==usart_index_end)
 	 USARTIndexersClr();

 return str;
}


/**********************************************************
 * 			Rutinas de interrupcion
 **********************************************************/

ISR(USART_UDRE_vect)
{
 if((++usart_index_init)<usart_index_end)
	UDR=usart_db[usart_index_init];
 else
	UCSRB&=~_BV(UDRIE);
}

ISR(USART_TXC_vect)
{if(usart_index_init==usart_index_end)
	usart_state=USART_STATE_NONE;
}

ISR(USART_RXC_vect)
{usart_db[usart_index_end++]=UDR;
 if(USARTIfDBFull())
	 {usart_index_end=0;
	  usart_rx_over=1;
	 }
}
//**************************************************
