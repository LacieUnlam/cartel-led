/*
 * cartel_comm.h
 *
 *  Created on: 24/09/2012
 *      Author: Christian
 */

#ifndef CT_COMM_H_
#define CT_COMM_H_

//Definicion de comandos
#define CMD_NONE		0
#define CMD_HELP		1
#define CMD_INVALID		2
#define CMD_UPDATE		3
#define CMD_CLEAR		4
#define CMD_GOTO		5
#define INVALID_DATA	6
#define CMD_COLOR		7
#define CMD_FONT_SEL	8
#define CMD_TEXT		9
#define CMD_POINT		10
#define CMD_INVERT		11
#define CMD_LINE		12
#define CMD_RECT		13
#define	CMD_OP			14
#define	CMD_CRONO		15
#define CMD_CRO_SEC		16
#define CMD_CRO_PAUSE	17
#define CMD_CRO_CONF	18
#define CMD_BRI_UP		19
#define CMD_BRI_DW		20
#define CMD_PRG_MSG1	21
#define CMD_PRG_MSG2	22
#define CMD_PRG_EXE		23
#define CMD_SCROLL		24

//Definicion de mensajes
#define MSG_HELP		0
#define MSG_INV_COMM	1
#define MSG_OK			2
#define MSG_INV_DATA	3
#define MSG_INV_FONT	4
#define	MSG_OVER		5

void CtCommInit();
uint8_t CtCommDecoder();
void CtCommPrint(uint8_t msg);

#endif /* CT_COMM_H_ */
