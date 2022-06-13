#ifndef SCHEDULER_H
#define	SCHEDULER_H

#include "types.h"

#define RR_QUANTUM      8

u_int RR_scheduler();
u_int PRIOR_scheduler();
u_int FIFO_scheduler();
void scheduler();


#endif	/* SCHEDULER_H */

