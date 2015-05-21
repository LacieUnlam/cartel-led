/*******************************************************************
************									********************
************	Rutinas para manejo del USART	********************
************									********************
*******************************************************************/
//
//Estas rutinas controlan el puerto USART haciendo uso de interrupciones.
//Las rutinas de interrupcion intercambian datos entre el registro UDR del
//modulo USART y el buffer usart_db.  Este ultimo contendra los datos
//a enviar o almacenara los recibidos durante una transmision o recepcion
//respectivamente.
//
//A continuacion se enuncian las funciones disponibles para uso del USART.
//La informacion especifica de cada funcion se encuentra disponible en las
//declaraciones respectivas de cada una en este archivo.
//
//	Funciones de inicializacion y configuracion:
//			USARTInit()					//Inicializa al USART
//			USARTSetBaudRate()			//Setea la velocidad de transmicion
//										//a velocidades no predefinidas
//	Funciones para transmicion:
//			USARTIndexersClr()		//Inicializa al USART para la
//										//posterior carga de datos en
//										//usart_db antes de iniciarse
//										//una transmicion.
//			USARTStartTx()				//Inicia la transmicion de los datos
//										//cargados en usart_db.
//			USARTAddByteToDB()	//Carga un byte en el buffer
//			USARTAddWordToDB()	//Carga un word en el buffer
//			USARTAddCharToDB()	//Carga un caracter en el buffer
//			USARTAddStrToDB()//Carga una cadena en el buffer
//			USARTSendStrAndWait() //Agrega un string a usart_db, 
//										  //inicia la transmision y espera a que finalice
//	Funciones para recepcion:
//			USARTStartDataReceive()		//Prepara al USART para una recepcion
//										//de datos numericos
//			USARTStartStrRxUntilChar()	//Prepara al USART para una recepcion
//											//de caracteres
//			USARTGetByteFromDB()//Devuelve un byte de los datos recibidos
//			USARTGetWordFromDB()//Devuelve un word de los datos recibidos
//			USARTGetStrFromDB()	//Devuelve una cadena de los datos recibidos
//			USARTRequestStrAndWait()//Espera una cadena y retorna cuando
//											//la cadena es recibida

#ifndef USART_H
#define USART_H

//para las funciones inline que incluyen funciones de la librería.
#ifndef _AVR_INTERRUPT_H_
#include <avr/interrupt.h>
#endif

#ifndef F_CPU
#define F_CPU 16000000
#endif

//******Variables globales
#define	USART_DB_LENGTH	100	//Longitud del buffer de datos
volatile uint8_t usart_db[USART_DB_LENGTH];	//buffer de datos
volatile uint8_t usart_index_init	//Puntero al buffer de datos
				,usart_index_end		//Cantidad de datos del buffer
				,usart_state		//Variables donde se especifica el tipo de operacion
				,usart_rx_over;		//indica si hubo overflow en el DB.

//----valores posibles de usart_state
#define USART_STATE_NONE		0	//No se esta realizando ninguna operacion
#define	USART_STATE_TX			1	//Se transmiten datos numericos

//*********funciones de configuracion
//----Modos de operacion
#define USART_SYNC_MODE		1
#define USART_ASYNC_MODE	0
//----Longitud de la palabra de datos
#define USART_B_SIZE_MASK	0b00000110 //para UCSRC
#define USART_5_B_SIZE		0
#define USART_6_B_SIZE		0b00000010
#define USART_7_B_SIZE		0b00000100
#define USART_8_B_SIZE		0b00000110
#define USART_9_B_SIZE		0b00000111//este valor no se corresponde
										  //con la mascara. Simplemente una identificacion
//----Paridad
#define USART_PAR_MASK		0b00110000
#define USART_PAR_DISABLE	0b00000000
#define USART_PAR_EVEN		0b00100000	//Par
#define USART_PAR_ODD		0b00110000	//Impar
//----Bits de Stop
#define	USART_1_STOP		0
#define	USART_2_STOP		1
//----Doblador de velocidad
#define	USART_2X_ENABLE		1
#define	USART_2X_DISABLE	0


/**********************************************************
 * 			funciones de inicializacion
 **********************************************************/

/*Inicializa al puerto serie
*	Parametros:	baud_rate		//Velocidad de transmision, en baudios
*				mode:			USART_SYNC_MODE		//Sincronico
*								USART_ASYNC_MODE	//Asincronico
*				data_length:	USART_5_B_SIZE
*								USART_6_B_SIZE
*								USART_7_B_SIZE
*								USART_8_B_SIZE
*								USART_9_B_SIZE
*				parity:			USART_PAR_DISABLE	//sin paridad
*								USART_PAR_EVEN		//Par
*								USART_PAR_ODD		//Impar
*				stop_bits:		USART_1_STOP
*								USART_2_STOP
*				double_speed:	USART_2X_ENABLE
*								USART_2X_DISABLE
*Nota: Esta funcion habilita tanto receptor como transmisor,como asi tambien el uso de interrupciones por RX,TX y UDR vacio.
*La velocidad de transmision es seteada como lo es especificada, independientemente de que se haya optado o no por el doblador
*de velocidad.
*/
inline void USARTInit(uint32_t baud_rate,uint8_t mode,uint8_t data_length,uint8_t parity
				,uint8_t stop_bits,uint8_t double_speed){
 volatile uint8_t UCSRC_reg=0b10000000;
 uint16_t baud_div;
 cli();
 if(mode==USART_SYNC_MODE)	//modo de operacion
 	{UCSRC_reg|=_BV(UMSEL);
	 baud_div=(uint16_t)(F_CPU/(2*baud_rate))-1;
	}
 else	//modo asincronico
 	{if(double_speed==USART_2X_ENABLE)	//Doblador de velocidad
 		{UCSRA|=_BV(U2X);
		 baud_div=(uint16_t)(F_CPU/(8*baud_rate))-1;
		}
 	 else	//sin el doblador de velocidad
 		{UCSRA&=~_BV(U2X);
		 baud_div=(uint16_t)(F_CPU/(16*baud_rate))-1;
		}
	}
 UBRRL=(uint8_t)baud_div;
 UBRRH=(uint8_t)(baud_div>>8);
 if(data_length!=USART_9_B_SIZE)	//DataSize
 	{UCSRC_reg|=data_length;
	 UCSRB&=~_BV(UCSZ2);
	}
 else
 	{UCSRC_reg|=USART_8_B_SIZE;
	 UCSRB|=_BV(UCSZ2);
	}
 UCSRC_reg|=parity; //paridad
 if(stop_bits==USART_2_STOP)	//Bits de stop
 	UCSRC_reg|=_BV(USBS);

 UCSRC=UCSRC_reg;
 UCSRB|=_BV(RXEN)|_BV(TXEN)|_BV(RXCIE)|_BV(TXCIE);	//Habilita transmision y recepcion
													// e interrupciones de RX,TX
 usart_index_init=0;
 usart_index_end=0;
 usart_state=USART_STATE_NONE;
 sei();
};

//	Permite setear la velocidad de transferencia a valores no predefinidos
//	Parametros:			baud_rate:	(en bits por segundo)
inline void USARTSetBaudRate(uint32_t baud_rate)
{uint16_t baud_div=(F_CPU/(16*baud_rate))-1;
 UBRRH=(baud_div>>8)&0x000F;
 UBRRL=baud_div&0x00FF;
};


/**********************************************************
 * 			funciones sobre el DB
 **********************************************************/

/* Inicializa a 0 todos los indices */
void USARTIndexersClr();

/* Indica si el DB está lleno, ya sea de dados ingresados para ser transmitidos, o de datos recibidos */
uint8_t USARTIfDBFull();


/**********************************************************
 * 			funciones para transmision
 **********************************************************/

//*****Funciones para inicio de transmision

/* Inicia una transmision del contenido del DB */
void USARTStartTx();

/* Inicia una transmision del contenido de usart_db y espera a que la transferencia finalice */
void USARTStartTxAndWait();

//*****Funciones para carga secuencial del DB

/* Agrega un byte a usart_db.  Autoincrementa usart_index_end luego de la carga
 * Parametros:				data	//Byte a agregar
 */
void USARTAddByteToDB(uint8_t data);

/* Agrega un word a usart_db.  Autoincrementa usart_index_end luego de la carga
 * Parametros:				data	//word a agregar
 */
void USARTAddWordToDB(uint16_t data);

/* Agrega un caracter a usart_db.  Autoincrementa usart_index_end luego de la carga
 * Parametros:				c	//Caracter a agregar
 */
void USARTAddCharToDB(char c);

/* Agrega un string a usart_db.  La cadena debe poseer el caracter de fin de linea.
 * La función posee dos versiones, con y sin uso de las librerías progmem.
 * El caracter de fin de linea no será transmitido.
 * Parametros:				*str	//Puntero al string a agregar
 */
void USARTAddStrToDB(char* str);
void USARTAddStrToDB_P(char* str);

/* Envía una cadena por el puerto.  La función retorna luego de la carga del DB, dejando a las rutinas de interrupcion
 * del puerto el completar el envio.  Sin embargo, si la cadena es mayor que la que acepta el DB, la funcion hará varios envíos
 * consecutivos, quedando en espera de la finalización de cada una de ellas.
 * La función posee dos versiones, con y sin uso de las librerías progmem.
 * El caracter de fin de linea no será transmitido.
 * Parametros:				*str	//puntero a la cadena
 */
uint8_t USARTSendStr_P(char* str);
uint8_t USARTSendStr(char* str);

/* Agrega un string a usart_db, inicia la transmision y espera a que finalice.  La cadena debe poseer el caracter de fin de linea.
 * El caracter de fin de linea no será transmitido.
 * La función posee dos versiones, con y sin uso de las librerías progmem.
 * Parametros:				*str	//Puntero al string a enviar
 */
void USARTSendStrAndWait(char *str);
void USARTSendStrAndWait_P(char *str);


/**********************************************************
 * 			funciones para recepcion
 **********************************************************/

//*****Inicializa para una recepcion

/* Prepara al los recursos para una recepcion de datos*/
void USARTStartRx();


//*****Informacion sobre la recepcion

/* Informa la cantidad de bytes recibidos, aun sin ser leidos. */
uint8_t USARTRxCount();

/* Informa si hubo overflow del DB durante la recepción de datos*/
uint8_t USARTIfOver();

//*****Recuperacion de los datos recibidos

/*	Devuelve el char al que apunta usart_index_init.
 *	usart_index_init sera incrementado en 1 luego de la lectura.
 *	Si luego de la lectura no restan datos, los indices son vueltos a 0.
 */
char USARTGetChar();

/*	Devuelve el byte al que apunta usart_index_init.
 *	usart_index_init sera incrementado en 1 luego de la lectura
 *	Si luego de la lectura no restan datos, los indices son vueltos a 0.
 */
uint8_t USARTGetByte();

/*	Devuelve el word al que apunta usart_index_init.
 *	usart_index_init sera incrementado en 2 luego de la lectura.
 *	Si luego de la lectura no restan datos, los indices son vueltos a 0.
 */
uint16_t USARTGetWord();

/*	Devuelve un puntero a cadena, a usart_db, desde la posicion en la que se encuentre usart_index_init.
 *	La cadena recibida por el puerto deberá contener el caracter que indique el fin de linea, especificado
 *	por end_ch.  La posicion donde se encuentra end_ch, será reemplazada por 0, caracter de fin de linea.
 *	Si el caracter de fin de linea no es encontrado, la función retornará 0, y los indices serán reseteados.
 *	Luego de la lectura, end_index_init apuntará a la posicion siguiente al caracter de fin de linea.
 *	Si luego de la lectura no restan datos, los indices son vueltos a 0.
 */
char* USARTGetStr(char end_ch);

#endif
