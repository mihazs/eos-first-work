#include "so.h"

int main()
{
   // Configura��o do sistema operacional
   config_os();
   
   // Cria��o das tarefas de usu�rio
   create_task(1, 3, task_1);
   create_task(2, 2, task_2);
   create_task(3, 1, task_3);
   
   // Inicializa o sistema operacional
   start_os();
   
   while (1) {
      
   }
   
   return 0;
}
