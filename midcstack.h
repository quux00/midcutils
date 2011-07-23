#ifndef MIDCSTACK_H
#define MIDCSTACK_H

#include <stdbool.h>
#include <errno.h>

/**
 * A dynamically resizing stack, with the normal methods associated
 * with the stack ADT.  Both the struct and its inner array (holding the stack)
 * are created on the heap by the create method.  Thus, when finished with
 * the stack, be sure to call midc_stkdispose, passing the pointer to the stack
 * so that both its malloc'd elements can be freed.  (If you call free on the
 * struct only, only 1 of the 2 parts of heap memory will be freed and you will
 * have a memory leak.)
 *
 * See the ut_midc_stack unit test for example usage.
 * Author: Michael Peterson
 * Date: June 2011
 */
struct midc_stack {
  void **pstack; // the stack (array of any type of entities), created on the heap
  void **top;    // pointer to top element on the stack
  int maxsize;   // current maxsize, but can be expanded in stkpush
  bool onheap;   // created by malloc? and should be freed in stkdispose?
};


struct midc_stack *midc_stkcreate(struct midc_stack *stk, int sz);
void  midc_stkdispose(struct midc_stack *stk);
int   midc_stkpush(struct midc_stack *stk, void *ent);
void *midc_stkpop(struct midc_stack *stk);
void *midc_stkpeek(struct midc_stack *stk);
bool  midc_stkisempty(struct midc_stack *stk);
void  midc_stk_reset(struct midc_stack *stk);
int   midc_stksize(struct midc_stack *stk);

#endif
