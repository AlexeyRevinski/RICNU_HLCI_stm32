#include "plan_fsm_builder.h"

void build_fsm(void)
{
  // Get FSM file string  ======================================================
  FATFS fs; FIL fil; UINT br;                   // Declare FatFs variables
  f_mount(&fs,"", 0);                           // Mount the default drive
  f_open(&fil,FILENAME, FA_READ);               // Open fsm file
  char *fsm_string = NULL;                      // Initialize fsm string ptr
  int str_size = fil.obj.objsize;               // Get file size
  if(str_size>MAX_FSM_FILE_LENGTH){GPIO_SetBits(LEDx_PORT,LED2_PIN); return;} // TODO ERROR STATE
  fsm_string = malloc(sizeof(*fsm_string)*str_size); // Allocate memory for str
  f_read(&fil,fsm_string,str_size,&br);         // Populate the string
  f_close(&fil);                                // Close the file
  f_mount(0,"", 0);                             // Unmount the default drive
  
  // Parse FSM string (jsmn tokens)  ===========================================
  jsmn_parser parser;                           // Declare parser
  jsmn_init(&parser);                           // Initialize parser
  int num_tkns =                                // Get num tokens needed
    jsmn_parse(&parser, fsm_string, str_size, NULL, NULL);
  jsmn_init(&parser);                           // Reset parser
  jsmntok_t *tokens =                           // Allocate mem for tokens array
    (jsmntok_t *)malloc(sizeof(jsmntok_t)*num_tkns);
  jsmn_parse(&parser, fsm_string, str_size, tokens, num_tkns); // Parse string
  
  // Build FSM  ================================================================
  // TODO implement build code
  
  
  // Free Memory  ==============================================================
  free(fsm_string);                             // Free the string memory
  free(tokens);
}