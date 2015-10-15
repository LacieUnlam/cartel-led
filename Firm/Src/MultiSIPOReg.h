
/****************************************************************************************************************************
		Funciones para manejo múltiple de registros de desplazamiento del tipo SIPO.
		Estas funciones permiten manejar 6 salidas de datos para 6 líneas de registros, compartiendo mutuamente las
		Líneas de clock y latch.
		Estas funciones están basadas en el uso de los registros de desplazamiento comerciales 74595 (SIPO)
		
*****************************************************************************************************************************/

#ifndef F_CPU
#warning Se esta usando una definicion por defecto de F_CPU=16000000
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <util/delay.h>

#ifndef ENABLE
#define ENABLE	1
#define DISABLE	0
#endif

#ifndef MULTI_SIPO_REG_H
#define MULTI_SIPO_REG_H

#define MSIPO_USE_DELAY	0	//Usar delay
//#define MSIPO_USE_MR	0	//Usar master reset
//#define MSIPO_USE_OE	0	//Usar open enable

#define MSIPO_SHIFT_DELAY	2	//48 Máximo, para xtal de 16mhz

//Declaración de puerto y pines asociados al reg SIPO
#define	MSIPO_PORT		PORTC	//Registro de datos del puerto
#define	MSIPO_DDR		DDRC	//Registro de direcciones del puerto
#define	MSIPO_PIN		PINC	//Data input register
#define	MSIPO_MR_PIN	0		//Pin de reset maestro del registro de desplazamiento
#define	MSIPO_SER1_PIN	0		//Pin de salida serial 1
#define	MSIPO_SER2_PIN	1		//Pin de salida serial 2
#define	MSIPO_SER3_PIN	2		//Pin de salida serial 3
#define	MSIPO_SER4_PIN	3		//Pin de salida serial 4
#define	MSIPO_SER5_PIN	4		//Pin de salida serial 5
#define	MSIPO_SER6_PIN	5		//Pin de salida serial 6
#define	MSIPO_SC_PIN	7		//(Shift Clock) Pin de clock para el reg de desplazamiento
#define	MSIPO_LC_PIN	6		//(Latch Clock) Pin de carga paralela del registro de desplazamiento al latch de salida.
#define	MSIPO_OE_PIN	0		//(Output Enable) Pin de habilitación de las salidas paralelas.


/**************************************************************************
		Funciones para control de las líneas de un registro SIPO del
		tipo HC595.
**************************************************************************/

//	Genera un pulso de clock para el corrimiento del registro de desplazamiento
extern inline void MSIPOShiftClockPulse(){
	MSIPO_PORT&=~_BV(MSIPO_SC_PIN);
#ifdef	MSIPO_USE_DELAY
	_delay_us (MSIPO_SHIFT_DELAY);
#endif
	MSIPO_PORT|=_BV(MSIPO_SC_PIN);
#ifdef	MSIPO_USE_DELAY
	_delay_us (MSIPO_SHIFT_DELAY);
#endif
};

//	Realiza un pulso para el reseteo del registro de desplazamiento
extern inline void MSIPOMasterReset(){
	MSIPO_PORT&=~_BV(MSIPO_MR_PIN);
#ifdef	MSIPO_USE_DELAY
	_delay_us (MSIPO_SHIFT_DELAY);
#endif
	MSIPO_PORT|=_BV(MSIPO_MR_PIN);
#ifdef	MSIPO_USE_DELAY
	_delay_us (MSIPO_SHIFT_DELAY);
#endif
};

//	Realiza un pulso que almacena en el latch de salida el contenido del registro de desplazamiento
extern inline void MSIPOLatchLoad(){
	MSIPO_PORT&=~_BV(MSIPO_LC_PIN);
#ifdef	MSIPO_USE_DELAY
	_delay_us (MSIPO_SHIFT_DELAY);
#endif
	MSIPO_PORT|=_BV(MSIPO_LC_PIN);
#ifdef	MSIPO_USE_DELAY
	_delay_us (MSIPO_SHIFT_DELAY);
#endif
};

//	Habilita las salidas paralelas de los latchs.
//	Parametros:		State:	ENABLE	//Salidas habilitadas
//							DISABLE	//Salidas en alta impedancia
extern inline void MSIPOEnableOutputs(uint8_t State){
	if(State==ENABLE)
		MSIPO_PORT&=~_BV(MSIPO_OE_PIN);
	else
		MSIPO_PORT|=_BV(MSIPO_OE_PIN);
};

/**************************************************************************
		Funciones para inicialización de un registro SIPO.
**************************************************************************/

//	Inicializa los pines del puerto que manejarán al shift register.
//	A su vez, también habilita las salidas del registro de desplazamiento.
extern inline void MSIPOInit(){
	MSIPO_DDR|=_BV(MSIPO_SER1_PIN)|_BV(MSIPO_SER2_PIN)|_BV(MSIPO_SER3_PIN)|_BV(MSIPO_SER4_PIN)|_BV(MSIPO_SER5_PIN)|_BV(MSIPO_SER6_PIN)|_BV(MSIPO_SC_PIN)|_BV(MSIPO_LC_PIN);
#ifdef MSIPO_USE_MR
	MSIPO_DDR|=_BV(MSIPO_MR_PIN);
#endif
#ifdef MSIPO_USE_OE
	MSIPO_DDR|=_BV(MSIPO_OE_PIN);
#endif
	MSIPO_PORT|=_BV(MSIPO_SC_PIN)|_BV(MSIPO_LC_PIN);
#ifdef MSIPO_USE_MR
	MSIPO_PORT|=_BV(MSIPO_MR_PIN);
#endif
#ifdef MSIPO_USE_OE
	MSIPOEnableOutputs(ENABLE);  //salidas habilitadas.
#endif
};

/************************************************************************************************************************************************
												FUNCIONES DE ESCRITURA EN LOS REGISTROS SIPO
	Las funciones a continuación realizan la serialización de los datos hacia los registros SIPO.
	Estas funciones se agrupan en dos tipos:
		-Por un lado las que poseen el termino "Add" en su nombre.  Estas funciones realizan la serialización de los datos a los registros pero
		 no resetean previamente a los SIPOs ni presentan los datos en sus salidas.  Estas funciones permiten la carga de los SIPOs de a partes, 
		 en sucesivas llamadas a funciones de carga.  Será necesario realizar una llamada a SIPOMasterReset() antes del fragmento de código
		 que realiza la serialización y posteriormente a SIPOLatchLoad() cuando los SIPOs esten listos para presentar su carga a las salidas.
		-Por otro lado, esta el juego de funciones que poseen el término "Write" en sus nombres.  Estas funciones realizan por si mismas todo el
		 procedimiento desde el reseteo de los SIPOs, la serialización de las datos a los mismos y la posterior presentación de esa carga en las
		 salidas de los registros.  Estas funciones no requieren hacer llamadas previas y posteriores a funciones de reseteo de los SIPOs 
		 ni a funciones de presentación de la carga en las salidas.  No deben ser usadas si se debe realizar una carga por partes de los registros.

*************************************************************************************************************************************************/


/***************************************************************************************
				Funciones RECURSIVAS a registros SIPO

Estas funciones poseen el termino "Add" en su nombre.  Estas funciones
		realizan la serialización de los datos a los registros pero no resetean
		previamente a los SIPOs ni presentan los datos en sus salidas.  Estas
		funciones permiten la carga de los SIPO de a partes, en sucesivas llamadas a
		funciones de carga.  Será necesario realizar una llamada a SIPOMasterReset()
		antes del fragmento de código que realiza la serialización y posteriormente a
		SIPOLatchLoad() cuando los SIPOs esten listos para presentar su carga a las
		salidas.

****************************************************************************************/

/*Realiza la transmición serial de los datos referenciados por el puntero Buffer.
	La función permite la escritura recursiva a los SIPOs. La serialisazión se realiza desde los bits de menor peso en direcciones
	bajas del buffer hasta los bits de mayor peso en direcciones altas del buffer:

                              /------Buffer--------\

				byte 0 (LSB)   <-|7|6| <-- |1|0|
                              |__________>_________
                                                   |
				byte 1         <-|7|6| <-- |1|0|<__|
						      |__________>_________
                                                   |
				byte 2 (MSB)     |7|6| <-- |1|0|<__|

	Como esta funci�n fu� pensada para trabajar sobre registros de desplazamiento del tipo HC595, cabe aclarar que en estos registros los datos serializados
	ingresan por el bit menos significativo del registro para desplazarse hacia los m�s significativos.  O sea, cada byte serializado hacia el registro se
	presentar� invertido en las salidas del mismo.
			byte->				1								0
			bit->	 15 14 13 12 11 10  9  8		  7  6  5  4  3  2  1  0
					 _______________________		 _______________________
					| A  B  C  D  E  F  G  H|		| A  B  C  D  E  F  G  H|
	salida	>---->--|SER       HC595       Q|-->->--|SER       HC595       Q|-->->--//////
	micro			|_______________________|		|_______________________|

	Parametros:		*Buffer1-6			//Punteros a la variables o vectores que contendra los bits a ser enviados para cada salida.
					Bits				//Cantidad de bits a ser enviados a los registros de desplazamiento.
	Nota: Esta funci�n no actua sobre la habilitaci�n de las salidas del registro de desplazamiento.  Anterior o posteriormente al envio de los
	datos, deber� ser llamada la funcion SIPOEnableOutputs() para que los datos enviados esten efectivamente presentes a la salida del registro
	de desplazamiento, si es que anteriormente las salidas fueron deshabilitadas.  Por defecto, la funcion SIPOInit() habilita las salidas
	paralelas del registro.
*/

extern inline void MSIPOAddData(uint8_t *buf1, uint8_t *buf2, uint8_t *buf3, uint8_t *buf4, uint8_t *buf5, uint8_t *buf6, uint8_t Bits){
 	uint8_t Byte=0;

	for(uint8_t clock_pulse=0;clock_pulse<Bits;clock_pulse++)
 		{Byte=(uint8_t) (clock_pulse/8);//determina el byte del buffer de donde se retiran los datos
	 	 //dato serial 1
		 if(bit_is_set(buf1[Byte],(clock_pulse-(Byte*8))))
	 		MSIPO_PORT|=_BV(MSIPO_SER1_PIN);
		 else
	 		MSIPO_PORT&=~_BV(MSIPO_SER1_PIN);
			
		//dato serial 2
		 if(bit_is_set(buf2[Byte],(clock_pulse-(Byte*8))))
	 		MSIPO_PORT|=_BV(MSIPO_SER2_PIN);
		 else
	 		MSIPO_PORT&=~_BV(MSIPO_SER2_PIN);
			
		//dato serial 3
		 if(bit_is_set(buf3[Byte],(clock_pulse-(Byte*8))))
	 		MSIPO_PORT|=_BV(MSIPO_SER3_PIN);
		 else
	 		MSIPO_PORT&=~_BV(MSIPO_SER3_PIN);
			
		//dato serial 4
		 if(bit_is_set(buf4[Byte],(clock_pulse-(Byte*8))))
	 		MSIPO_PORT|=_BV(MSIPO_SER4_PIN);
		 else
	 		MSIPO_PORT&=~_BV(MSIPO_SER4_PIN);

		//dato serial 5
		 if(bit_is_set(buf5[Byte],(clock_pulse-(Byte*8))))
		 	MSIPO_PORT|=_BV(MSIPO_SER5_PIN);
		 else
		 	MSIPO_PORT&=~_BV(MSIPO_SER5_PIN);

		//dato serial 6
		 if(bit_is_set(buf6[Byte],(clock_pulse-(Byte*8))))
		 	MSIPO_PORT|=_BV(MSIPO_SER6_PIN);
		 else
		 	MSIPO_PORT&=~_BV(MSIPO_SER6_PIN);
			
		 MSIPOShiftClockPulse();
		}
};

//	Escribe 1 byte en las lineas SIPO.  La funci�n permite la escritura recursiva.
//	Parametros:		data	//Byte a ser escrito en el SIPO
extern inline void MSIPOAddByte(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6){
	MSIPOAddData(&data1, &data2, &data3, &data4, &data5, &data6, 8);
};


/**********************************************************************************
				Funciones NO RECURSIVAS a registros SIPO

		Estas funciones poseen el t�rmino "Write" en sus nombres.  Estas funciones realizan por si mismas todo el procedimiento desde
		el reseteo de los SIPO, la serializaci�n de las datos a los mismos y la posterior presentaci�n de esa carga en las salidas 
		de los registros.  Estas funciones no requieren hacer llamadas previas y posteriores a funciones de reseteo de los SIPO ni 
		a funciones de presentaci�n de la carga en las salidas.
		No deben ser usadas si se debe realizar una carga por partes de los registros.

************************************************************************************/

/*	Ejecuta todo el proceso desde la transmision serial de los datos referenciados por los punteros Buffers hasta presentar estos datos
	en las salidas paralelas del ShiftRegister.
	La serializaci�n se realiza desde los bits de menor peso en direcciones bajas del buffer hasta los bits de mayor peso en direcciones altas del 
	buffer:

						  /------Buffer--------\

				byte 0->   <-|7|6| <-- |1|0|
						  |__________>_________
											   |
				byte 1->   <-|7|6| <-- |1|0|<__|
						  |__________>_________
											   |
				byte 2->	 |7|6| <-- |1|0|<__|

	Como esta funci�n fu� pensada para trabajar sobre registros de desplazamiento del tipo HC595, cabe aclarar que en estos registros los datos serializados
	ingresan por el bit menos significativo del registro para desplazarse hacia los m�s significativos.  O sea, cada byte serializado hacia el registro se
	presentar� invertido en las salidas del mismo.

	Parametros:		*bufx			//Puntero a la variable o vector que contendra los bits a ser enviados.
					Bits			//Cantidad de bits a ser enviados al  registro de desplazamiento.
	Nota: Esta funci�n no actua sobre la habilitaci�n de las salidas del registro de desplazamiento.  Anterior o posteriormente al envio de los
	datos, deber� ser llamada la funcion SIPOEnableOutputs() para que los datos enviados esten efectivamente presentes a la salida del registro
	de desplazamiento, si es que anteriormente las salidas fueron deshabilitadas.  Por defecto, la funcion SIPOInit() habilita las salidas paralelas del registro.
*/
extern inline void MSIPOWriteData(uint8_t *buf1, uint8_t *buf2, uint8_t *buf3, uint8_t *buf4, uint8_t *buf5, uint8_t *buf6, uint8_t Bits){
#ifdef MSIPO_USE_MR
	MSIPOMasterReset();
#endif
	MSIPOAddData(buf1, buf2, buf3, buf4, buf5, buf6, Bits);
	MSIPOLatchLoad();
};


//	Escribe 1 byte en las lineas SIPO.  La funci�n NO es de escritura recursiva;  la funci�n realiza una limpieza de los SIPOs antes de ser 
//	escritos, por lo que se supone que las lineas SIPO son de un byte de longitud.
//	Parametros:		datax	//Byte a ser escrito en las lineas SIPO
extern inline void MSIPOWriteByte(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6){
	MSIPOWriteData(&data1, &data2, &data3, &data4, &data5, &data6, 8);
};

#endif
