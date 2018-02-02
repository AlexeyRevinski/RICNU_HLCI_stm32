#ifndef PLAN_FSM_BUILDER_H
#define PLAN_FSM_BUILDER_H

#include "plan_include.h"

#define FILENAME "fsm.json"
#define MAX_FSM_FILE_LENGTH 8192
#define STACK_SIZE 50
#define MAX_STATES_PER_MODE     5
#define MAX_TRAN_PER_STATE      4
#define MAX_MODES_PER_FSM       4
#define MAX_GAINS               5

// TYPE OF CONTROL ENUM
typedef enum
{
  CTRL_NON = 0,
  CTRL_POS = 1,
  CTRL_CUR = 2,
  CTRL_IMP = 3,
} fsm_ctrl;

// XML MEMBER TYPE ENUM
typedef enum
{
  TAG,
  ATT
} xmltype;

// ERROR CODES
typedef enum
{
  FB_ERR = 0,
  FB_OK = 1,
} errcode;
 
// STACK STRUCTURE
typedef struct {
   int  arr[STACK_SIZE];
   int  top;
}fsm_stack;

// ENUMS
typedef enum {MR,LS,ME,LE,EQ} fsm_condition;
typedef enum {AX,AY,AZ,GX,GY,GZ,EM,EJ,CM,STRAIN} fsm_channel;

// OTHER TYPES
typedef int fsm_id;
typedef float fsm_gain;

// PERSON STRUCT
typedef struct
{
  char fn[20];
  char ln[20];
  int id;
} person;

// TRANSITION STRUCT
typedef struct
{
  fsm_id        id_self;
  fsm_id        id_next;
  int           chan;
  int           cond;
  float         thres;
} fsm_tran;

// STATE STRUCT
typedef struct
{
  fsm_id        id_self;
  int           ctrl;
  fsm_gain      g[MAX_GAINS];
  int           num_t;
  fsm_tran      t[MAX_TRAN_PER_STATE];
} fsm_state;

// MODE STRUCT
typedef struct
{
  fsm_id        id_self;
  fsm_id        id_defstate;
  int           num_s;
  fsm_state     s[MAX_STATES_PER_MODE];
} fsm_mode;

// MAIN FSM STRUCT
typedef struct
{
  fsm_id        id_defmode;
  person        user;
  person        patient;
  int           num_m;
  fsm_mode      m[MAX_MODES_PER_FSM];
} fsm;

// TRACKER STRUCT
typedef struct
{
  int   cm;
  int   cs;
} fsm_tracker;



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
