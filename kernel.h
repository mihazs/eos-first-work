#ifndef KERNEL_H
#define	KERNEL_H

#include "types.h"

// Acesso a fila de aptos
extern READY_queue_t ready_queue;


// Rotinas de configuração
void __interrupt() isr_INTERRUPTS();
void config_os();


// Chamadas de sistema
#if DYNAMIC_ALLOC
void create_task(u_int id, u_int prior, f_task task, u_int stack_size);
#else
void create_task(u_int id, u_int prior, f_task task);
#endif
void yield_task();
void start_os();
void exit_task();
TASK task_idle();
void delay_task(u_int time);

// Rotinas para salvar e restaurar o contexto de uma tarefa
#define SAVE_CONTEXT(STATE) { \
u_int p_context = 0, task_pos = ready_queue.task_running;\
do { \
    GLOBAL_INTERRUPTS_DISABLE(); \
    if (ready_queue.tasks_list[task_pos].task_STATE == RUNNING) { \
      ready_queue.tasks_list[task_pos].task_BSR_reg      = BSR; \
      ready_queue.tasks_list[task_pos].task_WORK_reg     = WREG; \
      ready_queue.tasks_list[task_pos].task_STATUS_reg   = STATUS; \
      do { \
        ready_queue.tasks_list[task_pos].task_CONTEXT[p_context] = TOS; \
        asm("POP");\
        p_context += 1;\
      } while (STKPTR);\
      ready_queue.tasks_list[task_pos].task_stack_real_size = p_context; \
      ready_queue.tasks_list[task_pos].task_STATE           = STATE;\
    } \
    GLOBAL_INTERRUPTS_ENABLE(); \
} while(0); \
} \

#define RESTORE_CONTEXT() { \
u_int task_pos    = ready_queue.task_running;\
u_int p_context   = ready_queue.tasks_list[task_pos].task_stack_real_size;\
do { \
    GLOBAL_INTERRUPTS_DISABLE(); \
    STKPTR = 0;\
    if (ready_queue.tasks_list[task_pos].task_stack_real_size > 0) { \
      BSR      = ready_queue.tasks_list[task_pos].task_BSR_reg; \
      WREG     = ready_queue.tasks_list[task_pos].task_WORK_reg; \
      STATUS   = ready_queue.tasks_list[task_pos].task_STATUS_reg; \
      while (p_context) { \
        p_context -= 1;\
        asm("PUSH");\
        TOS = ready_queue.tasks_list[task_pos].task_CONTEXT[p_context];\
      }\
    } \
    else { \
      asm("PUSH");\
      TOS = ready_queue.tasks_list[task_pos].task_PTR;\
    } \
    ready_queue.tasks_list[task_pos].task_STATE = RUNNING;\
    GLOBAL_INTERRUPTS_ENABLE(); \
} while(0); \
} \

#endif