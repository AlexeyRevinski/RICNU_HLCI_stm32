//Need:

//	Generate log strings when called from main or wherever else

#include "plan_log.h"


logtime 			logtm;
logqueue 			logq;
char	 			log_string	[LOG_STR_SIZE];
char	 			log_buffer	[LOG_BUFFER_SIZE];
char	 			tlog_buffer	[LOG_BUFFER_SIZE];
uint16_t 			log_bi = 0;							// Current buffer index
int					log_id = 0;
extern char 		st;
extern fsm_tracker	TR;
extern fsm			FSM_s;
extern ricnu_data 	rndata;

FIL logfile;
#define SZ_TBL 100

DWORD clmt[SZ_TBL];                    /* Cluster link map table buffer */


void log_init()
{
	for(int i=0;i<LOG_STR_SIZE;i++)	// Initialize log packet string
	{log_string[i]='-';}					// Set everything to contain space characters


	logtm.years = 2018;
	logtm.months = 23;
	logtm.days = 2;
	log_time_set((uint32_t)10,(uint8_t)38,(uint8_t)13);
	log_time_get();

	rndata.gx=0xB111;
	rndata.gy=0xB222;
	rndata.gz=0xB333;
	rndata.ax=0xA111;
	rndata.ay=0xA222;
	rndata.az=0xA333;
	rndata.em=0x55556666;
	rndata.ej=0x77778888;
	rndata.cu=0xFFFF;
	rndata.sc=0xABABABAB;
	rndata.s0=0x0111;
	rndata.s1=0x0222;
	rndata.s2=0x0333;
	rndata.s3=0x0444;
	rndata.s4=0x0555;
	rndata.s5=0x0666;

	log_q_init();						// Initialize log queue

	FRESULT fr;

	fr = f_open(&logfile, "ricnu.log", FA_WRITE | FA_OPEN_APPEND);
	if(fr!=FR_OK)
	{
		f_close(&logfile);
		return;
	}
    fr = f_lseek(&logfile, f_size(&logfile));               /* This is normal seek (cltbl is nulled on file open) */
    if(fr!=FR_OK)
    {
    	f_close(&logfile);
    	return;
    }
    logfile.cltbl = clmt;                      /* Enable fast seek function (cltbl != NULL) */
    clmt[0] = SZ_TBL;                      /* Set table size */
    fr = f_lseek(&logfile, CREATE_LINKMAP);     /* Create CLMT */
    if(fr!=FR_OK)
    {
    	f_close(&logfile);
    	return;
    }
}


void log_generate()
{
	log_gen_string();
	log_buf_append(log_string,LOG_STR_SIZE);
}


void log_gen_string()
{
	log_time_get();
	sprintf(log_string+LOG_STR_ID,		"%08X",log_id++);
	log_string[LOG_STR_HOUR-1]		=' ';
	sprintf(log_string+LOG_STR_HOUR,	"%02d",logtm.hours);
	log_string[LOG_STR_MINUTE-1] 	=':';						// ...HR:MN...
	sprintf(log_string+LOG_STR_MINUTE,	"%02d",logtm.minutes);
	log_string[LOG_STR_SECOND-1] 	=':';						// ...MN:SC...
	sprintf(log_string+LOG_STR_SECOND,	"%02d",logtm.seconds);
	log_string[LOG_STR_MSECOND-1] 	='.';						// ...SC:MSC...
	sprintf(log_string+LOG_STR_MSECOND,	"%03d",logtm.mseconds);
	log_string[LOG_STR_SYS_ST-1] 	=' ';
	sprintf(log_string+LOG_STR_SYS_ST,	&st);
	log_string[LOG_STR_FSM_M-1] 	=' ';
	sprintf(log_string+LOG_STR_FSM_M,	"%05d",FSM_s.m[TR.cm].id_self);
	log_string[LOG_STR_FSM_S-1] 	=' ';
	sprintf(log_string+LOG_STR_FSM_S,	"%05d",FSM_s.m[TR.cm].s[TR.cs].id_self);
	log_string[LOG_STR_FSM_T-1] 	=' ';
	sprintf(log_string+LOG_STR_FSM_T,	"-----");								// TODO put transition down
	log_string[LOG_STR_DAT_GX-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_GX,	"%04X",rndata.gx);
	log_string[LOG_STR_DAT_GY-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_GY,	"%04X",rndata.gy);
	log_string[LOG_STR_DAT_GZ-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_GZ,	"%04X",rndata.gz);
	log_string[LOG_STR_DAT_AX-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_AX,	"%04X",rndata.ax);
	log_string[LOG_STR_DAT_AY-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_AY,	"%04X",rndata.ay);
	log_string[LOG_STR_DAT_AZ-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_AZ,	"%04X",rndata.az);
	log_string[LOG_STR_DAT_EM-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_EM,	"%04X",(uint16_t)(rndata.em>>16));
	sprintf(log_string+LOG_STR_DAT_EM+4,"%04X",(uint16_t)(rndata.em));
	log_string[LOG_STR_DAT_EJ-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_EJ,	"%04X",(uint16_t)(rndata.ej>>16));
	sprintf(log_string+LOG_STR_DAT_EJ+4,"%04X",(uint16_t)(rndata.ej));
	log_string[LOG_STR_DAT_CU-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_CU,	"%04X",rndata.cu);
	log_string[LOG_STR_DAT_SC-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_SC,	"%04X",(uint16_t)(rndata.sc>>16));
	sprintf(log_string+LOG_STR_DAT_SC+4,"%04X",(uint16_t)(rndata.sc));
	log_string[LOG_STR_DAT_S0-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_S0,	"%03X",rndata.s0);
	log_string[LOG_STR_DAT_S1-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_S1,	"%03X",rndata.s1);
	log_string[LOG_STR_DAT_S2-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_S2,	"%03X",rndata.s2);
	log_string[LOG_STR_DAT_S3-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_S3,	"%03X",rndata.s3);
	log_string[LOG_STR_DAT_S4-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_S4,	"%03X",rndata.s4);
	log_string[LOG_STR_DAT_S5-1] 	=' ';
	sprintf(log_string+LOG_STR_DAT_S5,	"%03X",rndata.s5);
	log_string[LOG_STR_SIZE-1]		='\n';
}

//==============================================================================
// FUNCTION log_buf_append()
//      - appends string to the buffer string based on queue and available space
//==============================================================================
int log_buf_append(char* str, uint16_t len)
{
	int bnd = 0;
	if(log_q_is_full()) return 0;		// If queue is full, no space to write
	if(LOG_BUFFER_SIZE>(log_bi+len-1))	// If string will fit in the array
	{
		char* temp=log_buffer+log_bi; 		// Make temporary pointer
		int i=0;							// Loop index
		for(;i<len;i++){temp[i] = str[i];}	// Copy string to buffer
	}
	else								// If string will not fit in the array --> last sector
	{
		if(logq.size<logq.capacity-1)			// If at least the first sector of buffer
		{										//   can be overwritten
			char* temp=log_buffer+log_bi; 			// Make temporary pointer
			int i = 0,j = 0;						// Loop indices
			for(;i<len;i++)							// Copy string to buffer -->
			{
				if((i+log_bi)<LOG_BUFFER_SIZE)			// If next char still fits -->
				{
					temp[i] = str[i];						// Copy character
					j++;									// Increment temp index
				}
				else									// Once hit end of array -->
				{
					log_buffer[i-j] = str[i];				// Restart at beginning of array
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
	if (log_bi>LOG_BUFFER_SIZE-1)		// If overflown -->
	{
		log_bi%=LOG_BUFFER_SIZE;			// Overflow index
	}
	return 1;
}



//==============================================================================
// FUNCTION log_buf_clear()
//      - clears entire log buffer
//==============================================================================
void log_buf_clear()
{
	int i = LOG_BUFFER_SIZE;					// Set index to LOG_BUFFER_SIZE
	do log_buffer[--i]=0; while(i);				// While going through buffer, clear
}

void log_str_clear()
{
	int i = LOG_STR_SIZE;					// Set index to LOG_BUFFER_SIZE
	do log_string[--i]=0; while(i);				// While going through buffer, clear
}



//==============================================================================
// FUNCTION log_appendfile()
//		- if a sector of data is available, writes that sector to memory card as
//			an addition to information already available in the log file
//==============================================================================
int log_file_append()
{
	if(log_q_is_empty()) return 1;				// If queue empty, nothing to write

	FRESULT fr; 						// Result and file variables
	UINT bw = 0; UINT* bwp = &bw;

	fr = f_lseek(&logfile, f_size(&logfile));
	if (fr != FR_OK){ f_close(&logfile); return 1;}					// If failed - disk error
	f_write (&logfile,log_buffer+(logq.front*LOG_SECTOR_SIZE),
			LOG_SECTOR_SIZE,bwp);											// TODO buff here is a const void*
	f_sync(&logfile);
	log_q_deq();
	return 0;									// Return success
}


//==============================================================================
// FUNCTION log_q_init()
//      - initializes queue
//		- sets capacity to fifo size defined in plan_log.h
//		- sets front of the queue at the start of buffer string
//		- sets rear at last sector in buffer string. When first sector of the
//			buffer string is filled and enqueued, rear will point to start of
//			buffer string
//==============================================================================
void log_q_init()
{
	logq.capacity = LOG_FIFO_SIZE;				// Capacity is num of sectors
	logq.front = logq.size = 0;					// Queue is empty
	logq.rear = logq.capacity-1;				// Rear is one less than capacity
}												//  (once a sector is enqueued,
												//  it will point to its start,
												//  which is at address 0)

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
void log_time_set(uint32_t hr,uint8_t min,uint8_t sec)
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
	logtm.mseconds	= TIM_GetCounter(TIM3);	// Get millisecond count
	uint32_t count	= RTC_GetCounter();				// Get second count
	logtm.hours		= count / 3600;					// hours
	logtm.minutes	= (count % 3600) / 60;			// minutes
	logtm.seconds	= (count % 3600) % 60;			// seconds
}
