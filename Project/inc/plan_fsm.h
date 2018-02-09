#ifndef PLAN_FSM_H
#define PLAN_FSM_H

#include "plan_include.h"

#define FILENAME "fsm.json"
#define MAX_FSM_FILE_LENGTH     8192
#define STACK_SIZE              50
#define MAX_STATES_PER_MODE     5
#define MAX_TRAN_PER_STATE      4
#define MAX_MODES_PER_FSM       4
#define MAX_GAINS               5


 


// ENUMS
typedef enum {MR,LS,ME,LE,EQ} fsm_condition;

typedef enum {AX,AY,AZ,GX,GY,GZ,EM,EJ,CM,STRAIN} fsm_channel;

typedef enum
{
  CTRL_NON = 0,
  CTRL_POS = 1,
  CTRL_CUR = 2,
  CTRL_IMP = 3,
} fsm_ctrl;

typedef enum
{
  TAG,                                          // Type of XML data: XML tag
  ATT                                           // Type of XML data: XML attribute
} xmltype;

typedef enum
{
  FB_ERR = 0,                                   // FSM Build error
  FB_OK = 1,                                    // FSM Build ok
} errcode;

// OTHER TYPES
typedef int fsm_id;                             // ID type
typedef float fsm_gain;                         // Gain type

// STRUCTS
typedef struct {
   int  arr[STACK_SIZE];                        // Stack array
   int  top;                                    // Pointer to top of stack array
}fsm_stack;

typedef struct
{
  char fn[20];                                  // First name string
  char ln[20];                                  // Last name string
  int id;                                       // Person ID
} person;

typedef struct
{
  fsm_id        id_self;                        // Transition ID
  fsm_id        id_next;                        // Next state ID
  int           chan;                           // Channel to analyze
  int           cond;                           // Conditional operator
  float         thres;                          // Magnitude threshold
} fsm_tran;

typedef struct
{
  fsm_id        id_self;                        // State ID
  int           ctrl;                           // Control type
  fsm_gain      g[MAX_GAINS];                   // Control gains
  int           num_t;                          // Number of transitions in this state
  fsm_tran      t[MAX_TRAN_PER_STATE];          // Transitions in this state
} fsm_state;

typedef struct
{
  fsm_id        id_self;                        // Mode ID
  fsm_id        id_defstate;                    // Default State ID
  int           num_s;                          // Number of state in this mode
  fsm_state     s[MAX_STATES_PER_MODE];         // States in this mode
} fsm_mode;

typedef struct
{
  fsm_id        id_defmode;                     // Default Mode ID
  person        user;                           // User structure
  person        patient;                        // Patient structure
  int           num_m;                          // Number of modes in this fsm
  fsm_mode      m[MAX_MODES_PER_FSM];           // Modes in this fsm
} fsm;

typedef struct
{
  int   cm;                                     // Current mode
  int   cs;                                     // Current state
} fsm_tracker;

// FUNCTION PROTOTYPES
void            fsm_update(void);
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
