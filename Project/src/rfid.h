#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>

#define MISO 21
#define MOSI 19
#define SCLK 23
#define CE0 24
#define CE1 26

#define REG_CMD 0x01
#define REG_TMODE 0x2A
#define REG_PRESCALER 0x2B
#define REG_RELOADL 0x2D
#define REG_RELOADH 0x2C
#define REG_TXAUTO 0x15
#define REG_MODE 0x11
#define REG_TXCONTROL 0x14
#define REG_BITFRAMING 0x0D

/* Commands */
#define CMD_IDLE 0x00
#define CMD_MEM 0x01
#define CMD_RAND_ID 0x02
#define CMD_CALC_CRC 0x03
#define CMD_TRANSMIT 0x04
#define CMD_NO_CMD_CHANGE 0x07
#define CMD_RECEIVE 0x08
#define CMD_TRANSCEIVE 0xC
#define CMD_MF_AUTHENT 0xE
#define CMD_RESET 0x0F

/* https://github.com/mxgxw/MFRC522-python/blob/master/MFRC522.py */

void rfid_write(char addr, char val);
char rfid_read(char addr);
void rfid_reset();
int rfid_init();
void antenna_on();

/*  functions for external use */
int rfid_scan();








