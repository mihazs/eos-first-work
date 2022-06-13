#include "user_tasks.h"
#include "kernel.h"
#include "sync.h"
#include "memory.h"

mutex_t m_test;
//byte *mem_alloc_test;

void config_tasks()
{
   TRISDbits.RD0  = 0;
   TRISDbits.RD1  = 0;
   TRISDbits.RD2  = 0;
   
   // Inicializa a variável mutex
   init_mutex(&m_test);
   
   // Declara as tarefas como funções globais
   asm("GLOBAL _task_1, _task_2, _task_3");
}

TASK task_1()
{
   //mem_alloc_test = SRAMalloc(10);
   
   while (1) {
      LATDbits.LATD0 = ~PORTDbits.RD0;
      lock(&m_test);
      delay_task(600);
      unlock(&m_test);
   }
}

TASK task_2()
{
   while (1) {
      lock(&m_test);
      LATDbits.LATD1 = ~PORTDbits.RD1;
      unlock(&m_test);
   }
}

TASK task_3()
{
   while (1) {
      LATDbits.LATD2 = ~PORTDbits.RD2;
      delay_task(10);
   }
}
