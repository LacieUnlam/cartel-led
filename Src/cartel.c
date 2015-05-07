
/* 
	cartel.c
*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#include "MultiSIPOReg.h"

#include "cartel.h"

//#define CT_REFRESH_TIME 1500 //tiempo de refresco en uSeg, para demora de SIPO de 1uSeg.
//tiempo de refresco medidos en uSeg
//#define CT_MREFRESH_TIME 10750 //SIN demora de SIPO.
//#define CT_MREFRESH_TIME 11250 //para demora de SIPO de 1uSeg.
#define CT_MREFRESH_TIME 11616 //para demora de SIPO de 2uSeg.
//#define CT_MREFRESH_TIME 12800 //para demora de SIPO de 4uSeg.
//#define CT_MREFRESH_TIME 16000 //para demora de SIPO de 10uSeg.
//#define CT_MREFRESH_TIME 18592 //para demora de SIPO de 15uSeg.

//tiempo de borrado medidos en uSeg
//#define CT_MCLEAR_TIME	4200  //SIN demora de SIPO.
//#define CT_MCLEAR_TIME	4700  //para demora de SIPO de 1uSeg.
#define CT_MCLEAR_TIME	5170  //para demora de SIPO de 2uSeg.
//#define CT_MCLEAR_TIME	6300  //para demora de SIPO de 4uSeg.
//#define CT_MCLEAR_TIME	9400  //para demora de SIPO de 10uSeg.
//#define CT_MCLEAR_TIME	12033 //para demora de SIPO de 15uSeg.

//Tiempos medidos + incertidumbre en uSeg.
#define CT_CLEAR_TIME (CT_MCLEAR_TIME+500)
#define CT_REFRESH_TIME (CT_MREFRESH_TIME+500)

//#define CT_DUTY_STEPS (8000/CT_REFRESH_TIME) //cantidad de pasos por cuadro, suponiendo FR=25

#define CT_FR 39 //FrameRate
#define CT_FP (1000000/CT_FR) //Período de cuadro, en uSeg.
#define CT_TIME_PER_DUTY (CT_FP/100)//tiempo por paso de duty, en uSeg.
#define CT_DUTY_MIN (CT_CLEAR_TIME/CT_TIME_PER_DUTY)
#define CT_DUTY_MAX (100-(CT_REFRESH_TIME/CT_TIME_PER_DUTY))

const uint8_t*		ct_font; //puntero a fuente seleccionada
uint8_t				ct_font_color; //color de fuente

void CtInit()
{
 MSIPOInit();
 ct_actual_frame_time=0;
 ct_f_update=0;
 ct_f_clear=0;
 ct_map_sel=CT_MAP_OUT;
}

void CtClear()
{for(uint16_t i=0;i<CT_BYTES_LENGTH;i++)
	ct_map[ct_map_sel][i]=0;
}

void CtFull()
{for(uint16_t i=0;i<CT_BYTES_LENGTH;i++)
	ct_map[ct_map_sel][i]=0xFF;
}

void CtInvert()
{for(uint16_t i=0;i<CT_BYTES_LENGTH;i++)
	ct_map[ct_map_sel][i]=~ct_map[ct_map_sel][i];
}

void CtMapCopy(uint8_t dest, uint8_t fuente)
{for(uint8_t i=0; i<CT_BYTES_LENGTH; i++)
	ct_map[dest][i]=ct_map[fuente][i];
}

void CtMapLogicOp(uint8_t op)
{for(uint8_t i=0; i<CT_BYTES_LENGTH; i++)
	{switch(op)
		{case CT_AND:	//solo unos comunes (controlar con las figuras de uno, la vision del otro)
							ct_map[CT_MAP_OUT][i]=ct_map[CT_MAP_A][i] & ct_map[CT_MAP_B][i];
							break;
		 case CT_OR:	//combina ambos
							ct_map[CT_MAP_OUT][i]=ct_map[CT_MAP_A][i] | ct_map[CT_MAP_B][i];
							break;
		 case CT_XOR:	//positivo sobre 0, negativo sobre uno.
							ct_map[CT_MAP_OUT][i]=ct_map[CT_MAP_A][i] | ct_map[CT_MAP_B][i];
							break;
////////////////////////////////////////////////////////////////////////////////////////
//			TODO: Sin terminar - 4/10/12
///////////////////////////////////////////////////////////////////////////////////////
		}

	}

}

void CtUpdate()
{uint8_t byte[FILES_QTY]; //Un byte por salida serie SIPO.

 for(uint8_t i=0;i<FILES_QTY;i++)
	 byte[i]=0;

 for(int8_t modulo=0; modulo<MODULO_FILE_QTY; modulo++)	//una pasada por cada columna de modulos
	for(int8_t mod_fila=3; mod_fila>=0; mod_fila--)						//una pasada por cada fila en el módulo
		{for(int8_t mod_col=7; mod_col>=0; mod_col--)					//una pasada por cada columna en el módulo
			{
#if (FILES_QTY>0)
			 //carga de byte 0
			 if(bit_is_set(ct_map[ct_map_sel][(modulo*8)+mod_col],mod_fila))
				byte[0]|=_BV(7-mod_col);
			 else
				byte[0]&=~_BV(7-mod_col);
#endif
#if (FILES_QTY>1)
			 //carga de byte 1
			 if(bit_is_set(ct_map[ct_map_sel][(modulo*8)+mod_col],mod_fila+4))
				byte[1]|=_BV(7-mod_col);
			 else
				byte[1]&=~_BV(7-mod_col);
#endif
#if (FILES_QTY>2)
			 //carga de byte 2
			 if(bit_is_set(ct_map[ct_map_sel][CT_ROW_DOTS+(modulo*8)+mod_col],mod_fila))
				byte[2]|=_BV(7-mod_col);
			 else
				byte[2]&=~_BV(7-mod_col);
#endif
#if (FILES_QTY>3)
			 //carga de byte 3
			 if(bit_is_set(ct_map[ct_map_sel][CT_ROW_DOTS+(modulo*8)+mod_col],mod_fila+4))
				byte[3]|=_BV(7-mod_col);
			 else
				byte[3]&=~_BV(7-mod_col);
#endif
#if (FILES_QTY>4)
			 //carga de byte 4
			 if(bit_is_set(ct_map[ct_map_sel][2*CT_ROW_DOTS+(modulo*8)+mod_col],mod_fila))
			 	byte[4]|=_BV(7-mod_col);
			 else
			 	byte[4]&=~_BV(7-mod_col);
#endif
#if (FILES_QTY>5)
			 //carga de byte 5
			 if(bit_is_set(ct_map[ct_map_sel][2*CT_ROW_DOTS+(modulo*8)+mod_col],mod_fila+4))
			 	byte[5]|=_BV(7-mod_col);
			 else
			 	byte[5]&=~_BV(7-mod_col);
#endif
			}

#if (FILES_QTY==1)
		 MSIPOAddByte(byte[0],0,0,0,0,0);
#elif (FILES_QTY==2)
		 MSIPOAddByte(byte[0],byte[1],0,0,0,0);
#elif (FILES_QTY==3)
		 MSIPOAddByte(byte[0],byte[1],byte[2],0,0,0);
#elif (FILES_QTY==4)
		 MSIPOAddByte(byte[0],byte[1],byte[2],byte[3],0,0);
#elif (FILES_QTY==5)
		 MSIPOAddByte(byte[0],byte[1],byte[2],byte[3],byte[4],0);
#elif (FILES_QTY==6)
		 MSIPOAddByte(byte[0],byte[1],byte[2],byte[3],byte[4],byte[5]);
#endif
		}

 MSIPOLatchLoad();
}

void CtClrScr()
{for(uint8_t i=0;i<(MODULO_FILE_QTY*4);i++) //modulos por fila x 4 filas (de 8 LEDs) por módulo
	 MSIPOAddByte(0,0,0,0,0,0);
 MSIPOLatchLoad();
}

uint8_t CtDuty(uint8_t duty, uint16_t time)
{uint16_t duty_time;
 uint8_t ret=0;

 if(duty>100)
	 duty=100;

 ct_actual_frame_time+=time; //actualiza el tiempo del ciclo
 while(ct_actual_frame_time>CT_FP)//corrige si hay desborde
	 {ct_actual_frame_time-=CT_FP;
	  ct_f_update=0;
	  ct_f_clear=0;
	  ret=1;
	 }

 duty_time=duty*CT_TIME_PER_DUTY;
 if(duty_time<CT_CLEAR_TIME)
	 duty_time=CT_CLEAR_TIME;

 if(ct_actual_frame_time >= CT_FP-CT_REFRESH_TIME && !ct_f_update)
	 {CtUpdate();
	  ct_f_update=1;
	 }

 if((ct_actual_frame_time >= duty_time-CT_CLEAR_TIME) && (CT_FP-CT_CLEAR_TIME-CT_REFRESH_TIME > ct_actual_frame_time) && !ct_f_clear)
  	 {CtClrScr();
  	  ct_f_clear=1;
  	 }

 return ret;
}

uint8_t CtGoto(int16_t x, int16_t y) //x->vertical, y->horizontal.  Desde esquina superior izquierda
{uint8_t f_over=0, f_x_neg=0, f_y_neg=0;

 f_over=(0<x && x<CT_COL_DOTS && 0<y && y<CT_ROW_DOTS)? 0: 1; //indica si hubo overflow de alguna de las coordenadas

 //Ajusta X e Y para referenciarlas a 0.
 if(x>0) x--; else	{x++; f_x_neg=1;}
 if(y>0) y--; else	{y++; f_y_neg=1;}

 x=(x%CT_COL_DOTS);
 y=(y%CT_ROW_DOTS);

 //Si alguna es negativa, se la referencia desde la esquina superior izquierda
 if(f_x_neg) x+=CT_COL_DOTS-1;
 if(f_y_neg) y+=CT_ROW_DOTS-1;

 ct_pos_bit=x%8; //determina la posición dentro del byte
 ct_pos_byte=CT_ROW_DOTS*(x/8)+y; //determina el byte en el mapa

 return f_over;
}

uint8_t CtSetDot(int16_t x, int16_t y, uint8_t color)
{uint8_t f_over=0;

 f_over=CtGoto(x,y);

 if(color==BLACK)
	ct_map[ct_map_sel][ct_pos_byte]|=_BV(ct_pos_bit);
 else
	ct_map[ct_map_sel][ct_pos_byte]&=~_BV(ct_pos_bit);

 return f_over;
}

void CtLineH(uint16_t x, uint16_t y1, uint16_t y2, uint8_t color)
{if(y2-y1>0)
	while(y1<=y2)
		CtSetDot(x,y1++,color);
 else
	while(y1>=y2)
		CtSetDot(x,y1--,color);
}

void CtLineV(uint16_t y, uint16_t x1, uint16_t x2, uint8_t color)
{if(x2-x1>0)
	while(x1<=x2)
		CtSetDot(x1++,y,color);
 else
	while(x1>=x2)
		CtSetDot(x1--,y,color);
}

void CtRec(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color, uint8_t full)
{if(full)
	{//lleno
	 for(int8_t i=0;abs(x2-x1)>=i;i++)
		 {CtLineH(x1+i,y1,y2,color);
		 }
	}
 else
	 {//solo borde
	  CtLineH(x1,y1,y2,color);
	  CtLineH(x2,y1,y2,color);
	  CtLineV(y1,x1,x2,color);
	  CtLineV(y2,x1,x2,color);
	 }
}

//TODO: Sin terminar
/*
void CtDrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{int8_t pend;
 int8_t error;
 
 //Para el análisis, suponer al cartel rotado 90 grados en sentido antihorario, para verlo
 //en la forma cartesiana comun.
 if(x2==x1) //linea vertical
	{if(y2-y1>0)
		while(y1<=y2)
			CtSetDot(x1,y1++,color);
	 else
		while(y1>=y2)
			CtSetDot(x1,y1--,color);
	}
 else
	{//Linea inclinada u horizontal
	 if(abs(y2-y1)>=abs(x2-x1))
		{//linea con pendiente menor a -1 o mayor a 1, pero nunca vertical
		 pend=(y2-y1)/(x2-x1);
		 error=(y2-y1)%(x2-x1);
		 if(y2>=y1)
			 {while(y2>=y1)
				 {

				 }

			 }

		}
	 else
		{//linea con pendiente entre -1 y 1

		}

	}

}
*/

/************************************************************************
				Funciones para uso de fuentes
************************************************************************/

uint8_t CtDataRead(const uint8_t* ptr) {
	return pgm_read_byte(ptr);
}

void CtSelectFont(const char* font, uint8_t color) {
	ct_font = (uint8_t*)font;
	ct_font_color = color;
	ct_font_height=CtDataRead(ct_font+FONT_HEIGHT); //carga la altura de la fuente
}

int8_t CtCharWidth(char c)
{
 uint8_t firstChar = CtDataRead(ct_font+FONT_FIRST_CHAR); //primer caracter ASCII definido de la fuente
 uint8_t charCount = CtDataRead(ct_font+FONT_CHAR_COUNT); //cantidad de caracteres de la fuente

 if(c < firstChar || c >= (firstChar+charCount)) //verifica si el caracter requerido esta presente en la fuente
	 return -1; //devuelve -1 si el caracter no existe.

 c-= firstChar; //(CH) Solo los caracteres imprimibles de la tabla ascii se presentan en el .h descriptor de la fuente a utilizar.

 return (CtDataRead(ct_font+FONT_WIDTH_TABLE+c)); //recupera de la tabla de anchos, el que se corresponde con el caracter requerido
}

uint8_t CtPutChar(char c, uint8_t x, uint8_t y) 
{
	uint8_t width = 0;
	ct_last_char_height=0;
	uint8_t bytes = (ct_font_height+7)/8; //(CH) Bytes ocupados por el alto de la fuente.
	
	uint8_t firstChar = CtDataRead(ct_font+FONT_FIRST_CHAR); //primer caracter ASCII definido de la fuente
	uint8_t charCount = CtDataRead(ct_font+FONT_CHAR_COUNT); //cantidad de caracteres de la fuente
	
	if(c < firstChar || c >= (firstChar+charCount)) //verifica si el caracter requerido esta presente en la fuente
		return 0;

	c-= firstChar; //(CH) Solo los caracteres imprimibles de la tabla ascii se presentan en el .h descriptor de la fuente a utilizar.
	
uint16_t index = 0;
	// read width data, to get the index
	for(uint8_t i=0; i<c; i++) //(CH)acumula los valores de la tabla de anchos para obtener el salto hasta el caracter a imprimir
		index += CtDataRead(ct_font+FONT_WIDTH_TABLE+i);
	
index = index*bytes+charCount+FONT_WIDTH_TABLE;
width = CtDataRead(ct_font+FONT_WIDTH_TABLE+c); //recupera de la tabla de anchos, el que se corresponde con el caracter requerido

// last but not least, draw the character
for(uint8_t i=0; i<bytes; i++) {//una vuelta por cada byte en altura de la fuente
	uint8_t page = i*width;	//Una pagina contiene todos los bytes de cada fila en la fuente.
	for(uint8_t j=0; j<width; j++) {//una vuelta por cada byte de la pagina
		uint8_t data = CtDataRead(ct_font+index+page+j);
		
		if(ct_font_height < (i+1)*8) {			//si es la última fila de bytes del caracter, y con ella se supera la altura del caracter.
			data >>= (i+1)*8-ct_font_height;	//no se porque esto
		}		
		
		if(ct_font_color != BLACK)
			data=~data;
			
		for(uint8_t k=0;k<8;k++)//un ciclo por cada bit dibujado del byte
			{if(i*8+k>ct_font_height)
				break;//Si el punto queda por arriba del tamaño de la fuente, se interrumpe la impresión del byte
				
			 if(bit_is_set(data,k))//dibuja el punto
				CtSetDot(x+(i*8)+k,y+j,BLACK);
			 else
				CtSetDot(x+(i*8)+k,y+j,WHITE);
				
			 if(ct_font_color==BLACK)//al salir la función, se habra guardado en ct_last_char_height la altura de este caracter.
				{if((bit_is_set(data,k)) && ((i*8)+k>ct_last_char_height))
					ct_last_char_height=(i*8)+k;
				}					
			 else
				{if((bit_is_clear(data,k)) && ((i*8)+k>ct_last_char_height))
					ct_last_char_height=(i*8)+k;
				}					
			
			}
		}
	}
ct_last_char_width=width;
return 1;
}

void CtPuts(char *str,uint8_t x,uint8_t y)
{uint8_t l=y, h=x;
 uint8_t line_height=0;

 while(*str!=0)
	{if(*str!='\n')
		{CtPutChar(*str,h,l);
		 l+=ct_last_char_width+1;
		 if(line_height<ct_last_char_height)
			line_height=ct_last_char_height;
		}		
	 else
		{h+=line_height+1;//un pixel de separacion entre lineas
		 line_height=0;
		 l=y;
		}
	 str++;
	 
	 //Un pixel de separación entre caracteres
	 for(uint8_t i=0;i<ct_font_height;i++)
		if(ct_font_color==BLACK)
			CtSetDot(h+i,l,WHITE);
		else
			CtSetDot(h+i,l,BLACK);
	}
}
