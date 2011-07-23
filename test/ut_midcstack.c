#include <stdlib.h>
#include <string.h>
#include "midcunit.h"
#include "midcstack.h"

/**
 * Unit test for the midc_stack.c library.
 * It uses the midcunit testing framework.
 * Author: Michael Peterson
 * July 2011
 */

// helper functions
static char * do_well_behaved_test(char *source, struct midc_stack *stk, int sz);
static char * do_overflow(char *source, struct midc_stack *stk, int sz);

static char * test_well_behaved_create_own() {
  char *rt;
  int sz = 9991999;
  struct midc_stack stk;
  midc_stkcreate(&stk, sz);
  rt = do_well_behaved_test("create-own", &stk, sz);
  return rt;
}

static char * test_well_behaved_library_creates() {
  char *rt;
  int sz = 13;
  struct midc_stack *stk = midc_stkcreate(NULL, sz);
  rt = do_well_behaved_test("library-creates", stk, sz);
  midc_stkdispose(stk);
  return rt;
}

static char * test_underflow() {
  int sz = 4;
  struct midc_stack *stk = midc_stkcreate(NULL, sz);
  char *s;

  md_assert( midc_stksize(stk) == 0 );
  errno = 0;
  s = midc_stkpop(stk);
  md_assert( errno == ERANGE );
  md_assert( s == NULL );
  errno = 0;
  s = midc_stkpeek(stk);
  md_assert( errno == ERANGE );
  md_assert( s == NULL );

  midc_stkpush(stk, "hi dad");
  midc_stkpush(stk, "hi mom");
  md_assert( midc_stksize(stk) == 2 );
  md_assert( strcmp(midc_stkpeek(stk), "hi mom") == 0 );
  errno = 0;
  s = midc_stkpop(stk);
  md_assert( errno == 0 );
  errno = 0;
  s = midc_stkpop(stk);
  md_assert( errno == 0 );
  md_assert( midc_stksize(stk) == 0 );
  md_assert( midc_stkisempty(stk) );
  md_assert( strcmp(s, "hi dad") == 0 );

  errno = 0;
  s = midc_stkpop(stk);
  md_assert( errno == ERANGE );
  md_assert( s == NULL );
  errno = 0;
  s = midc_stkpeek(stk);
  md_assert( errno == ERANGE );
  md_assert( s == NULL );

  midc_stkdispose(stk);
  return 0;
}

static char * test_overflow_create_own() {
  char *rt;
  int sz = 4;
  struct midc_stack stk;
  midc_stkcreate(&stk, sz);
  rt = do_overflow("create-own", &stk, sz);
  midc_stkdispose(&stk);
  return rt;
}

static char * test_overflow_library_creates() {
  char *rt;
  int sz = 4;
  struct midc_stack *stk;
  stk = midc_stkcreate(NULL, sz);
  rt = do_overflow("library-create", stk, sz);
  midc_stkdispose(stk);
  return rt;
}

// all previous tests enstackd strings, now try a numeric type

static char * test_int_stack() {
  int sz = 4;
  struct midc_stack *stk = midc_stkcreate(NULL, sz);

  int i, j, k, m;
  i = 11;
  j = 12;
  k = 13;
  m = 14;

  midc_stkpush(stk, &i);
  midc_stkpush(stk, &j);
  midc_stkpush(stk, &k);

  md_assert(  midc_stksize(stk) == 3 );
  md_assert( !midc_stkisempty(stk) );
  md_assert(  midc_stkpeek(stk) == &k );

  midc_stkpush(stk, &m);
  midc_stkpush(stk, &m);
  midc_stkpush(stk, &m);
  midc_stkpush(stk, &m);

  md_assert(  midc_stksize(stk) == 7 );
  md_assert( !midc_stkisempty(stk) );
  md_assert(  midc_stkpeek(stk) == &m );

  int *p;
  p = midc_stkpop(stk);
  md_assert( midc_stksize(stk) == 6 );
  md_assert( *p == 14 );

  p = midc_stkpop(stk);
  md_assert( midc_stksize(stk) == 5 );
  md_assert( *p == 14 );

  p = midc_stkpop(stk);
  md_assert( midc_stksize(stk) == 4 );
  md_assert( *p == 14 );

  p = midc_stkpop(stk);
  md_assert( midc_stksize(stk) == 3 );
  md_assert( *p == 14 );

  p = midc_stkpop(stk);
  md_assert( midc_stksize(stk) == 2 );
  md_assert( *p == 13 );

  p = midc_stkpop(stk);
  md_assert( midc_stksize(stk) == 1 );
  md_assert( !midc_stkisempty(stk) );
  md_assert( *p == 12 );

  p = midc_stkpop(stk);
  md_assert( midc_stksize(stk) == 0 );
  md_assert( midc_stkisempty(stk) );
  md_assert( *p == 11 );

  p = midc_stkpop(stk);
  md_assert( midc_stksize(stk) == 0 );
  md_assert( p == NULL );

  midc_stkdispose(stk);
  return 0;
}

// now try a newly defined type

typedef struct {
  char *str;
  int size;
} String;

static char * test_struct_String_on_stack() {
  int sz = 30;
  struct midc_stack *stk = midc_stkcreate(NULL, sz);

  String s1;
  String s2;

  s1.str = malloc(20);
  strcpy(s1.str, "hi mom");
  s1.size = strlen(s1.str);

  s2.str = malloc(40);
  strcpy(s2.str, "Papa was a Rolling Stone ...");
  s2.size = strlen(s2.str);

  midc_stkpush(stk, &s1);
  midc_stkpush(stk, &s1);
  midc_stkpush(stk, &s2);

  md_assert(  midc_stksize(stk) == 3 );
  md_assert( !midc_stkisempty(stk) );
  md_assert(  midc_stkpeek(stk) == &s2 );

  midc_stkpush(stk, &s2);

  md_assert(  midc_stksize(stk) == 4 );
  md_assert( !midc_stkisempty(stk) );
  md_assert(  midc_stkpeek(stk) == &s2 );

  String *p;
  p = midc_stkpop(stk);
  md_assert( midc_stksize(stk) == 3 );
  md_assert( p->size == s2.size );
  md_assert( strcmp(p->str, s2.str) == 0 );

  p = midc_stkpop(stk);
  md_assert( midc_stksize(stk) == 2 );
  md_assert( p->size == s2.size );
  md_assert( strcmp(p->str, s2.str) == 0 );

  p = midc_stkpop(stk);
  md_assert( midc_stksize(stk) == 1 );
  md_assert( p->size == s1.size );
  md_assert( strcmp(p->str, s1.str) == 0 );

  free(s1.str);
  free(s2.str);
  midc_stkdispose(stk);
  return 0;
}

static void all_tests() {
  md_run_test(test_well_behaved_create_own);
  md_run_test(test_well_behaved_library_creates);
  md_run_test(test_underflow);
  md_run_test(test_overflow_create_own);
  md_run_test(test_overflow_library_creates);
  md_run_test(test_int_stack);
  md_run_test(test_struct_String_on_stack);
}

int main() {
  RUN_TESTS(all_tests);
}

/* --- HELPER FUNCTIONS --- */
static char * do_overflow(char *source, struct midc_stack *stk, int sz) {
  int i;
  for (i = 0; i < sz; i++) {
    midc_stkpush(stk, "first string");
  }
  md_assertm( source, midc_stksize(stk) == sz );
  md_assertm( source, strcmp(midc_stkpop(stk), "first string") == 0 );
  md_assertm( source, midc_stksize(stk) == sz-1 );

  midc_stkpush(stk, "second string");
  md_assertm( source, midc_stksize(stk) == sz );
  // this one will cause a resize
  midc_stkpush(stk, "second string");
  md_assertm( source, midc_stksize(stk) == sz+1 );
  md_assertm( source, strcmp(midc_stkpeek(stk), "second string") == 0 );

  for (i = 0; i < sz; i++) {
    md_assertm( source, midc_stkpush(stk, "second string") > -1 );
  }
  md_assertm( source, midc_stksize(stk) == 9 );
  md_assertm( source, strcmp(midc_stkpop(stk), "second string") == 0 );
  md_assertm( source, strcmp(midc_stkpop(stk), "second string") == 0 );
  md_assertm( source, midc_stksize(stk) == 7 );

  midc_stkpush(stk, "third string");
  md_assertm( source, strcmp(midc_stkpop(stk), "third string") == 0 );
  midc_stkpush(stk, "second string");
  midc_stkpush(stk, "second string");
  midc_stkpush(stk, "second string");
  md_assertm( source, midc_stksize(stk) == 10 );
  md_assertm( source, strcmp(midc_stkpeek(stk), "second string") == 0 );

  // this will cause another resize
  for (i = 0; i < 20; i++) {
    md_assertm( source, midc_stkpush(stk, "third string") > -1 );
  }
  md_assertm( source, midc_stksize(stk) == 30 );
  md_assertm( source, strcmp(midc_stkpeek(stk), "third string") == 0 );

  md_assertm( source, strcmp(midc_stkpop(stk), "third string") == 0 );
  md_assertm( source, strcmp(midc_stkpop(stk), "third string") == 0 );
  md_assertm( source, strcmp(midc_stkpop(stk), "third string") == 0 );
  md_assertm( source, strcmp(midc_stkpop(stk), "third string") == 0 );
  md_assertm( source, strcmp(midc_stkpop(stk), "third string") == 0 );
  md_assertm( source, strcmp(midc_stkpop(stk), "third string") == 0 );
  md_assertm( source, midc_stksize(stk) == 24 );
  md_assertm( source, strcmp(midc_stkpeek(stk), "third string") == 0 );

  for (i = 0; i < 15; i++) {
    midc_stkpush(stk, "fourth string");
  }
  md_assertm( source, midc_stksize(stk) == 39 );
  md_assertm( source, strcmp(midc_stkpeek(stk), "fourth string") == 0 );

  // this will cause another resize
  midc_stkpush(stk, "fifth and last string!!");
  md_assertm( source, midc_stksize(stk) == 40 );
  md_assertm( source, !midc_stkisempty(stk) );
  md_assertm( source, strcmp(midc_stkpeek(stk), "fifth and last string!!") == 0 );
  md_assertm( source, strcmp(midc_stkpop(stk), "fifth and last string!!") == 0 );
  md_assertm( source, strcmp(midc_stkpeek(stk), "fourth string") == 0 );
  for (i = 0; i < 15; i++) {
    md_assertm( source, strcmp(midc_stkpop(stk), "fourth string") == 0 );
  }
  md_assertm( source, strcmp(midc_stkpeek(stk), "third string") == 0 );
  md_assertm( source, midc_stksize(stk) == 24 );
  md_assertm( source, !midc_stkisempty(stk) );
  return 0;
}

static char * do_well_behaved_test(char *source, struct midc_stack *stk, int sz) {
  // library creates midc_stack on the heap
  char *rt;
  char *s = "string 1234";
  char t[] = "5555555555!! string 1234 asdfqwerasdf EOL";
  char *u = "";

  md_assertm( source, midc_stkisempty(stk) );
  md_assertm( source, midc_stksize(stk) == 0 );
  md_assertm( source, midc_stkpeek(stk) == NULL );
  md_assertm( source, midc_stkpush(stk, s) > -1 ); 

  md_assertm( source, midc_stksize(stk) == 1 );
  md_assertm( source, !midc_stkisempty(stk) );
  md_assertm( source, strcmp(midc_stkpeek(stk), s) == 0 );
  rt = midc_stkpop(stk);
  md_assertm( source, midc_stksize(stk) == 0 );
  md_assertm( source, midc_stkisempty(stk) );
  md_assertm( source, strcmp(s, rt) == 0 );
  
  md_assertm( source, midc_stkpush(stk, s) > -1 ); 
  md_assertm( source, midc_stksize(stk) == 1 );
  md_assertm( source, midc_stkpush(stk, t) > -1 ); 
  md_assertm( source, midc_stksize(stk) == 2 );
  md_assertm( source, midc_stkpush(stk, u) > -1 ); 
  md_assertm( source, midc_stksize(stk) == 3 );
  md_assertm( source, ! midc_stkisempty(stk) );
  md_assertm( source, strcmp(midc_stkpeek(stk), u) == 0 );

  md_assertm( source, strcmp(midc_stkpop(stk), u) == 0 );
  md_assertm( source, midc_stksize(stk) == 2 );
  md_assertm( source, strcmp(midc_stkpeek(stk), t) == 0 );

  md_assertm( source, strcmp(midc_stkpop(stk), t) == 0 );
  md_assertm( source, midc_stksize(stk) == 1 );
  md_assertm( source, strcmp(midc_stkpeek(stk), s) == 0 );

  int i;
  for (i = 0; i < sz - 1; i++) {
    md_assertm( source, midc_stkpush(stk, t) > -1 ); 
  }
  md_assertm( source, !midc_stkisempty(stk) );
  md_assertm( source, midc_stksize(stk) == sz );
  md_assertm( source, strcmp(midc_stkpeek(stk), t) == 0 );

  md_assertm( source, strcmp(midc_stkpop(stk), t) == 0 );
  md_assertm( source, midc_stksize(stk) == sz-1 );
  md_assertm( source, strcmp(midc_stkpeek(stk), t) == 0 );
  
  for (i = sz-2; i > 0; --i) {
    md_assertm( source, strcmp(midc_stkpop(stk), t) == 0 );
  }
  md_assertm( source, midc_stksize(stk) == 1 );
  md_assertm( source, !midc_stkisempty(stk) );
  md_assertm( source, strcmp(midc_stkpeek(stk), s) == 0 );
  
  md_assertm( source, strcmp(midc_stkpop(stk), s) == 0 );
  md_assertm( source, midc_stksize(stk) == 0 );
  md_assertm( source, midc_stkisempty(stk) );
  md_assertm( source, midc_stkpeek(stk) == NULL );

  return 0;
}
