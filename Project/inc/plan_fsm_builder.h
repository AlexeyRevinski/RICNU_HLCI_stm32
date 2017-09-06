#ifndef PLAN_FSM_BUILDER_H
#define PLAN_FSM_BUILDER_H

#include "plan_include.h"

#define FILENAME "fsm.json"
#define MAX_FSM_FILE_LENGTH 8192


typedef enum
{
  CTRL_POS = 1,
  CTRL_CUR = 2,
  CTRL_IMP = 3,
} ctrl;

typedef enum
{
  TAG,
  ATT
} xmltype;

// ERROR CODES
typedef enum
{
  FB_NOT_FOUND = 0,
  FB_OK = 1,
  FB_ERR_BUILD = 1,
  FB_ERR_MEM = 2,
} errcode;

#define STACK_SIZE 50
 
// STACK STRUCTURE
typedef struct {
   int  arr[STACK_SIZE];
   int  top;
}fsm_stack;

// FUNCTION PROTOTYPES
errcode         fsm_build(void);
int             fsm_populate_structure(void);
int             fsm_move_up(fsm_stack*, int);
int             fsm_move_in(fsm_stack*,xmltype,const char*,int);
int             fsm_move_to(fsm_stack*,xmltype,const char*,int);
int             fsm_stack_init(fsm_stack*);
int             fsm_stack_put(fsm_stack*,int);
int             fsm_stack_pop(fsm_stack*);
int             fsm_stack_get(fsm_stack*);
int             fsm_get_elm(int,int);
int             fsm_get_att(int,const char*);
int             fsm_get_tag(int,const char*,int);
int             fsm_get_tkn(int,const char*,xmltype,int);
int             fsm_get_i(fsm_stack*,int*);
float           fsm_get_f(fsm_stack*,float*);
int             fsm_get_str(fsm_stack*,char*);
uint8_t         fsm_is_att(int);
uint8_t         fsm_is_tag(int);
uint8_t         fsm_is_str(int);
uint8_t         fsm_is_obj(int);
uint8_t         fsm_is_arr(int);
uint8_t         fsm_str_cpy(int,char*);
uint8_t         fsm_str_cmp(int,const char*);
errcode         fsm_mem_free(void);

#endif //PLAN_FSM_BUILDER_H
