/*
 * CartelTest.c
 *
 * Created: 20/09/2012 20:11:45
 *  Author: alumnos
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "MultiSIPOReg.h"

int main(void)
{volatile uint32_t dato=0;
 uint8_t dir=1; //1:enciende en secuencia, 0: apaga en secuencia.
  
 MSIPOInit();
 
 while(1)
	{dato=dato<<1;
	 if(dir==1)
		{dato|=1;
		 if(dato==0xFFFFFFFF)
			dir=0;
		}
	 else
		{dato&=~((uint32_t)1);
		 if(dato==0)
			dir=1;
		}
					
	 MSIPOWriteData((uint8_t*)(&dato),(uint8_t*)(&dato),(uint8_t*)(&dato),(uint8_t*)(&dato),32);
	 
	  for(uint8_t i=0;i<30;i++)
		_delay_ms(10);
	}
}