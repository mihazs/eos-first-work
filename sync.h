#ifndef SYNC_H
#define	SYNC_H

#include "types.h"

// System calls for mutex mechanism
void init_mutex(mutex_t *resource);
void lock(mutex_t *resource);
void unlock(mutex_t *resource);

// System calls for semaphore mechanism
void sem_init(semaphore_t *resource, int nr_of_instances);
void sem_wait(semaphore_t *resource);
void sem_post(semaphore_t *resource);


#endif	/* SYNC_H */

