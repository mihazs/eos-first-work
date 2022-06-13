#ifndef USER_CONF_H
#define	USER_CONF_H

#include <xc.h>

#include "types.h"

#pragma config PBADEN = OFF
#pragma config WDT    = OFF


#define MAX_TASKS_SCHEDULER         4
#define STACK_SIZE                  32

#define DEFAULT_SCHEDULER           RR_SCHEDULER
#define DEBUG_TASK_IDLE             ENABLE
#define DYNAMIC_ALLOC               DISABLE


#endif	/* USER_CONF_H */

