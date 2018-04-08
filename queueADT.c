///
/// file queueADT.c
///
/// author: Owen Gruss
/// email: omg6429@rit.edu
///

#include <stdio.h>
#include <stdlib.h>
#define QUEUE_INC 5


struct myADT{
	void **contents;
	int size;
	int num;
 	int (*compare)(const void*a, const void*b);
};
typedef struct myADT *QueueADT;

#define _QUEUE_IMPL_
#include "queueADT.h"

QueueADT que_create(int (*cmp)(const void*a, const void*b)){
	QueueADT ret;
	ret  =  malloc(sizeof(struct myADT));
	ret->size = 0;
	ret->num = 0;
	ret->contents = NULL;
	ret->compare = cmp;
	return ret;	
}

void que_insert(QueueADT queue, void* data){
	if(queue->size == 0){
		queue->contents = malloc(sizeof(void*) * QUEUE_INC);
		queue->size =  QUEUE_INC;
	}
	if(queue->size == queue->num && queue->size > 0){
		queue->contents = realloc(queue->contents, sizeof(void*) * (queue->size + QUEUE_INC));
		queue->size +=  QUEUE_INC;
	}
	if(queue->compare == NULL){
		
		if(queue->num == 0)
                        queue->contents[0] = data;
		else{
			for(int i = queue->num - 1; i >= 0 ; i--){   
	                        queue->contents[i+1] = queue->contents[i];
        	        }
                        queue->contents[0] = data;
		}
	
	}
	else{
		if(queue->num == 0){
			queue->contents[0] = data;
		}
		else{
			int i;	
			for(i = queue->num - 1; (i >= 0 && (*queue->compare)(data, queue->contents[i]) > 0);
				i--){
				queue->contents[i+1] = queue->contents[i];
			}
			queue->contents[i+1] = data;
		}
	}
	queue->num ++;
	
}

void* que_remove(QueueADT queue){
	if(queue->num == 0){
		fprintf(stderr, "Queue was empty");
		exit(EXIT_FAILURE);
		return NULL;
	}
	void* ret = queue->contents[queue->num-1];
	queue->num = queue->num - 1;
	return ret;
}
void que_clear( QueueADT queue ){
	if(queue->size != 0)
		free(queue->contents);
	queue->size = 0;
	queue->num = 0;
}

void que_destroy( QueueADT queue ){
	if( queue->size != 0 )
		free(queue->contents);
	free(queue);
}

bool que_empty(QueueADT queue){
	if(queue->num == 0)
		return true;
	return false;
}

