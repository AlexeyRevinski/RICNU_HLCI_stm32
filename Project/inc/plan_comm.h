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
	uint8_t offset;
	uint8_t control;
	uint32_t setpoint;
	uint16_t g0;
	uint16_t g1;
	uint16_t g2;
	uint16_t g3;
} flexsea_ctrl;

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
uint8_t     comm_unpack_manage(void);
void    	comm_start(device);
void    	comm_unpack_user(void);
void    	comm_prep_manage(void);
void		comm_send_manage(void);
void 		comm_send_user_cmd(uint8_t cmd);
void 		comm_prep_user(uint8_t offset);

#endif //PLAN_COMM_H
