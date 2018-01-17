#ifndef PLAN_FSM_STRUCTURES_H
#define PLAN_FSM_STRUCTURES_H

// MEMORY LIMITS
#define MAX_STATES_PER_MODE     5
#define MAX_TRAN_PER_STATE      4
#define MAX_MODES_PER_FSM       4
#define MAX_GAINS               5

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

typedef struct
{
  int   cm;
  int   cs;
} fsm_tracker;

#endif //PLAN_FSM_STRUCTURES_H