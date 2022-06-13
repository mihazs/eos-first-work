#include "kernel.h"
#include "user_tasks.h"
#include "scheduler.h"
#include "memory.h"

// Declaração da fila de aptos
READY_queue_t ready_queue;
u_int rr_quantum_control = RR_QUANTUM;

// Rotinas de configuração
void __interrupt() isr_INTERRUPTS()
{
   // Verificação da flag de interrupção
   if (INTCONbits.INT0IF) {      
      INTCONbits.INT0IF = 0;
   }
   else if (INTCONbits.TMR0IF) {
      INTCONbits.TMR0IF    = 0;
      TMR0                 = 64;
      
      // Verifique se existem tarefas no estado de waiting
      int i;
      for (i = 0; i < ready_queue.tasks_installed; i++) {
         if (ready_queue.tasks_list[i].delay_waiting > 0) {
            ready_queue.tasks_list[i].delay_waiting -= 1;
            if (ready_queue.tasks_list[i].delay_waiting == 0) {
               ready_queue.tasks_list[i].task_STATE = READY;
            }
         }
      }
   #if DEFAULT_SCHEDULER == RR_SCHEDULER
      rr_quantum_control  -= 1;
      if (rr_quantum_control == 0) {
         rr_quantum_control = RR_QUANTUM;
         // Salva contexto da tarefa que está em execução
         SAVE_CONTEXT(READY);
         // Chama o escalonador para selecionar outra tarefa para executar
         scheduler();
         // Restaura o contexto da tarefa que irá executar
         RESTORE_CONTEXT();
      }
   #elif DEFAULT_SCHEDULER == PRIOR_SCHEDULER
         // Salva contexto da tarefa que está em execução
         SAVE_CONTEXT(READY);
         // Chama o escalonador para selecionar outra tarefa para executar
         scheduler();
         // Restaura o contexto da tarefa que irá executar
         RESTORE_CONTEXT();
   #endif
   }
}

void config_os()
{
   // Ajusta estrutura de dados do kernel
   ready_queue.task_running      = 0;
   ready_queue.tasks_installed   = 0;
   
   __asm("GLOBAL _task_idle");
   
   // Cria a tarefa idle()
   create_task(0, 0, task_idle);
   
   #if DEBUG_TASK_IDLE == ENABLE
   TRISCbits.RC0                 = 0;
   #endif
   
   // Configuração das interrupções
   // Int0
   TRISBbits.RB0                 = 1;
   INTCONbits.INT0IE             = 1;
   INTCONbits.INT0IF             = 0;
   // Timer0
   INTCONbits.PEIE               = 1;
   INTCONbits.TMR0IE             = 1;
   INTCONbits.TMR0IF             = 0;
   T0CONbits.TMR0ON              = 0;
   T0CONbits.T0CS                = 0;
   T0CONbits.PSA                 = 0;
   T0CONbits.T0PS                = 0b101;
   
   // Configuração das tarefas de usuário
   config_tasks();
}


// Chamadas de sistema
#if DYNAMIC_ALLOC
void create_task(u_int id, u_int prior, f_task task, u_int stack_size)
{
   TCB_t new_task;
   
   // Cria uma nova tarefa
   new_task.task_ID                 = id;
   new_task.task_PRIOR              = prior;
   new_task.task_PTR                = task;
   new_task.task_STATE              = READY;
   new_task.task_stack_real_size    = 0;
   new_task.delay_waiting           = 0;
   
   new_task.task_CONTEXT = SRAMalloc(stack_size);
   
   ready_queue.tasks_list[ready_queue.tasks_installed] = new_task;
   ready_queue.tasks_installed                        += 1;
}

#else
void create_task(u_int id, u_int prior, f_task task)
{
   TCB_t new_task;
   
   // Cria uma nova tarefa
   new_task.task_ID                 = id;
   new_task.task_PRIOR              = prior;
   new_task.task_PTR                = task;
   new_task.task_STATE              = READY;
   new_task.task_stack_real_size    = 0;
   new_task.delay_waiting           = 0;
   
   ready_queue.tasks_list[ready_queue.tasks_installed] = new_task;
   ready_queue.tasks_installed                        += 1;
}
#endif
void yield_task()
{
   GLOBAL_INTERRUPTS_DISABLE();
   ready_queue.tasks_list[ready_queue.task_running].task_STATE = READY;
   scheduler();
   RESTORE_CONTEXT();
}

void exit_task()
{
   GLOBAL_INTERRUPTS_DISABLE();
   ready_queue.tasks_list[ready_queue.task_running].task_STATE = TERMINATED;
   scheduler();
   RESTORE_CONTEXT();
}

void delay_task(u_int time)
{
   GLOBAL_INTERRUPTS_DISABLE();
   ready_queue.tasks_list[ready_queue.task_running].delay_waiting = time;
   SAVE_CONTEXT(WAITING);
   scheduler();
   RESTORE_CONTEXT();
}

void start_os()
{
   #if DYNAMIC_ALLOC
   SRAMInitHeap();
   #endif
   
   // Habilita as interrupções
   GLOBAL_INTERRUPTS_ENABLE();
   // Inicializar o timer0
   T0CONbits.TMR0ON = 1;
   TMR0             = 64;
}

TASK task_idle()
{
   while (1) {
      #if DEBUG_TASK_IDLE == ENABLE
      LATCbits.LATC0 = ~PORTCbits.RC0;
      #else
      Nop();
      #endif
   }
}
