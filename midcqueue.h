#ifndef MIDCQUEUE_H
#define MIDCQUEUE_H

#include <stdbool.h>
#include <errno.h>

/**
 * A dynamically resizing queue, with the normal methods associated
 * with the queue ADT.  Both the struct and its inner array (holding the queue)
 * are created on the heap by the create method.  Thus, when finished with
 * the queue, be sure to call midc_qdispose, passing the pointer to the queue
 * so that both its malloc'd elements can be freed.  (If you call free on the
 * struct only, only 1 of the 2 parts of heap memory will be freed and you will
 * have a memory leak.)
 *
 * See the ut_midc_queue unit test for example usage.
 */

struct midc_queue {
  void **pqueue; // the queue (array of any type of entities), created on the heap
  int f;         // front of queue, offset relative to **pqueue 
  int r;         // rear of queue, offset relative to **pqueue 
  int maxsize;   // current maxsize, but can be expanded in qenqueue
  bool onheap;   // created by malloc? and should be freed in qdispose?
};


struct midc_queue *midc_qcreate(struct midc_queue *q, int sz);
void  midc_qdispose(struct midc_queue *q);
void *midc_qpeek(struct midc_queue *q);
int   midc_qenqueue(struct midc_queue *q, void *ent);
void *midc_qdequeue(struct midc_queue *q);
bool  midc_qisempty(struct midc_queue *q);
int   midc_qsize(struct midc_queue *q);

#if defined DEBUG
void  midc_qinspect(struct midc_queue *q);
void  midc_qcontents(struct midc_queue *q);
#endif  /* #if defined DEBUG */

#endif
