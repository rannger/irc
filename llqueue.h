#ifndef __LLQUEUE_H
#define __LLQUEUE_H
#include <stdint.h>

struct llqueue_node {
	void* val;
	struct llqueue_node* next;

	llqueue_node()
	:val(0)
	,next(0)
	{
	}
};

struct llqueue {
	llqueue_node* head;
	llqueue_node* tail;
	volatile uint32_t size;

	llqueue()
	:head(0)
	,tail(0)
	,size(0)
	{
	}

	~llqueue() {}
};

void initQueue(llqueue** queue);
int queueAppend(llqueue* queue,void* val);
void* queueGet(llqueue* queue);
#endif
