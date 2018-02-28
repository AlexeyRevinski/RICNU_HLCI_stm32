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
	int16_t gx;
	int16_t gy;
	int16_t gz;
	int16_t ax;
	int16_t ay;
	int16_t az;
	int32_t em;
	int32_t ej;
	int16_t cu;
	int32_t sc;
	int32_t st[6];
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
