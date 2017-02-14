#include "llqueue.h"
#include <libkern/OSAtomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void initQueue(llqueue** queue)
{
	*queue = reinterpret_cast<llqueue*>(new llqueue());
	if (NULL == *queue) {
		puts("malloc failed");
		assert(0);
	}

	(*queue)->head = reinterpret_cast<llqueue_node*>(new llqueue_node());
	(*queue)->tail = (*queue)->head;
	(*queue)->head->val = NULL;
	(*queue)->head->next = NULL;
}

int queueAppend(llqueue* queue,void* val)
{
	llqueue_node* n = NULL;
	llqueue_node* node = reinterpret_cast<llqueue_node*>(new llqueue_node());
	node->val = val;
	node->next = NULL;
	do {
		n = queue->tail;
		if (OSAtomicCompareAndSwapPtr(NULL,
					     reinterpret_cast<void*>(node),
			                     reinterpret_cast<void**>(&(n->next)))) {
			break;
		} else {
			OSAtomicCompareAndSwapPtr(reinterpret_cast<void*>(n),
      			                          reinterpret_cast<void*>(n->next),
			                          reinterpret_cast<void**>(&(queue->tail)));
		}
	} while(1);

	OSAtomicCompareAndSwapPtr(reinterpret_cast<void*>(n),
	                          reinterpret_cast<void*>(node),
	                          reinterpret_cast<void**>(&(queue->tail)));

	return 1;
}

void* queueGet(llqueue* queue)
{
	llqueue_node* n = NULL;
	void* val = NULL;
	do{
		n = queue->head;
		if (NULL == n->next) return NULL;
		if (OSAtomicCompareAndSwapPtr(reinterpret_cast<void*>(n),
					      reinterpret_cast<void*>(n->next),
			                      reinterpret_cast<void**>(&(queue->head))))
			break;
	}while(1);

	val = reinterpret_cast<void*>(n->next->val);
	delete n;
	return val;
}
