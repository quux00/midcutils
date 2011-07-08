#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include "../minunit/minunit.h"
#include "intstack.h"

/* unit test for ch15/intstack.c */

int tests_run = 0;
char msg[100];
 
static char * test_initial_state() {
  stack_reset();
  /* test multiple braces */
  sprintf(msg, "ERROR: %d: init_state: should not be empty", __LINE__);
  mu_assert(msg, stack_is_empty());
  sprintf(msg, "ERROR: %d: init_state: should not be full", __LINE__);
  mu_assert(msg, ! stack_is_full());

  push(1);
  sprintf(msg, "ERROR: %d: init_state: should not be empty", __LINE__);
  mu_assert(msg, ! stack_is_empty());
  sprintf(msg, "ERROR: %d: init_state: should not be full", __LINE__);
  mu_assert(msg, ! stack_is_full());

  return EXIT_SUCCESS;
}


static char * test_push_till_full() {
  int i = 0;

  stack_reset();

  for (i = 0; i < MAX_INTSTACK - 1; i++) {
    push(i);
    sprintf(msg, "ERROR: %d: iter %d: push_till_full: should not be full", __LINE__, i);
    mu_assert(msg, ! stack_is_full());
    sprintf(msg, "ERROR: %d: iter %d: push_till_full: should not be full", __LINE__, i);
    mu_assert(msg, ! stack_is_empty());
  }

  push(INT_MAX);

  sprintf(msg, "ERROR: %d: push_till_full: should not be empty", __LINE__);
  mu_assert(msg, ! stack_is_empty());
  sprintf(msg, "ERROR: %d: push_till_full: should be full", __LINE__);
  mu_assert(msg, stack_is_full());

  return EXIT_SUCCESS;
}
 
static char * test_push_pop() {
  int i;

  stack_reset();

  /* push one, pop one */
  push(INT_MAX);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == INT_MAX);
  sprintf(msg, "ERROR: %d: should be empty", __LINE__);
  mu_assert(msg, stack_is_empty());
  sprintf(msg, "ERROR: %d: should pop INT_MIN when empty", __LINE__);
  mu_assert(msg, pop() == INT_MIN);
  sprintf(msg, "ERROR: %d: should pop INT_MIN when empty", __LINE__);
  mu_assert(msg, pop() == INT_MIN);
  

  /* push three, pop two */
  push(INT_MAX);
  push(-1);
  push(44);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == 44);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == -1);
  
  /* push three, pop one */
  push(1);
  push(2);
  push(3);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == 3);

  /* should have three on intstack - add till full */
  for (i = 30; ! stack_is_full(); i++) {
    push(i);
  }

  sprintf(msg, "ERROR: %d: should be full", __LINE__);
  mu_assert(msg, stack_is_full());

  /* now pop all remaining and test values */
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == 36);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == 35);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == 34);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == 33);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == 32);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == 31);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == 30);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == 2);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == 1);
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == INT_MAX);
  sprintf(msg, "ERROR: %d: should be empty", __LINE__);
  mu_assert(msg, stack_is_empty());
  sprintf(msg, "ERROR: %d: should pop expected", __LINE__);
  mu_assert(msg, pop() == INT_MIN);


  return EXIT_SUCCESS;
}


static char * test_peek() {
  stack_reset();

  // peek should return INT_MIN (with warning) when intstack is empty
  sprintf(msg, "ERROR: %d: peek should return INT_MIN when empty", __LINE__);
  mu_assert(msg, peek() == INT_MIN);
  
  push(55);
  sprintf(msg, "ERROR: %d: peek should return expected", __LINE__);
  mu_assert(msg, peek() == 55);
  sprintf(msg, "ERROR: %d: peek should return expected", __LINE__);
  mu_assert(msg, peek() == 55);
  sprintf(msg, "ERROR: %d: peek should return expected", __LINE__);
  mu_assert(msg, peek() == 55);

  push(56);
  sprintf(msg, "ERROR: %d: peek should return expected", __LINE__);
  mu_assert(msg, peek() == 56);
  sprintf(msg, "ERROR: %d: peek should return expected", __LINE__);
  mu_assert(msg, peek() == 56);

  sprintf(msg, "ERROR: %d: push_till_full: should not be empty", __LINE__);
  mu_assert(msg, ! stack_is_empty());
  sprintf(msg, "ERROR: %d: push_till_full: should not be full", __LINE__);
  mu_assert(msg, ! stack_is_full());

  pop();
  sprintf(msg, "ERROR: %d: peek should return expected", __LINE__);
  mu_assert(msg, peek() == 55);

  pop();
  sprintf(msg, "ERROR: %d: peek should return expected", __LINE__);
  mu_assert(msg, peek() == INT_MIN);

  return EXIT_SUCCESS;
}
 

 
static char * all_tests() {
  mu_run_test(test_initial_state);
  mu_run_test(test_push_till_full);
  mu_run_test(test_peek);
  mu_run_test(test_push_pop);
  return EXIT_SUCCESS;
}
 
int main(int argc, char *argv[]) {
  char *result = all_tests();
  if (result != 0) {
    printf("%s\n", result);
  }
  else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);
 
  return result != 0;
}
