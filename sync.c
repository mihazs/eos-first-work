#include "sync.h"
#include "kernel.h"
#include "scheduler.h"

// System calls for mutex mechanism
void init_mutex(mutex_t *resource)
{
   resource->mutex_flag                               = FALSE;
   resource->mutex_bloqued_queue.nr_of_tasks_bloqued  = 0;
   resource->mutex_bloqued_queue.input_pos            = 0;
   resource->mutex_bloqued_queue.output_pos           = 0;
}

void lock(mutex_t *resource)
{
   GLOBAL_INTERRUPTS_DISABLE();
   // Verifica se o recurso está desbloqueado
   if (!resource->mutex_flag) resource->mutex_flag = TRUE;
   else {
      // Bloqueia o processo (recurso indisponível)
      resource->mutex_bloqued_queue.queue[resource->mutex_bloqued_queue.input_pos] = ready_queue.task_running;
      resource->mutex_bloqued_queue.input_pos = (resource->mutex_bloqued_queue.input_pos+1) % MAX_TASKS_SCHEDULER;
      resource->mutex_bloqued_queue.nr_of_tasks_bloqued++;
      SAVE_CONTEXT(W_MUTEX);
      scheduler();
      RESTORE_CONTEXT();
   }
   GLOBAL_INTERRUPTS_ENABLE();
}

void unlock(mutex_t *resource)
{
   GLOBAL_INTERRUPTS_DISABLE();
   // Desbloqueia processo
   resource->mutex_flag = FALSE;
   // Verifica se existem processos bloqueados aguardando a liberação do recurso
   if (resource->mutex_bloqued_queue.nr_of_tasks_bloqued > 0) {
      // Desbloqueia o processo (recurso indisponível)
      ready_queue.tasks_list[resource->mutex_bloqued_queue.queue[resource->mutex_bloqued_queue.output_pos]].task_STATE = READY;
      resource->mutex_bloqued_queue.output_pos = (resource->mutex_bloqued_queue.output_pos+1) % MAX_TASKS_SCHEDULER;
      resource->mutex_bloqued_queue.nr_of_tasks_bloqued--;
      SAVE_CONTEXT(READY);
      scheduler();
      RESTORE_CONTEXT();
   }
   GLOBAL_INTERRUPTS_ENABLE();   
}

// System calls for semaphore mechanism
void sem_init(semaphore_t *resource, int nr_of_instances)
{
   resource->sem_value                       = nr_of_instances;
   resource->sem_bloqued_queue.input_pos     = 0;
   resource->sem_bloqued_queue.output_pos    = 0;   
}

void sem_wait(semaphore_t *resource)
{
   
}

void sem_post(semaphore_t *resource)
{
   
}

