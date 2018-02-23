#ifndef PLAN_COMM_H
#define PLAN_COMM_H

// INCLUDES  ===================================================================

#include "plan_include.h"


// CONSTANTS ===================================================================

#define CMD_RICNU			101
#define DATA_SIZE_FLEXSEA               COMM_STR_BUF_LEN      //bytes
#define DATA_SIZE_P2U                   25      //bytes
#define DATA_SIZE_U2P                   2       //bytes
#define POS_MOTOR_ON_OFF                0
#define POS_MOTOR_DIRECT                1
#define COMM_ERR                        0
#define COMM_OK                         1

typedef enum
{
  MANAGE,
  USER,
} device;



typedef struct
{
	uint16_t gx;
	uint16_t gy;
	uint16_t gz;
	uint16_t ax;
	uint16_t ay;
	uint16_t az;
	uint32_t em;
	uint32_t ej;
	uint16_t cu;
	uint32_t sc;
	uint16_t s0;
	uint16_t s1;
	uint16_t s2;
	uint16_t s3;
	uint16_t s4;
	uint16_t s5;
} ricnu_data;


// FUNCTION PROTOTYPES  ========================================================
int     unpack(device);
void    update(device);
void    update_Manage(void);
void    update_User(void);
int     pack_P2U(void);
void    pack_U2P(void);
void    prep_packet(uint8_t offset, uint8_t ctrl, int32_t sp, int16_t gain0, \
                 int16_t gain1, int16_t gain2, int16_t gain3);

#endif //PLAN_COMM_H
