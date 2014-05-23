/*
 * cartel_comm.h
 *
 *  Created on: 24/09/2012
 *      Author: Christian
 */

#ifndef CT_COMM_H_
#define CT_COMM_H_

//Definicion de comandos
#define COMMAND_NONE		0
#define COMMAND_HELP		1
#define COMMAND_INVALID		2
#define COMMAND_UPDATE		3
#define COMMAND_CLEAR		4
#define COMMAND_GOTO		5
#define INVALID_DATA		6
#define COMMAND_COLOR		7
#define COMMAND_FONT_SEL	8
#define COMMAND_TEXT		9
#define COMMAND_POINT		10
#define COMMAND_INVERT		11
#define COMMAND_LINE		12
#define COMMAND_RECT		13
#define	COMMAND_OP			14
#define	COMMAND_CRONO		15
#define COMMAND_CRO_SEC		16

//Definicion de mensajes
#define MSG_HELP			0
#define MSG_INV_COMM		1
#define MSG_OK				2
#define MSG_INV_DATA		3
#define MSG_INV_FONT		4
#define	MSG_OVER			5

void CtCommInit();
uint8_t CtCommDecoder();
void CtCommPrint(uint8_t msg);

#endif /* CT_COMM_H_ */
