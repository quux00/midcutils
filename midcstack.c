#include <stdio.h>
#include <stdlib.h>
#include <limits.h>   /* ~TODO: need this? */

#include "midcstack.h"

/**
 * Will initialize and possibly create a new midc_stack struct.
 * @param stk if you pass in a midc_stack pointer, it is assumed
 *            that you have already created the struct (either
 *            on the stack or the heap.  This method will try
 *            to initialize the midc_stack it is pointing to
 *          - if q is NULL, then qcreate will create a new
 *            midc_stack for you on the heap and initialize it.
 *          - In either case, remember to call qdispose when
 *            done with the midc_stack - that method is smart enough
 *            to know whether to dispose of just the array in the
 *            midc_stack or both the array and the struct itself.
 * @param sz - the initial size of the underlying stack itself
 *             make this value large enough to avoid unnecessary
 *             expansions of the stack later, but it is not a hard
 *             limit, since the qenstack method will expand the
 *             size of the stack if more space is needed.
 * @return a pointer to the struct midc_stack (either the same one
 *         you passed in or the new one that was just created on
 *         the heap
 */
struct midc_stack *midc_stkcreate(struct midc_stack *s, int sz) {
  if (s == NULL) {
    s = calloc( 1, sizeof(struct midc_stack) );
    if (s == NULL) return NULL;
    s->onheap = true;
  } else {
    s->onheap = false;
  }
  s->maxsize = sz;
  s->pstack = calloc( s->maxsize, sizeof(void *) );  
  if (s->pstack == NULL) {
    if (s->onheap) free(s);
    return NULL;
  }
  /* top points to first elt, so if empty has to point one less than pstack */
  /* this is dangerous, so always check isempty() before peek or pop */
  s->top = (s->pstack - 1);
  return s;
}

int midc_stksize(struct midc_stack *stk) {
  return (stk->top - stk->pstack) + 1;
}

/**
 * Will free of any memory that this library has itself
 * created on the heap.  If you passed in a midc_stack
 * to stkcreate, then it will only free the underlying stack
 * (array) itself.  If you passed in NULL, it will free
 * both the stack (array) and midc_stack (struct).
 */
void midc_stkdispose(struct midc_stack *s) {
  if (s != NULL) {
    if (s->pstack != NULL) free(s->pstack);
    if (s->onheap)         free(s);
  }
}

int midc_stkpush(struct midc_stack *s, void *ent) {
  if ((s->top - s->pstack) == (s->maxsize - 1)) {
    void **newstk;
    newstk = realloc(s->pstack, sizeof(void *) * (s->maxsize * 2));
    if (newstk == NULL) return -1;
    s->pstack = newstk;
    /* adjust top as well, in case realloc moved the stack in memory space */
    s->top = (s->pstack + (s->maxsize-1));
    s->maxsize *= 2;
  }

  ++(s->top);  /* always points to curr elt, so advance before assigning new elt */
  *(s->top) = ent;
  return 0;
}


bool midc_stkisempty(struct midc_stack *stk) {
  return (stk->top - stk->pstack < 0);
}

void *midc_stkpeek(struct midc_stack *s) {
  if ( !midc_stkisempty(s) ) return *(s->top);
  else {
    // stack underflow: set errno to indicate error state
    errno = ERANGE;
    return NULL;
  }
}

void *midc_stkpop(struct midc_stack *s) {
  void *elt = midc_stkpeek(s);  /* let peek handle any underflow error */
  if (elt != NULL) (s->top)--;
  return elt;
}


/**
 * Resets the stack to appear empty.
 * 
 * This method does not call free on any of the contents of the stack,
 * so only use this method if the elements put on there do not need to
 * be free'd from the heap, otherwise you will have a memory leak.
 */
void midc_stk_reset(struct midc_stack *stk) {
  stk->top = (stk->pstack - 1);
}
