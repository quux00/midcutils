#include <stdio.h>
#include <stdlib.h>
#include "midcqueue.h"

// the modulo arithmetic is used for both f and r ptrs: keep code DRY
#define QMODULO_ADD(v) (q->v = (q->v + 1) % q->maxsize)

int midc_qsize(struct midc_queue *q) {
  return ((q->r - q->f + q->maxsize) % q->maxsize);
}

bool midc_qisempty(struct midc_queue *q) {
  return q->f == q->r;
}

int midc_qenqueue(struct midc_queue *q, void *ent) {
  // resize if queue is full
  if (q->maxsize - midc_qsize(q) == 1) {
    void **newq;
    newq = calloc(q->maxsize * 2, sizeof(void *));
    if (newq == NULL) return -1;
    int newr = 0; // new 'r' pointer
    //now copy from one queue-array to the other
    for (; !midc_qisempty(q); newr++) {
      newq[newr] = midc_qdequeue(q);
    }
    free(q->pqueue);
    // reset new queue to the proper settings
    q->pqueue = newq;      
    q->f = 0;
    q->r = newr;
    q->maxsize *= 2;
  
#if defined DEBUG    
    fprintf(stderr, "Queue overflow\n");
#endif
  }

  // enqueue the new entity
  *(q->pqueue + q->r) = ent;
  QMODULO_ADD(r);
  return 0;
}


void *midc_qpeek(struct midc_queue *q) {
  if (!midc_qisempty(q)) return *(q->pqueue + q->f);
  else {
    //queue underflow: set errno to indicate an error state
    errno = ERANGE;
    return NULL;
  }
}

void *midc_qdequeue(struct midc_queue *q) {
  void *s = midc_qpeek(q);
  if (s != NULL) QMODULO_ADD(f);
  return s;
}


/**
 * Will initialize and possibly create a new midc_queue struct.
 * @param q - if you pass in a midc_queue pointer, it is assumed
 *            that you have already created the struct (either
 *            on the stack or the heap.  This method will try
 *            to initialize the midc_queue it is pointing to
 *          - if q is NULL, then qcreate will create a new
 *            midc_queue for you on the heap and initialize it.
 *          - In either case, remember to call qdispose when
 *            done with the midc_queue - that method is smart enough
 *            to know whether to dispose of just the array in the
 *            midc_queue or both the array and the struct itself.
 * @param sz - the initial size of the underlying queue itself
 *             make this value large enough to avoid unnecessary
 *             expansions of the queue later, but it is not a hard
 *             limit, since the qenqueue method will expand the
 *             size of the queue if more space is needed.
 * @return a pointer to the struct midc_queue (either the same one
 *         you passed in or the new one that was just created on
 *         the heap
 */
struct midc_queue *midc_qcreate(struct midc_queue *q, int sz) {
  if (q == NULL) {
    q = calloc( 1, sizeof(struct midc_queue) );
    if (q == NULL) return NULL;
    q->onheap = true;
  } else {
    q->onheap = false;
  }
  // sz+1 bcs one slot is always unused
  q->pqueue = calloc( sz+1, sizeof(void *) );
  if (q->pqueue == NULL) {
    if (q->onheap) free(q);
    return NULL;
  }
  q->f = 0;
  q->r = 0;
  q->maxsize = sz+1;
  return q;
}


/**
 * Will free of any memory that this library has itself
 * created on the heap.  If you passed in a midc_queue
 * to qcreate, then it will only free the underlying queue
 * (array) itself.  If you passed in NULL, it will free
 * both the queue (array) and midc_queue (struct).
 */
void midc_qdispose(struct midc_queue *q) {
  if (q != NULL) {
    if (q->pqueue != NULL) free(q->pqueue);
    if (q->onheap)         free(q);
  }
}

#if defined DEBUG
void midc_qinspect(struct midc_queue *q) {
  printf("Struct metadata: f: %d; r: %d; size = %d; maxsize = %d\n",
         q->f, q->r, qsize(q), q->maxsize );
}

/**
 * This method assumes that the contents of the
 * queue can be printed with the %s format
 * of printf.  Will probably fail otherwise.
 */
void midc_qcontents(struct midc_queue *q) {
  puts("======== Struct contents =========");
  int i;
  char *preface;
  for (i = 0; i < (*q).maxsize; i++) {
    if (q->r == i && q->f == i) preface = "fr=>";
    else if (q->r == i) preface = "r =>";
    else if (q->f == i) preface = "f =>";
    else preface = "    ";

    if ( q->pqueue[i] == NULL || q->r == i ) {
      printf("%s[%d]: %s\n", preface, i, "NULL");
    } else {
      printf("%s[%d]: %s\n", preface, i, q->pqueue[i]);
    }
  }
  puts("====== END Struct contents =======");
}
#endif  /* #if defined DEBUG */ 
