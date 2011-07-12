#include <stdlib.h>
#include <string.h>
#include "midcunit.h"
#include "midcqueue.h"

/**
 * Unit test for the midc_queue.c library.
 * It uses the midcunit testing framework.
 * Author: Michael Peterson
 * July 2011
 */

// helper functions
static char * do_well_behaved_test(char *source, struct midc_queue *q, int sz);
static char * do_overflow(char *source, struct midc_queue *q, int sz);

static char * test_well_behaved_create_own() {
  char *rt;
  int sz = 999999;
  struct midc_queue q;
  midc_qcreate(&q, sz);
  rt = do_well_behaved_test("create-own", &q, sz);
  midc_qdispose(&q);
  return rt;
}

static char * test_well_behaved_library_creates() {
  char *rt;
  int sz = 13;
  struct midc_queue *q = midc_qcreate(NULL, sz);
  rt = do_well_behaved_test("library-creates", q, sz);
  midc_qdispose(q);
  return rt;
}

static char * test_underflow() {
  int sz = 4;
  struct midc_queue *q = midc_qcreate(NULL, sz);
  char *s;

  md_assert( midc_qsize(q) == 0 );
  errno = 0;
  s = midc_qdequeue(q);
  md_assert( errno == ERANGE );
  md_assert( s == NULL );
  errno = 0;
  s = midc_qpeek(q);
  md_assert( errno == ERANGE );
  md_assert( s == NULL );

  midc_qenqueue(q, "hi mom");
  midc_qenqueue(q, "hi dad");
  md_assert( midc_qsize(q) == 2 );
  md_assert( strcmp(midc_qpeek(q), "hi mom") == 0 );
  errno = 0;
  s = midc_qdequeue(q);
  md_assert( errno == 0 );
  errno = 0;
  s = midc_qdequeue(q);
  md_assert( errno == 0 );
  md_assert( midc_qsize(q) == 0 );
  md_assert( midc_qisempty(q) );
  md_assert( strcmp(s, "hi dad") == 0 );

  errno = 0;
  s = midc_qdequeue(q);
  md_assert( errno == ERANGE );
  md_assert( s == NULL );
  errno = 0;
  s = midc_qpeek(q);
  md_assert( errno == ERANGE );
  md_assert( s == NULL );

  midc_qdispose(q);
  return 0;
}

static char * test_overflow_create_own() {
  int sz = 4;
  struct midc_queue q;
  midc_qcreate(&q, sz);
  do_overflow("create-own", &q, sz);
  midc_qdispose(&q);
  return 0;
}

static char * test_overflow_library_creates() {
  int sz = 4;
  struct midc_queue *q;
  q = midc_qcreate(NULL, sz);
  do_overflow("library-create", q, sz);
  midc_qdispose(q);
  return 0;
}

// all previous tests enqueued strings, now try a numeric type

static char * test_int_queue() {
  int sz = 4;
  struct midc_queue *q = midc_qcreate(NULL, sz);

  int i, j, k, m;
  i = 11;
  j = 12;
  k = 13;
  m = 14;

  midc_qenqueue(q, &i);
  midc_qenqueue(q, &j);
  midc_qenqueue(q, &k);

  md_assert(  midc_qsize(q) == 3 );
  md_assert( !midc_qisempty(q) );
  md_assert(  midc_qpeek(q) == &i );

  midc_qenqueue(q, &m);
  midc_qenqueue(q, &m);
  midc_qenqueue(q, &m);
  midc_qenqueue(q, &m);

  md_assert(  midc_qsize(q) == 7 );
  md_assert( !midc_qisempty(q) );
  md_assert(  midc_qpeek(q) == &i );

  int *p;
  p = midc_qdequeue(q);
  md_assert( midc_qsize(q) == 6 );
  md_assert( *p == 11 );

  p = midc_qdequeue(q);
  md_assert( midc_qsize(q) == 5 );
  md_assert( *p == 12 );

  p = midc_qdequeue(q);
  md_assert( midc_qsize(q) == 4 );
  md_assert( *p == 13 );

  p = midc_qdequeue(q);
  md_assert( midc_qsize(q) == 3 );
  md_assert( *p == 14 );

  p = midc_qdequeue(q);
  md_assert( midc_qsize(q) == 2 );
  md_assert( *p == 14 );

  p = midc_qdequeue(q);
  md_assert( midc_qsize(q) == 1 );
  md_assert( !midc_qisempty(q) );
  md_assert( *p == 14 );

  p = midc_qdequeue(q);
  md_assert( midc_qsize(q) == 0 );
  md_assert( midc_qisempty(q) );
  md_assert( *p == 14 );

  p = midc_qdequeue(q);
  md_assert( midc_qsize(q) == 0 );
  md_assert( p == NULL );

  midc_qdispose(q);
  return 0;
}

// now try a newly defined type

typedef struct {
  char *str;
  int size;
} String;

static char * test_struct_String_on_queue() {
  int sz = 30;
  struct midc_queue *q = midc_qcreate(NULL, sz);

  String s1;
  String s2;

  s1.str = malloc(20);
  strcpy(s1.str, "hi mom");
  s1.size = strlen(s1.str);

  s2.str = malloc(40);
  strcpy(s2.str, "Papa was a Rolling Stone ...");
  s2.size = strlen(s2.str);

  midc_qenqueue(q, &s1);
  midc_qenqueue(q, &s1);
  midc_qenqueue(q, &s2);

  md_assert(  midc_qsize(q) == 3 );
  md_assert( !midc_qisempty(q) );
  md_assert(  midc_qpeek(q) == &s1 );

  midc_qenqueue(q, &s2);

  md_assert(  midc_qsize(q) == 4 );
  md_assert( !midc_qisempty(q) );
  md_assert(  midc_qpeek(q) == &s1 );

  String *p;
  p = midc_qdequeue(q);
  md_assert( midc_qsize(q) == 3 );
  md_assert( p->size == s1.size );
  md_assert( strcmp(p->str, s1.str) == 0 );

  p = midc_qdequeue(q);
  md_assert( midc_qsize(q) == 2 );
  md_assert( p->size == s1.size );
  md_assert( strcmp(p->str, s1.str) == 0 );

  p = midc_qdequeue(q);
  md_assert( midc_qsize(q) == 1 );
  md_assert( p->size == s2.size );
  md_assert( strcmp(p->str, s2.str) == 0 );

  free(s1.str);
  free(s2.str);
  midc_qdispose(q);
  return 0;
}

static void all_tests() {
  md_run_test(test_well_behaved_create_own);
  md_run_test(test_well_behaved_library_creates);
  md_run_test(test_underflow);
  md_run_test(test_overflow_create_own);
  md_run_test(test_overflow_library_creates);
  md_run_test(test_int_queue);
  md_run_test(test_struct_String_on_queue);
}

int main() {
  RUN_TESTS(all_tests);
}

/* --- HELPER FUNCTIONS --- */
static char * do_overflow(char *source, struct midc_queue *q, int sz) {
  int i;
  for (i = 0; i < sz; i++) {
    midc_qenqueue(q, "first string");
  }
  md_assertm( source, midc_qsize(q) == sz );
  md_assertm( source, strcmp(midc_qdequeue(q), "first string") == 0 );
  md_assertm( source, midc_qsize(q) == sz-1 );

  midc_qenqueue(q, "second string");
  md_assertm( source, midc_qsize(q) == sz );
  // this one will cause a resize
  midc_qenqueue(q, "second string");
  md_assertm( source, midc_qsize(q) == sz+1 );
  md_assertm( source, strcmp(midc_qpeek(q), "first string") == 0 );

  for (i = 0; i < sz; i++) {
    midc_qenqueue(q, "second string");
  }
  md_assertm( source, midc_qsize(q) == 9 );
  md_assertm( source, strcmp(midc_qdequeue(q), "first string") == 0 );
  md_assertm( source, strcmp(midc_qdequeue(q), "first string") == 0 );
  md_assertm( source, midc_qsize(q) == 7 );

  midc_qenqueue(q, "third string");
  md_assertm( source, strcmp(midc_qdequeue(q), "first string") == 0 );
  midc_qenqueue(q, "second string");
  midc_qenqueue(q, "second string");
  midc_qenqueue(q, "second string");
  md_assertm( source, midc_qsize(q) == 10 );
  md_assertm( source, strcmp(midc_qpeek(q), "second string") == 0 );

  // this will cause another resize
  for (i = 0; i < 20; i++) {
    midc_qenqueue(q, "third string");
  }
  md_assertm( source, midc_qsize(q) == 30 );
  md_assertm( source, strcmp(midc_qpeek(q), "second string") == 0 );

  // this causes r to wrap around and be "less" than f
  md_assertm( source, strcmp(midc_qdequeue(q), "second string") == 0 );
  md_assertm( source, strcmp(midc_qdequeue(q), "second string") == 0 );
  md_assertm( source, strcmp(midc_qdequeue(q), "second string") == 0 );
  md_assertm( source, strcmp(midc_qdequeue(q), "second string") == 0 );
  md_assertm( source, strcmp(midc_qdequeue(q), "second string") == 0 );
  md_assertm( source, strcmp(midc_qdequeue(q), "second string") == 0 );
  md_assertm( source, midc_qsize(q) == 24 );
  md_assertm( source, strcmp(midc_qpeek(q), "third string") == 0 );

 for (i = 0; i < 15; i++) {
  midc_qenqueue(q, "fourth string");
 }
  md_assertm( source, midc_qsize(q) == 39 );
  md_assertm( source, strcmp(midc_qpeek(q), "third string") == 0 );

  // this will cause another resize
  midc_qenqueue(q, "fifth and last string!!");
  md_assertm( source, midc_qsize(q) == 40 );
  md_assertm( source, !midc_qisempty(q) );
  md_assertm( source, strcmp(midc_qpeek(q), "third string") == 0 );
  return 0;
}

static char * do_well_behaved_test(char *source, struct midc_queue *q, int sz) {
  // library creates midc_queue on the heap
  char *rt;
  char *s = "string 1234";
  char t[] = "5555555555!! string 1234 asdfqwerasdf EOL";
  char *u = "";

  md_assertm( source, midc_qisempty(q) );
  md_assertm( source, midc_qsize(q) == 0 );
  md_assertm( source, midc_qpeek(q) == NULL );  
  midc_qenqueue(q, s);
  md_assertm( source, midc_qsize(q) == 1 );
  md_assertm( source, !midc_qisempty(q) );
  md_assertm( source, strcmp(midc_qpeek(q), s) == 0 );
  rt = midc_qdequeue(q);
  md_assertm( source, midc_qsize(q) == 0 );
  md_assertm( source, midc_qisempty(q) );
  md_assertm( source, strcmp(s, rt) == 0 );
  
  midc_qenqueue(q, s);
  md_assertm( source, midc_qsize(q) == 1 );
  midc_qenqueue(q, t);
  md_assertm( source, midc_qsize(q) == 2 );
  midc_qenqueue(q, u);
  md_assertm( source, midc_qsize(q) == 3 );
  md_assertm( source, ! midc_qisempty(q) );
  md_assertm( source, strcmp(midc_qpeek(q), s) == 0 );

  md_assertm( source, strcmp(midc_qdequeue(q), s) == 0 );
  md_assertm( source, midc_qsize(q) == 2 );
  md_assertm( source, strcmp(midc_qpeek(q), t) == 0 );

  md_assertm( source, strcmp(midc_qdequeue(q), t) == 0 );
  md_assertm( source, midc_qsize(q) == 1 );
  md_assertm( source, strcmp(midc_qpeek(q), u) == 0 );

  int i;
  for (i = 0; i < sz - 1; i++) {
    midc_qenqueue(q, s);    
  }
  md_assertm( source, !midc_qisempty(q) );
  md_assertm( source, midc_qsize(q) == sz );
  md_assertm( source, strcmp(midc_qpeek(q), u) == 0 );

  md_assertm( source, strcmp(midc_qdequeue(q), u) == 0 );
  md_assertm( source, midc_qsize(q) == sz-1 );
  md_assertm( source, strcmp(midc_qpeek(q), s) == 0 );
  
  for (i = sz-2; i > 0; --i) {
    md_assertm( source, strcmp(midc_qdequeue(q), s) == 0 );
  }
  md_assertm( source, midc_qsize(q) == 1 );
  md_assertm( source, !midc_qisempty(q) );
  md_assertm( source, strcmp(midc_qpeek(q), s) == 0 );
  
  md_assertm( source, strcmp(midc_qdequeue(q), s) == 0 );
  md_assertm( source, midc_qsize(q) == 0 );
  md_assertm( source, midc_qisempty(q) );
  md_assertm( source, midc_qpeek(q) == NULL );

  return 0;
}
