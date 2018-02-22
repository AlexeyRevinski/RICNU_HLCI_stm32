//Need:

//	Generate log strings when called from main or wherever else
//  Append them to the log buffer. Once log reaches 512 size (log string pointer - log start pointer), send them off, update next log start pointer
//  After sending, if successful, update send pointer accordingly

#include "plan_log.h"


time tm;
struct Queue logq;
uint8_t log_buffer[LOG_STRING_SIZE]; // Change to 2048?
uint8_t temp_log_buffer[LOG_STRING_SIZE];
uint16_t log_bi = 0;	// Current buffer index
uint16_t log_fs = 0;	// Location of current growing sector (front sector) in fifo
uint16_t log_bs = 0;	// Location of ready to send sector (back sector) in log_buffer



//==============================================================================
// FUNCTION log_buf_append()
//      - appends string to the buffer string based on queue and available space
//==============================================================================
int log_buf_append(char* str, uint16_t len)
{
	int bnd = 0;
	if(log_q_is_full()) return 0;		// If queue is full, no space to write
	if(LOG_STRING_SIZE>(log_bi+len-1))	// If string will fit in the array
	{
		uint8_t* temp=log_buffer+log_bi; 	// Make temporary pointer
		int i=0;							// Loop index
		for(;i<len;i++){temp[i] = str[i];}	// Copy string to buffer
	}
	else								// If string will not fit in the array --> last sector
	{
		if(logq.size<logq.capacity-1)			// If at least the first sector of buffer
		{										//   can be overwritten
			uint8_t* temp=log_buffer+log_bi; 	// Make temporary pointer
			int i = 0,j = 0;					// Loop indices
			for(;i<len;i++)						// Copy string to buffer -->
			{
				if((i+log_bi)<LOG_STRING_SIZE)		// If still fits -->
				{
					temp[i] = str[i];				// Copy character
					j++;							// Increment temp index
				}
				else								// Once hit end of array -->
				{
					log_buffer[i-j] = str[i];		// Restart at beginning of array
				}
			}
		}// Otherwise, queue is not full but can't fill the last sector
	}
	log_bi+=len;						// Increment pointer
	bnd = log_bi/LOG_SECTOR_SIZE;		// Integer division (tells which sector we are in now)
	if(bnd>logq.rear+1||				// If crossed or reached sector boundary
	  ((bnd==1)&&(logq.rear==(logq.capacity-1))))
	{
		log_q_enq();						// Enqueue this finished sector
	} // Otherwise do not enqueue
	if (log_bi>LOG_STRING_SIZE-1)		// If overflown -->
	{
		log_bi%=LOG_STRING_SIZE;			// Overflow index
	}
	return 1;
}


//==============================================================================
// FUNCTION log_buf_clear()
//      - clears entire log buffer
//==============================================================================
void log_buf_clear()
{
	int i = LOG_STRING_SIZE;					// Set index to LOG_STRING_SIZE
	do log_buffer[--i]=0; while(i);				// While going through buffer, clear
}


//==============================================================================
// FUNCTION log_appendfile()
//		- if a sector of data is available, writes that sector to memory card as
//			an addition to information already available in the log file
//==============================================================================
int log_file_append()
{
	if(log_q_is_empty()) return 1;				// If queue empty, nothing to write
	/*
	FRESULT fr; FIL fil;						// Result and file variables
	UINT bw = 0; UINT* bwp = &bw;
	fr = open_append(&fil, "ricnu.log");		// Create log file
	if (fr != FR_OK) return 1;					// If failed - disk error
	//f_printf(&fil, "mylog\n");				// Otherwise, append file
	f_write (&fil,log_buffer,
			logq.front*LOG_SECTOR_SIZE,bwp);											// TODO buff here is a const void*
	f_close(&fil);								// Close the file
	*/


	// test code
	int start = logq.front*LOG_SECTOR_SIZE;
	for(int i = 0;i<LOG_SECTOR_SIZE;i++)
	{
		temp_log_buffer[start+i] = log_buffer[start+i];
	}
	log_q_deq();

	return 0;									// Return success
}


//==============================================================================
// FUNCTION log_q_init()
//      - checks if queue is full
//==============================================================================
void log_q_init()
{
	logq.capacity = LOG_FIFO_SIZE;				// Set capacity to LOG_FIFO_SIZE
	logq.front = logq.size = 0;					// Set size to 0
	logq.rear = LOG_FIFO_SIZE-1;				// Set rear to item before last
}


//==============================================================================
// FUNCTION log_q_is_full()
//      - checks if queue is full
//==============================================================================
int log_q_is_full()
{
	return (logq.size == logq.capacity);	// Return 1 if size equals capacity
}


//==============================================================================
// FUNCTION log_q_is_empty()
//      - checks if queue is empty
//==============================================================================
int log_q_is_empty()
{
	return (logq.size == 0);					// Return 1 if size equals 0
}


//==============================================================================
// FUNCTION log_q_enq()
//      - adds an item to the queue
//==============================================================================
void log_q_enq()
{
	if (log_q_is_full()) return;			// Return if unable to enqueue
	logq.rear = (logq.rear + 1)%			// Increment rear pointer
			logq.capacity;					//   if reached capacity - circle back
	logq.size = logq.size + 1;				// Increment size of queue
}


//==============================================================================
// FUNCTION log_q_deq()
//      - removes an item from the queue
//==============================================================================
void log_q_deq()
{
	if (log_q_is_empty()) return;			// Return if queue is empty
    logq.front = (logq.front + 1)			// Increment front pointer
    		%logq.capacity;					//   if reached capacity - circle back
    logq.size = logq.size - 1;				// Decrement queue size
}


//==============================================================================
// FUNCTION log_settime()
//      - set current time with a second resolution
//==============================================================================
void log_time_set(uint16_t hr,uint8_t min,uint8_t sec)
{
	RTC_WaitForLastTask();						// Wait: last write operation finished
	RTC_SetCounter(								// Change current time
			((uint32_t)hr)*3600+				// Hours
			((uint32_t)min)*60+					// Minutes
			((uint32_t)sec));					// Seconds
	RTC_WaitForLastTask();						// Wait: last write operation finished
}


//==============================================================================
// FUNCTION log_gettime()
//      - gets current time (RTC second counter and TIM3 millisecond counter)
//==============================================================================
void log_time_get()
{
	uint32_t countms = TIM_GetCounter(TIM3);	// Get millisecond count
	uint32_t count = RTC_GetCounter();			// Get second count
	tm.hours 	= count / 3600;					// hours
	tm.minutes	= (count % 3600) / 60;			// minutes
	tm.seconds	= (count % 3600) % 60;			// seconds
	tm.mseconds = countms;						// milliseconds
}
