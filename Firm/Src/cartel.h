
/* 
	cartel.h
*/

#ifndef CT_H
#define CT_H

//sentido de carga en el cartel
#define CT_DIR_IZQ 1 		//en ISIS
//#define CT_DIR_DER 0		//en la facu

//dimensiones del mapa en memoria del cartel
#define MODULO_FILE_QTY		8						//cantidad de modulos en una fila
#define FILES_QTY			6						//cantidad de filas de modulos

#define CT_COL_DOTS			(FILES_QTY*4)			//cantidad de puntos verticales
#define CT_ROW_DOTS			(8*MODULO_FILE_QTY)		//cantidad de puntos horizontales

//Cantidad de bytes del cartel.  Un byte cada 2 lineas de modulos.
#define CT_BYTES_LENGTH	(((FILES_QTY+1) / 2)*CT_ROW_DOTS)

//Identificacion de los mapas
#define CT_MAP_OUT		0 //mapa de salida y refresco
#define CT_MAP_A		1
#define CT_MAP_B		2

//definicion de operadores logicos entre mapas
#define CT_AND	0	//ambos en 1 (Intersección da 1)
#define CT_OR	1	//cualquiera en 1. (Union da 1)
#define CT_XOR	2	//uno solo en 1 (las figuras de un mapa negativiza las del otro)
#define CT_XNOR	3	//ambos en 0 o en 1.
#define CT_NAND	4	//si alguno de los 2 o ambos son 0

//color de graficos y fuentes
#define BLACK	1
#define WHITE	0

// Font Indices
#define FONT_LENGTH			0
#define FONT_FIXED_WIDTH	2
#define FONT_HEIGHT			3
#define FONT_FIRST_CHAR		4
#define FONT_CHAR_COUNT		5
#define FONT_WIDTH_TABLE	6

#define FONT_LINE_SPA		1 //pixel de espaciado entre lineas

//	Variables globales
volatile uint8_t ct_map[3][CT_BYTES_LENGTH]; //matriz con los mapas
volatile uint8_t ct_map_sel; //seleccion del cartel a editar
volatile uint8_t ct_pos_byte, ct_pos_bit; //posición lógica en el cartel
volatile uint8_t ct_last_char_width; //para calcular la distancia entre caracteres
volatile uint8_t ct_last_char_height; //para calcular la distancia entre lineas
volatile uint8_t ct_font_height;
volatile uint16_t ct_actual_frame_time; //indica el tiempo transcurrido en el cuadro actual
volatile uint8_t ct_f_update; //flag indicando op de refresco en el cuadro
volatile uint8_t ct_f_clear;  //flag indicando op de borrado en el cuadro

//	Declaracion de funciones
void CtInit();		//Inicia los recursos del cartel
void CtClear(); 	//Borra el cartel en memoria
void CtClrScr(); 	//Borra el cartel sin modificar la memoria.
uint8_t CtDuty(uint8_t, uint16_t); //controla el nivel de brillo
void CtFull();		//Pone todos en uno
void CtInvert();	//Invierte todos los puntos.
void CtMapCopy(uint8_t, uint8_t); //copia un mapa en otro.  El primer parametro es destino, el segundo fuente.
void CtUpdate(); //Actualiza el buffer de refresco
void CtRefresh();	//Actualiza el cartel
uint8_t CtGoto(int16_t x, int16_t y);	//Situa el cursor en un punto del cartel.  X crece hacia abajo, mientras que Y hacia la derecha.
										//No acepta cero como valor posible.
										//Retorna 0 si no hay overflow ni en X, ni en Y.
uint8_t CtSetDot(int16_t x, int16_t y, uint8_t color);	//Dibuja un punto en memoria.  Devuelve 1 si hay overflow del cartel.
void CtLineH(uint16_t x, uint16_t y1, uint16_t y2, uint8_t color);	//linea horizontal
void CtLineV(uint16_t y, uint16_t x1, uint16_t x2, uint8_t color);	//linea vertical
void CtRec(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color, uint8_t full); //dibuja un rectángulo lleno o vacio

//void CtDrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color); //Dibuja una linea //TODO: a implementar

//Funciones para uso de fuentes
void CtSelectFont(const char* font, uint8_t color); //Seleciona tipo y color de fuente
uint8_t CtDataRead(const uint8_t* ptr); //Funcion interna para lectura de los datos de la fuente
int8_t CtCharWidth(char c);//devuelve el ancho en puntos del caracter.  -1 si no existe
uint8_t CtPutChar(char c, uint8_t x, uint8_t y);
void CtPuts(char *str,uint8_t x,uint8_t y);



#endif
