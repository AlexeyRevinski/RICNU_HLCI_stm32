#ifndef PLAN_LOG_H
#define PLAN_LOG_H

#include "plan_include.h"

#define LOG_FIFO_SIZE	6
#define LOG_SECTOR_SIZE	512
#define LOG_BUFFER_SIZE	LOG_SECTOR_SIZE*LOG_FIFO_SIZE

//00000000| 00 00:00:00.000 A 00010 00201 30102 XXXX XXXX XXXX XXXX XXXX XXXX XXXXXXXX XXXXXXXX XXXX XXXX XXX XXX XXX XXX XXX XXXn


#define LOG_STR_ID							0
#define LOG_STR_HOUR	1+LOG_STR_ID+		8
#define LOG_STR_MINUTE	1+LOG_STR_HOUR+		2
#define LOG_STR_SECOND	1+LOG_STR_MINUTE+	2
#define LOG_STR_MSECOND	1+LOG_STR_SECOND+	2
#define LOG_STR_SYS_ST	1+LOG_STR_MSECOND+	3
#define LOG_STR_FSM_M	1+LOG_STR_SYS_ST+	1
#define LOG_STR_FSM_S	1+LOG_STR_FSM_M+	5
#define LOG_STR_FSM_T	1+LOG_STR_FSM_S+	5
#define LOG_STR_DAT_GX	1+LOG_STR_FSM_T+	5
#define LOG_STR_DAT_GY	1+LOG_STR_DAT_GX+	4
#define LOG_STR_DAT_GZ	1+LOG_STR_DAT_GY+	4
#define LOG_STR_DAT_AX	1+LOG_STR_DAT_GZ+	4
#define LOG_STR_DAT_AY	1+LOG_STR_DAT_AX+	4
#define LOG_STR_DAT_AZ	1+LOG_STR_DAT_AY+	4
#define LOG_STR_DAT_EM	1+LOG_STR_DAT_AZ+	4
#define LOG_STR_DAT_EJ	1+LOG_STR_DAT_EM+	8
#define LOG_STR_DAT_CU	1+LOG_STR_DAT_EJ+	8
#define LOG_STR_DAT_SC	1+LOG_STR_DAT_CU+	4
#define LOG_STR_DAT_S0	1+LOG_STR_DAT_SC+	8
#define LOG_STR_DAT_S1	1+LOG_STR_DAT_S0+	3
#define LOG_STR_DAT_S2	1+LOG_STR_DAT_S1+	3
#define LOG_STR_DAT_S3	1+LOG_STR_DAT_S2+	3
#define LOG_STR_DAT_S4	1+LOG_STR_DAT_S3+	3
#define LOG_STR_DAT_S5	1+LOG_STR_DAT_S4+	3
#define LOG_STR_SIZE	1+LOG_STR_DAT_S5+	3	// Log string size


typedef struct
{
	uint16_t years;
	uint8_t months;
	uint8_t days;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t mseconds;
}logtime;

typedef struct
{
    uint8_t front;
    uint8_t rear;
    uint8_t size;
    uint8_t capacity;
}logqueue;

typedef enum
{
	LOG_ON,
	LOG_OFF,
}lstate;


void 	log_init();
void 	log_generate();
void 	log_str_clear();
void 	log_gen_string();
void 	log_buf_clear();
int 	log_buf_append(char* str, uint16_t len);
int		log_file_append();
void	log_time_set(uint32_t hours, uint8_t minutes, uint8_t seconds);
void	log_time_get();
void	log_q_init();
int		log_q_is_full();
int		log_q_is_empty();
void 	log_q_enq();
void	log_q_deq();

#endif //PLAN_LOG_H
