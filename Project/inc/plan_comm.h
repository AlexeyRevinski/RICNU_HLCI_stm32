#ifndef PLAN_COMM_H
#define PLAN_COMM_H

// INCLUDES  ===================================================================

#include "plan_include.h"


// CONSTANTS ===================================================================

#define POS_MOTOR_ON_OFF 0
#define POS_MOTOR_DIRECT 1


// FUNCTION PROTOTYPES  ========================================================

void update_Manage(void);
void update_User(void);
void pack_P2U(void);
void pack_U2P(void);

#endif //PLAN_COMM_H