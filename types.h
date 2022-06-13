#ifndef TYPES_H
#define	TYPES_H

#include "user_conf.h"

#define TASK                            void
#define GLOBAL_INTERRUPTS_DISABLE()     INTCONbits.GIE = 0
#define GLOBAL_INTERRUPTS_ENABLE()      INTCONbits.GIE = 1

#define RR_SCHEDULER                    1
#define PRIOR_SCHEDULER                 2
#define FIFO_SCHEDULER                  3

#define ENABLE                          1
#define DISABLE                         0

#define TRUE                            1
#define FALSE                           0


typedef unsigned char byte;
typedef unsigned int u_int;
typedef void(*f_task)(void);
typedef enum {READY = 0, RUNNING, WAITING, W_MUTEX, W_SEM, TERMINATED} state_t;
typedef u_int pos_in_ready_queue;
typedef unsigned char boolean;

typedef struct TCB {
    u_int task_ID;
    u_int task_PRIOR;
    f_task task_PTR;
    __uint24 task_CONTEXT[STACK_SIZE];
    __uint24 task_BSR_reg;
    __uint24 task_WORK_reg;
    __uint24 task_STATUS_reg;
    u_int task_stack_real_size;
    state_t task_STATE;
    u_int delay_waiting;
} TCB_t;

typedef struct READY_queue {
    TCB_t tasks_list[MAX_TASKS_SCHEDULER+1];  // User tasks + task idle()
    u_int tasks_installed;
    u_int task_running;    
} READY_queue_t;

typedef struct sync_queue {
    pos_in_ready_queue queue[MAX_TASKS_SCHEDULER];
    u_int input_pos;
    u_int output_pos;
    u_int nr_of_tasks_bloqued;
} sync_queue_t;

typedef struct mutex {
    boolean mutex_flag;
    sync_queue_t mutex_bloqued_queue;
} mutex_t;

typedef struct semaphore {
    int sem_value;
    sync_queue_t sem_bloqued_queue;
} semaphore_t;

#endif	/* TYPES_H */

