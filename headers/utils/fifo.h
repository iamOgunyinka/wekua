#ifndef WEKUA_FIFO_H
#define WEKUA_FIFO_H

#include <pthread.h>
#include <stdint.h>
#include "linked_list.h"

typedef struct _w_fifo_t {
	wlinked_list queue;
	pthread_mutex_t lock;
	pthread_cond_t cond;

	uint64_t qsize; // Numbers of elements in the queue
} *wfifo;

wfifo wekuaAllocFIFO();

void wekuaFIFOPut(wfifo fifo, void *data);
void *wekuaFIFOGet(wfifo fifo);

void wekuaFreeFIFO(wfifo queue);

#endif