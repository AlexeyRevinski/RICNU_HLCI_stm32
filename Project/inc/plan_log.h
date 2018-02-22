#ifndef PLAN_LOG_H
#define PLAN_LOG_H

#include "plan_include.h"

#define LOG_FIFO_SIZE	4
#define LOG_SECTOR_SIZE	5
#define LOG_STRING_SIZE	LOG_SECTOR_SIZE*LOG_FIFO_SIZE

typedef struct
{
	uint16_t years;
	uint8_t months;
	uint8_t days;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t mseconds;
} time;

struct Queue
{
    uint8_t front;
    uint8_t rear;
    uint8_t size;
    uint8_t capacity;
    //uint8_t* array;
};


void 	log_buf_clear();
int 	log_buf_append(char* str, uint16_t len);

int		log_file_append();

void	log_time_set(uint16_t hr,uint8_t min,uint8_t sec);
void	log_time_get();

void	log_q_init();
int		log_q_is_full();
int		log_q_is_empty();
void 	log_q_enq();
void	log_q_deq();



#endif //PLAN_LOG_H
