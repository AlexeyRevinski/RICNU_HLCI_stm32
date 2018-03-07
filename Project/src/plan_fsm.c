#include "plan_fsm.h"

// TAG STRINGS  ================================================================
const char* tag_root            = "ricnuplan";
const char* tag_user            = "user";
const char* tag_patient         = "patient";
const char* tag_test            = "test";
const char* tag_fsm             = "fsm";
const char* tag_mode            = "mode";
const char* tag_state           = "state";
const char* tag_ctrl            = "control";
const char* tag_ctrl_non        = "control_non";
const char* tag_ctrl_pos        = "control_pos";
const char* tag_ctrl_cur        = "control_cur";
const char* tag_ctrl_imp        = "control_imp";
const char* tag_k               = "k";
const char* tag_b               = "b";
const char* tag_e               = "e";
const char* tag_kp              = "kp";
const char* tag_ki              = "ki";
const char* tag_trans           = "transitions";
const char* tag_tran            = "transition";
const char* tag_nxt_st          = "next_state";
const char* tag_event           = "event";
const char* tag_func_const      = "function_constant";
const char* tag_constant        = "constant_value";

// ATTRIBUTE STRINGS  ==========================================================
const char* att_fname           = "firstname";
const char* att_lname           = "lastname";
const char* att_id              = "id";
const char* att_tag             = "tag";
const char* att_chan            = "channel";
const char* att_func            = "function";
const char* att_val             = "value";
const char* att_dst             = "defstate";
const char* att_dmd             = "defmode";

// VALUE STRINGS  ==============================================================
const char* val_ax              = "ax";
const char* val_ay              = "ay";
const char* val_az              = "az";
const char* val_gx              = "gx";
const char* val_gy              = "gy";
const char* val_gz              = "gz";
const char* val_em              = "em";
const char* val_ej              = "ej";
const char* val_cm              = "cm";
const char* val_fx				= "fx";
const char* val_fy				= "fy";
const char* val_fz				= "fz";
const char* val_mx				= "mx";
const char* val_my				= "my";
const char* val_mz				= "mz";

const char* val_mr              = "mr";
const char* val_ls              = "ls";
const char* val_me              = "me";
const char* val_le              = "le";
const char* val_eq              = "eq";

// GLOBAL VARIABLES  ===========================================================
static  fsm_stack       fst;
static  fsm_stack       *fstp   = &fst; // Initialize ptr to fsm ptr stack

fsm             		FSM_s;
fsm             		*FSM    = &FSM_s; // Initialize pointer to fsm structure
fsm_tracker     		TR;

extern ricnu_data rndata;

int32_t setpoint = -1;
int16_t g0=-1,g1=-1,g2=-1,g3=-1;

// HEAP POINTERS  ==============================================================
static  jsmntok_t       *tkns   = NULL; // Initialize tokens pointer
static  char            *fstr   = NULL; // Initialize fsm string pointer

uint8_t	g_offset = 0;

uint32_t new_transition = 0;

extern flexsea_ctrl fc;

extern int32_t imucal[];

//==============================================================================
// FUNCTION fsm_build
//      - builds fsm from a JSON file stored on the SD card
//==============================================================================
errcode fsm_build(void)
{  
  // Select SD Card for SPI communication  =====================================
  GPIO_ResetBits(GPIO_SD_NSS_PORT,GPIO_SD_NSS_PIN);                             // Not needed??
  
  // Get FSM file string  ======================================================
  FIL fil; UINT br;                   			// Declare FatFs variables

  if(f_open(&fil,FILENAME, FA_READ)!=FR_OK) return FB_ERR;               // Open fsm file
  int str_size = fil.obj.objsize;               // Get file size
  if(str_size>MAX_FSM_FILE_LENGTH){return FB_ERR;} // If too large ==> error
  fstr = (char *)malloc(sizeof(*fstr)*str_size);// Allocate memory for fstr
  if(!fstr){fsm_mem_free(); return FB_ERR;}     // If didn't allocate ==> error
  f_read(&fil,fstr,str_size,&br);               // Populate the string
  f_close(&fil);                                // Close the file
  //f_mount(0,"",0);                              // Unmount the default drive
  
  // Parse FSM string to get jsmn tokens  ======================================
  jsmn_parser parser;                           // Declare parser
  jsmn_init(&parser);                           // Initialize parser
  int num_tkns =                                // Get num tokens needed
    jsmn_parse(&parser, fstr, str_size, NULL, (unsigned int)NULL);
  jsmn_init(&parser);                           // Reset parser
  tkns =                                        // Allocate mem for tokens array
    (jsmntok_t *)malloc(sizeof(jsmntok_t)*num_tkns);
  if(!tkns){fsm_mem_free();return FB_ERR;}      // If didn't allocate ==> error
  jsmn_parse(&parser,fstr,str_size,tkns,num_tkns); // Parse string 
  
  // Populate FSM structure from FSM string and jsmn tokens  ===================
  if(!fsm_populate_structure()) {fsm_mem_free();return FB_ERR;}
  else                          {fsm_mem_free();return FB_OK;}
}

//==============================================================================
// FUNCTION fsm_populate_structure
//      - goes through the file string and populates the fsm structure with 
//        data found in the JSON file objects
//      - path-based; keeps track of current and prev objects using a stack
//==============================================================================
int fsm_populate_structure(void)
{
  //////////////////////////////////////////////////////////////////////////////
  int mode_ptr=1,stat_ptr=1,tran_ptr=1,nm=0,ns=0,nt=0;
  FSM->num_m = 0;
  for (int i=0;i<MAX_MODES_PER_FSM;i++){FSM->m[i].num_s = 0;
    for (int j=0;j<MAX_STATES_PER_MODE;j++){FSM->m[i].s[j].num_t = 0;}}
  ////////////////////////////////////////////////////////////////////////////// 
  
  // Go to root
  fsm_stack_init(fstp);                                                         // root
  
  // Populate "user"  ==========================================================
  if(!fsm_move_in(fstp,TAG,tag_user,1))         return FB_ERR;                          // root/user
  if(!fsm_move_in(fstp,ATT,att_fname,1))        return FB_ERR;                                            // root/user/firstname
  if(!fsm_get_str(fstp,FSM->user.fn))           return FB_ERR;
  if(!fsm_move_to(fstp,ATT,att_lname,1))        return FB_ERR;                                            // root/user/lastname
  if(!fsm_get_str(fstp,FSM->user.ln))           return FB_ERR;
  if(!fsm_move_to(fstp,ATT,att_id,1))           return FB_ERR;                                               // root/user/id
  if(!fsm_get_i(fstp,&FSM->user.id))            return FB_ERR;
  if(!fsm_move_up(fstp,2))                      return FB_ERR;                                                          // root
      
  // Populate "patient"  =======================================================
  if(!fsm_move_in(fstp,TAG,tag_patient,1))      return FB_ERR;                                          // root/patient
  if(!fsm_move_in(fstp,ATT,att_fname,1))        return FB_ERR;                                            // root/patient/firstname
  if(!fsm_get_str(fstp,FSM->patient.fn))        return FB_ERR;
  if(!fsm_move_to(fstp,ATT,att_lname,1))        return FB_ERR;                                            // root/patient/lastname
  if(!fsm_get_str(fstp,FSM->patient.ln))        return FB_ERR;
  if(!fsm_move_to(fstp,ATT,att_id,1))           return FB_ERR;                                               // root/patient/id
  if(!fsm_get_i(fstp,&FSM->patient.id))         return FB_ERR;
  if(!fsm_move_up(fstp,2))                      return FB_ERR;                                                          // root
  
  // Populate the state machine  ===============================================
  if(!fsm_move_in(fstp,TAG,tag_fsm,1))          return FB_ERR;                                              // root/fsm
  
  // Get default mode id 
  if(!fsm_move_in(fstp,ATT,att_dmd,1))          return FB_ERR;                                              // root/fsm/defmode
  if(!fsm_get_i(fstp,&FSM->id_defmode))         return FB_ERR;
  
  // Move up to root/fsm
  if(!fsm_move_up(fstp,1))                      return FB_ERR;                                                          // root/fsm
  
  // Populate each mode  =======================================================
  while(mode_ptr)       // While successfully getting new modes
  {
    // Get number of modes encountered; reset number of states to 0
    nm = FSM->num_m; ns = 0;
    // Get new mode
    mode_ptr = fsm_move_in(fstp,TAG,tag_mode,nm+1);                             // root/fsm/mode
    if(mode_ptr)        // If new mode found ==>
    {
      // Get mode id attribute
      fsm_move_in(fstp,ATT,att_id,1);                                           // root/fsm/mode/id
      fsm_get_i(fstp,&FSM->m[nm].id_self);
      
      // Get mode name attribute
      fsm_move_to(fstp,ATT,att_tag,1);                                          // root/fsm/mode/tag
      //---------
      
      // Get default state id
      fsm_move_to(fstp,ATT,att_dst,1);
      fsm_get_i(fstp,&FSM->m[nm].id_defstate);
      
      // Move up to mode level
      fsm_move_up(fstp,1);                                                      // root/fsm/mode
      
      // Populate each state  ==================================================
      while(stat_ptr)           // While successfully getting new states
      {
        // Get number of states encountered; reset number of transitions to 0
        ns = FSM->m[nm].num_s; nt = 0;
        // Get new state
        stat_ptr = fsm_move_in(fstp,TAG,tag_state,ns+1);                        // root/fsm/mode/state
        if(stat_ptr)         // If new state found ==>
        {
          // Get state id
          fsm_move_in(fstp,ATT,att_id,1);                                       // root/fsm/mode/state/id
          fsm_get_i(fstp,&FSM->m[nm].s[ns].id_self);
          
          // Get state name
          fsm_move_to(fstp,ATT,att_tag,1);                                      // root/fsm/mode/state/tag
          //---------
          
          // Move to control tag
          fsm_move_to(fstp,TAG,tag_ctrl,1);                                     // root/fsm/mode/state/control
          
          
          // Move to specific control tag                                       //TODO IMPLEMENT "NO CONTROL" ROUTINE
          if (fsm_move_in(fstp,TAG,tag_ctrl_non,1))                             // root/fsm/mode/state/control/control_non
          {
            // Set control type to position
            FSM->m[nm].s[ns].ctrl = CTRL_NON;
            // get e, kp, ki
          }
          if (fsm_move_in(fstp,TAG,tag_ctrl_pos,1))                             // root/fsm/mode/state/control/control_pos
          {
            // Set control type to position
            FSM->m[nm].s[ns].ctrl = CTRL_POS;
            // get e, kp, ki
          }
          else if (fsm_move_in(fstp,TAG,tag_ctrl_cur,1))                        // root/fsm/mode/state/control/control_cur
          {
            // Set control type to position
            FSM->m[nm].s[ns].ctrl = CTRL_CUR;
            // get e, kp, ki
          }
          else if (fsm_move_in(fstp,TAG,tag_ctrl_imp,1))                        // root/fsm/mode/state/control/control_imp
          {
            // Set control type to impedance control
            FSM->m[nm].s[ns].ctrl = CTRL_IMP;
            // Get k
            fsm_move_in(fstp,TAG,tag_k,1);
            fsm_move_in(fstp,TAG,tag_func_const,1);
            fsm_move_in(fstp,TAG,tag_constant,1);
            fsm_move_in(fstp,ATT,att_val,1);
            fsm_get_f(fstp,&FSM->m[nm].s[ns].g[0]);
            fsm_move_up(fstp,4);
            // Get b
            fsm_move_in(fstp,TAG,tag_b,1);
            fsm_move_in(fstp,TAG,tag_func_const,1);
            fsm_move_in(fstp,TAG,tag_constant,1);
            fsm_move_in(fstp,ATT,att_val,1);
            fsm_get_f(fstp,&FSM->m[nm].s[ns].g[1]);
            fsm_move_up(fstp,4);
            // Get e
            fsm_move_in(fstp,TAG,tag_e,1);
            fsm_move_in(fstp,TAG,tag_func_const,1);
            fsm_move_in(fstp,TAG,tag_constant,1);
            fsm_move_in(fstp,ATT,att_val,1);
            fsm_get_f(fstp,&FSM->m[nm].s[ns].g[2]);
            fsm_move_up(fstp,4);
            // Get kp
            fsm_move_in(fstp,TAG,tag_kp,1);
            fsm_move_in(fstp,TAG,tag_func_const,1);
            fsm_move_in(fstp,TAG,tag_constant,1);
            fsm_move_in(fstp,ATT,att_val,1);
            fsm_get_f(fstp,&FSM->m[nm].s[ns].g[3]);
            fsm_move_up(fstp,4);
            // Get ki
            fsm_move_in(fstp,TAG,tag_ki,1);
            fsm_move_in(fstp,TAG,tag_func_const,1);
            fsm_move_in(fstp,TAG,tag_constant,1);
            fsm_move_in(fstp,ATT,att_val,1);
            fsm_get_f(fstp,&FSM->m[nm].s[ns].g[4]);
            fsm_move_up(fstp,4);
          }
          // Move up to state level
          fsm_move_up(fstp,2);                                                  // root/fsm/mode/state
          
          // Populate each transition  =========================================
          while(tran_ptr)       // While successfully getting new transitions
          {
            // Get number of trans encountered
            nt = FSM->m[nm].s[ns].num_t;
            // Get new transitions
            tran_ptr = fsm_move_in(fstp,TAG,tag_tran,nt+1);                     // root/fsm/mode/state/transition
            if(tran_ptr)        // If new mode found ==>
            {
              // Get transition id
              fsm_move_in(fstp,ATT,att_id,1);                                   // root/fsm/mode/state/transition/id
              fsm_get_i(fstp,&FSM->m[nm].s[ns].t[nt].id_self);
              
              // Get next state id
              fsm_move_to(fstp,TAG,tag_nxt_st,1);                               // root/fsm/mode/state/transition/next_state
              fsm_move_in(fstp,ATT,att_id,1);                                   // root/fsm/mode/state/transition/next_state/id
              fsm_get_i(fstp,&FSM->m[nm].s[ns].t[nt].id_next);
              fsm_move_up(fstp,1);                                              // root/fsm/mode/state/transition/next_state
              
              // Get event information
              fsm_move_to(fstp,TAG,tag_event,1);                                // root/fsm/mode/state/transition/event
              fsm_move_in(fstp,ATT,att_chan,1);                                 // root/fsm/mode/state/transition/next_state/channel
              if     (fsm_str_cmp(fstp->arr[fstp->top]+1,val_ax))FSM->m[nm].s[ns].t[nt].chan = AX;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_ay))FSM->m[nm].s[ns].t[nt].chan = AY;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_az))FSM->m[nm].s[ns].t[nt].chan = AZ;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_gx))FSM->m[nm].s[ns].t[nt].chan = GX;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_gy))FSM->m[nm].s[ns].t[nt].chan = GY;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_gz))FSM->m[nm].s[ns].t[nt].chan = GZ;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_em))FSM->m[nm].s[ns].t[nt].chan = EM;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_ej))FSM->m[nm].s[ns].t[nt].chan = EJ;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_cm))FSM->m[nm].s[ns].t[nt].chan = CM;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_fx))FSM->m[nm].s[ns].t[nt].chan = FX;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_fy))FSM->m[nm].s[ns].t[nt].chan = FY;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_fz))FSM->m[nm].s[ns].t[nt].chan = FZ;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_mx))FSM->m[nm].s[ns].t[nt].chan = MX;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_my))FSM->m[nm].s[ns].t[nt].chan = MY;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_mz))FSM->m[nm].s[ns].t[nt].chan = MZ;
              fsm_move_to(fstp,ATT,att_func,1);                                 // root/fsm/mode/state/transition/next_state/function
              if     (fsm_str_cmp(fstp->arr[fstp->top]+1,val_mr))FSM->m[nm].s[ns].t[nt].cond = MR;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_ls))FSM->m[nm].s[ns].t[nt].cond = LS;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_me))FSM->m[nm].s[ns].t[nt].cond = ME;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_le))FSM->m[nm].s[ns].t[nt].cond = LE;
              else if(fsm_str_cmp(fstp->arr[fstp->top]+1,val_eq))FSM->m[nm].s[ns].t[nt].cond = EQ;
              fsm_move_to(fstp,ATT,att_val,1);                                  // root/fsm/mode/state/transition/next_state/value
              fsm_get_f(fstp,&FSM->m[nm].s[ns].t[nt].thres);
              fsm_move_up(fstp,1);                                              // root/fsm/mode/state/transition/next_state
              
              // Move up to transition level
              fsm_move_up(fstp,1);                                              // root/fsm/mode/state/transition
              
              // Increment number of modes encountered
              FSM->m[nm].s[ns].num_t = ++nt;
              // Move up to state level
              fsm_move_up(fstp,1);                                              // root/fsm/mode/state
            }
          }
          // Reset tran_ptr
          tran_ptr = 1;
          // Increment number of modes encountered
          FSM->m[nm].num_s = ++ns;
          // Move back up to root/fsm/mode
          fsm_move_up(fstp,1);                                                  // root/fsm/mode
        }
      }
      // Reset stat_ptr
      stat_ptr = 1;
      // Increment number of modes encountered
      FSM->num_m = ++nm;
      // Move back up to root/fsm
      fsm_move_up(fstp,1);                                                      // root/fsm
    }
  }
  
  // Set default state of default mode as starting state
  TR.cm = 0; TR.cs = 0;
  for(int mcnt=0;mcnt<(FSM->num_m);mcnt++)
  {
    for(int scnt=0;scnt<(FSM->m[mcnt].num_s);scnt++)
    {
      if(FSM->m[mcnt].id_self==FSM->id_defmode&&
         FSM->m[mcnt].s[scnt].id_self==FSM->m[mcnt].id_defstate)
      {
        TR.cm = mcnt;
        TR.cs = scnt;
        break;
      }
    }
    if(TR.cm&&TR.cs) break;
  }
  return FB_OK;
}

//==============================================================================        // Perhaps will move to fsm
// FUNCTION update_fsm()
//      - processes data from Manage
//      - catches events based on current state
//      - changes current state and mode
//==============================================================================
void fsm_update(void)
{
  float data = 0, threshold = -1;
  int r = -1, condition = -1, prev_mode = -1, prev_state = -1;
  
  // Catch event  ==============================================================
  // Cycle through transitions of current state
  for(int tcnt=0;tcnt<(FSM->m[TR.cm].s[TR.cs].num_t);tcnt++)
  {
	  // Get transition's channel value to analyze
	  switch(FSM->m[TR.cm].s[TR.cs].t[tcnt].chan)
	  {
	  case GX: data=rndata.gx/(double)164;      break;
	  case GY: data=rndata.gy/(double)164;      break;
	  case GZ: data=rndata.gz/(double)164;      break;

	  case AX: data=rndata.ax/(double)8192;    break;
      case AY: data=rndata.ay/(double)8192;    break;
      case AZ: data=rndata.az/(double)8192;    break;

      case EM: data=rndata.em*0.021973;          break;
      case EJ: data=rndata.ej*0.021973;          break;
      case CM: data=rndata.cu;                 break;

      	  case FX: data=rndata.st[0]*0.1;          break;
      	  case FY: data=rndata.st[1]*0.1;          break;
      	  case FZ: data=rndata.st[2]*0.1;           break;
      	  case MX: data=rndata.st[3]*0.001;          break;
		  case MY: data=rndata.st[4]*0.001;          break;
		  case MZ: data=rndata.st[5]*0.001;         break;


      }
      condition = FSM->m[TR.cm].s[TR.cs].t[tcnt].cond;
      threshold = FSM->m[TR.cm].s[TR.cs].t[tcnt].thres;
      
      // If relationship between data and transition's
      //        threshold matches condition ==> generate event
      if((condition==MR&&data>threshold)||
         (condition==LS&&data<threshold)||
         (condition==ME&&data>=threshold)||
         (condition==LE&&data<=threshold)||
         (condition==EQ&&data==threshold))
      {
        r = tcnt;               // Save this transition number
        //new_transition = r;
        new_transition = FSM->m[TR.cm].s[TR.cs].t[tcnt].id_self;
        break;
      }
  }
  if(r>=0) // If caught an event (transition number is not -1)
  {
    // Save previous mode and state
    prev_mode = TR.cm;
    prev_state = TR.cs;
    
    // Cycle through modes 
    for(int mcnt=0;mcnt<(FSM->num_m);mcnt++)
    {
      // Cycle through states to find the next state based on event id
      for(int scnt=0;scnt<(FSM->m[mcnt].num_s);scnt++)
      {
        // If this state matches the next state id in the caught event ==>
        if(FSM->m[mcnt].s[scnt].id_self==
           FSM->m[TR.cm].s[TR.cs].t[r].id_next)
        {
          TR.cm = mcnt; // Go to this state (save mode index and state index)
          TR.cs = scnt;
          break;        // Stop cycling through states
        }
      }
      // If went to next state, stop cycling through modes
      if(!(TR.cs==prev_state&&TR.cm==prev_mode)) break;
    }
  }

  switch(FSM->m[TR.cm].s[TR.cs].ctrl)
  {
  case CTRL_NON:
	  fc.control = CTRL_NONE;
    break;
  case CTRL_POS:
	  fc.control = CTRL_POSITION;
    break;
  case CTRL_CUR:
	  fc.control = CTRL_CURRENT;
    break;
  case CTRL_IMP:
    fc.control = CTRL_IMPEDANCE;
    fc.setpoint = ((uint32_t)(FSM->m[TR.cm].s[TR.cs].g[2]+(imucal[6]*0.021973)))*45;
    fc.g0 = (uint16_t)FSM->m[TR.cm].s[TR.cs].g[0];
    fc.g1 = (uint16_t)FSM->m[TR.cm].s[TR.cs].g[1];
    fc.g2 = (uint16_t)FSM->m[TR.cm].s[TR.cs].g[3];
    fc.g3 = (uint16_t)FSM->m[TR.cm].s[TR.cs].g[4];
    break;
  }
  
  
  if(TR.cs==1)
  {
    LED_state(LED_RED,ON,CON);
    LED_state(LED_BLU,OFF,CON);
  }
  else if(TR.cs==2)
  {
    LED_state(LED_RED,OFF,CON);
    LED_state(LED_BLU,ON,CON);
  }
  else
  {
    LED_state(LED_RED,OFF,CON);
    LED_state(LED_BLU,OFF,CON);
  }
}

//==============================================================================
// FUNCTION fsm_move_up
//      - moves pointer up to parent of current object
//==============================================================================
int fsm_move_up(fsm_stack* s, int numgen)
{
  int ptr;
  // Move up a generation a specified number of times
  for(int i=0;i<numgen;i++)
  {
    // Move up the stack
    fsm_stack_pop(s);
    // Get value at that position
    ptr = s->arr[s->top];
  }
  return ptr;
}

//==============================================================================
// FUNCTION fsm_move_in
//      - moves pointer down to child of current object
//==============================================================================
int fsm_move_in(fsm_stack* s, xmltype type, const char *str, int inst)
{
  int ptr = 1;
  // If tag, get tag inside current head object
  if(type==TAG){ptr = fsm_get_tag(s->arr[s->top],str,inst);}
  // If attribute, get attribute inside current head object
  else if (type==ATT){ptr = fsm_get_att(s->arr[s->top],str);}
  // If unsuccessful, return error
  if(!ptr){return FB_ERR;}
  // Add pointer to stack
  fsm_stack_put(s, ptr);
  return ptr;
}

//==============================================================================
// FUNCTION fsm_move_to
//      - moves pointer to a sibling of current object
//==============================================================================
int fsm_move_to(fsm_stack* s, xmltype type, const char *str, int inst)
{
  // Move up to parent of given object
  fsm_move_up(s,1);
  // Move down to child of parent of given object
  return fsm_move_in(s,type,str,inst);
}

//==============================================================================
// FUNCTION fsm_stack_init
//      - initializes a pointer stack
//==============================================================================
int fsm_stack_init(fsm_stack* s)
{
  // Initialize first array value to 1 (root)
  s->arr[0] = 1;
  // Initialize head to 0 (to arr[0], so to root)
  s->top = 0;
  return 1;
}

//==============================================================================
// FUNCTION fsm_stack_put
//      - puts an element on top of the stack
//==============================================================================
int fsm_stack_put(fsm_stack* s, int ptr)
{
  // If not enough space, return
  if (s->top == STACK_SIZE - 1) {return -1;}
  // Point head to new value
  s->top++;
  // Populate new value
  s->arr[s->top] = ptr;
  // Return new value
  return ptr;
}
 
//==============================================================================
// FUNCTION fsm_stack_pop
//      - pops an element from the stack
//==============================================================================
int  fsm_stack_pop(fsm_stack* s)
{
  // If head is invalid, return
  if (s->top == -1) {return 0;}
  // Get value at head
  int ptr = fsm_stack_get(s);
  // Point head to previous
  s->top--;
  // Return value just popped
  return ptr;
}

//==============================================================================
// FUNCTION fsm_stack_get
//      - gets current pointer from top of the stack
//==============================================================================
int fsm_stack_get(fsm_stack* s)
{
  // Return the value at head
  return s->arr[s->top];
}

//==============================================================================
// FUNCTION fsm_get_str
//      - gets a string value of specified attribute
//==============================================================================
int fsm_get_str(fsm_stack* s, char* str)
{
  // Copy next token's string (attribute value) into specified string
  return fsm_str_cpy(fsm_stack_get(s)+1,str);
}

//==============================================================================
// FUNCTION fsm_get_i
//      - gets an integer value of specified attribute
//==============================================================================
int fsm_get_i(fsm_stack* s, int* var)                                           
{
  // Get value's token
  int ptr = fsm_stack_get(s)+1;
  // Get length of value's string
  int len = tkns[ptr].end-tkns[ptr].start;
  // Allocate appropriate memory (temp string)
  char* str = (char*)malloc(sizeof(char)*len);
  // Copy value string into the temp string
  fsm_str_cpy(fsm_stack_get(s)+1,str);
  // Convert to integer
  *var = atoi(str);
  // Free memory
  for(int i=0;i<len;i++){str[i]='\0';}
  free(str);
  return *var;
}

//==============================================================================
// FUNCTION fsm_get_f
//      - gets a float value of specified attribute
//==============================================================================
float fsm_get_f(fsm_stack* s, float* var)                                       
{
  // Get value's token
  int ptr = fsm_stack_get(s)+1;
  // Get length of value's string
  int len = tkns[ptr].end-tkns[ptr].start;
  // Allocate appropriate memory (temp string)
  char* str = (char*)malloc(sizeof(char)*len);
  // Copy value string into the temp string
  fsm_str_cpy(fsm_stack_get(s)+1,str);
  // Convert to integer
  *var = atof(str);
  // Free memory
  for(int i=0;i<len;i++){str[i]='\0';}
  free(str);
  return *var;
}



int fsm_get_elm(int ptr, int inst)
{
  int arry_end = 0, elem_end = 0;
  //Check if referenced ptr is an array
  // Get array end
  arry_end = tkns[++ptr].end;
  // Get end of first element
  elem_end = tkns[++ptr].end;
  // While in array
  while(tkns[ptr].start<arry_end&&inst>0)
  {
    //If object is direct element of the array
    if(tkns[ptr].end==elem_end||tkns[ptr].start>elem_end)                   
    {
      // If not the right element, set this element's end as ref for next
      if(inst!=1){elem_end = tkns[ptr].end;inst--;}
      else return ptr;      
    }
    ptr++;
  }
  if(inst>0) return FB_ERR; //No such element found
  return ptr;
}


//==============================================================================
// FUNCTION fsm_get_tag
//      - gets a specified tag object from a specified tag object (pass the name)
//==============================================================================
int fsm_get_tag(int ptr, const char* name, int inst)
{
  // Call get_tkn using type TAG
  return fsm_get_tkn(ptr,name,TAG,inst);  
}

//==============================================================================
// FUNCTION fsm_get_att
//      - gets a specified attribute from a specified tag object (pass the name)
//==============================================================================
int fsm_get_att(int ptr, const char* name)
{
  // Call get_tkn using type ATT
  return fsm_get_tkn(ptr,name,ATT,1);                  
}

//==============================================================================
// FUNCTION fsm_get_tkn
//      - gets a specified xml type from a specified tag object (pass the name)
//==============================================================================
int fsm_get_tkn(int ptr, const char* name, xmltype type, int inst)
{
  int prnt_end=0,chld_end=0;
  // If referenced token is not a tag name, quit                                //DISTINGUISH BETWEEN REGULAR TAG AND ELEMENT
  //if(!fsm_is_tag(ptr)){return FB_NOT_FOUND;}
  
  // End of parent object
  if(fsm_is_tag(ptr))   // If this is a tag, get size of next token
  {
    prnt_end = tkns[++ptr].end;
  }
  else if(fsm_is_obj(ptr)) // If this is an object (so, element), get size of this token
  {
    prnt_end = tkns[ptr].end;
  }
  else {return FB_ERR;}
  
  // Get into the object (point to next token)
  chld_end = tkns[(++ptr)+1].end;
  
  // While in parent
  while(tkns[ptr].start<prnt_end)     
  {
    // If token is a direct child of the parent object
    if(tkns[ptr+1].end==chld_end||tkns[ptr].start>(chld_end+1))
    {
      // If it matches name
      if(fsm_str_cmp(ptr,name))
      {
          // If request TAG or ATT and token string is TAG or ATT, return pointer
          if((type==TAG&&fsm_is_tag(ptr))||
             (type==ATT&&fsm_is_att(ptr)))
          {
            // If this is not an array and instance is larger than 1, return error
            if(!fsm_is_arr(ptr+1)&&inst>1){return FB_ERR;}
            // If this is an array, return the requested element
            if(fsm_is_arr(ptr+1)){ptr = fsm_get_elm(ptr,inst);}
            return ptr;
          }
      }
      // If not found the right child, set this child's end as ref for next
      chld_end = tkns[ptr+1].end;
    }
    ptr++; //Go to the next token
  }
  // If didn't find token, return error
  return FB_ERR;
}

//==============================================================================
// FUNCTION fsm_str_cpy
//      - copies a string from fsm string into a given string
//==============================================================================
uint8_t fsm_str_cpy(int ptr, char *str)
{
  // Check if token is string
  if(!fsm_is_str(ptr)){return 0;}
  // Find start of string
  int strt = tkns[ptr].start;
  // Check if string is an attribute (if attribute, increment start pointer)
  if(fsm_is_att(ptr)){strt++;}
  // Copy string from fstr to destination str
  memmove(str,&fstr[strt],((tkns[ptr].end)-(strt)));
  // Return ok
  return 1;
}

//==============================================================================
// FUNCTION fsm_str_cmp
//      - compares string from FSM string to a known tag or attribute name
//==============================================================================
uint8_t fsm_str_cmp(int ptr, const char *string)
{
  // Check if token is string
  if(!fsm_is_str(ptr)){return 0;}
  // Find start and end of string
  int strt = tkns[ptr].start;
  int end = tkns[ptr].end;
  // Check if string is an attribute (if attribute, increment start pointer)
  if(fsm_is_att(ptr)){strt++;}
  // Check size of strings
  if((end-strt-strlen(string))!=0){return 0;}
  // If memcmp unsuccessful, strings don't match
  if(memcmp(&fstr[strt],string,(end-strt))){return 0;}
  // Otherwise, strings match
  return 1;
}

//==============================================================================
// FUNCTION fsm_is_att
//      - checks if a string is an attribute (they start with non-alphanumeric)
//==============================================================================
uint8_t fsm_is_att(int ptr)
{
  // If not a string and next token is not a string - not a valid attribute
  if(!(fsm_is_str(ptr)&&fsm_is_str(ptr+1))){return 0;}
  // Pick out first character in string pointed to by token's start field
  char c = fstr[tkns[ptr].start];
  // Check if it is alphanumeric ==> if yes, not a valid attribute name
  if((c>='0'&&c<='9')||(c>='A'&&c<='Z')||(c>='a'&&c<='z')){return 0;}
  // If not, string is an attribute name
  return 1;
}

//==============================================================================
// FUNCTION fsm_is_tag
//      - checks if a string is a tag (alphanumeric start + object next)
//==============================================================================
uint8_t fsm_is_tag(int ptr)
{
  // Check if token is of string type and next token is of object type
  //    or if token is string, next is array, and next is object
  if(!((fsm_is_str(ptr) && fsm_is_obj(ptr+1))||
       (fsm_is_str(ptr) && fsm_is_arr(ptr+1) && fsm_is_obj(ptr+2)))){return 0;}
  return 1;
}

//==============================================================================
// FUNCTION fsm_is_obj
//      - checks if jsmn token is an object type
//==============================================================================
uint8_t fsm_is_obj(int ptr)
{
  // Check if type field of token is JSMN_OBJECT
  if(tkns[ptr].type!=JSMN_OBJECT){return 0;}    return 1;
}

//==============================================================================
// FUNCTION fsm_is_arr
//      - checks if jsmn token is an array type
//==============================================================================
uint8_t fsm_is_arr(int ptr)
{
  // Check if type field of token is JSMN_ARRAY
  if(tkns[ptr].type!=JSMN_ARRAY){return 0;}     return 1;
}

//==============================================================================
// FUNCTION fsm_is_obj
//      - checks if jsmn token is a string type
//==============================================================================
uint8_t fsm_is_str(int ptr)
{
  // Check if type field of token is JSMN_STRING
  if(tkns[ptr].type!=JSMN_STRING){return 0;}       return 1;
}

//==============================================================================
// FUNCTION fsm_free_mem
//      - frees memory for all the allocated pointers
//==============================================================================
errcode fsm_mem_free(void)
{
  // Free tokens array if allocated
  if(tkns){free(tkns);tkns=NULL;}
  // Free char array if allocated
  if(fstr){free(fstr);fstr=NULL;}
  // If free was not successful for one of them, return error flag
  if(tkns||fstr){return FB_ERR;}
  // Otherwise, return ok
  return FB_OK;
}
