#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "midcutils.h"
#include "midcunit.h"

#define TEST_FILE "test.out"

static void final_teardown(void);

/* unit test for midcutils.c */

static char * test_mid_malloc(void) {
  char *str = mid_malloc(128);
  md_assert(str != NULL);

  char *p = mid_malloc(3);
  md_assert(p != NULL);
  strcpy(p, "hi");
  md_assert(strcmp(p, "hi") == 0);
  md_assert(strlen(p) == 2);
  free(p);
  
  FILE *fp = freopen(TEST_FILE, "w", stderr);
  md_assert(fp != NULL);

  p = mid_malloc(-1);
  md_assert(p == NULL);
  fclose(fp);

  fp = fopen(TEST_FILE, "r");
  char *s = fgets(str, 128, fp);
  md_assert(s != NULL);
  md_assert(s == str);
  md_assert(strcmp("Unable to allocate heap memory in call to malloc.\n", str) == 0);
  fclose(fp);
  free(str);
  return 0;
}

static char * test_mid_calloc(void) {
  char *str = mid_calloc(1, 128);
  md_assert(str != NULL);

  char *p = mid_calloc(3, sizeof(int));
  md_assert(p != NULL);
  p[0] = 1;
  p[1] = 2;
  p[2] = 3;
  md_assert((p[0] + p[1] + p[2]) == 6);
  free(p);
  
  FILE *fp = freopen(TEST_FILE, "w", stderr);
  md_assert(fp != NULL);

  p = mid_calloc(1, -1);
  md_assert(p == NULL);
  fclose(fp);

  fp = fopen(TEST_FILE, "r");
  char *s = fgets(str, 128, fp);
  md_assert(s != NULL);
  md_assert(s == str);
  md_assert(strcmp("Unable to allocate and zero out heap memory in call to calloc.\n", str) == 0);
  fclose(fp);
  free(str);
  return 0;
}

static char * test_mid_strdup(void) {

  // check static string
  char *tmpl1 = "lefse is Scandinavian";
  char *p = strdup(tmpl1);
  md_assert(p != NULL);
  md_assert(strlen(p) == strlen(tmpl1));
  md_assert(strcmp(tmpl1, p) == 0);
  free(p);

  // check string on the heap
  char *tmpl2 = mid_malloc(4000);
  strcpy(tmpl2, tmpl1);
  strcat(tmpl2, ". Pizza is American. Tacos are Mexican. Beer is universal.");
  p = mid_strdup(tmpl2);
  md_assert(p != NULL);
  md_assert(strlen(p) == strlen(tmpl2));
  md_assert(strcmp(tmpl2, p) == 0);
  free(p);
  free(tmpl2);

  //check empty string
  p = mid_strdup("");
  md_assert(p != NULL);
  md_assert(strlen(p) == 0);
  md_assert(strcmp("", p) == 0);
  free(p);

  return 0;
}

static char * test_mid_strrev(void) {
  // check non-palindrome
  char *p = mid_strdup("level123");
  char *q = mid_strrev(p);
  md_assert(p == q); // should be the same pointer ref
  md_assert(strcmp(p, q) == 0);
  md_assert(strlen(p) == 8);
  md_assert(strcmp(p, "321level") == 0);
  free(p);

  // check palindrome
  p = mid_strdup("able was I ere I saw elba");
  q = mid_strrev(p);
  md_assert(p == q); // should be the same pointer ref
  md_assert(strcmp(p, q) == 0);
  md_assert(strcmp(p, "able was I ere I saw elba") == 0);
  free(p);

  // check one-char string
  p = mid_strdup("a");
  q = mid_strrev(p);
  md_assert(p == q); // should be the same pointer ref
  md_assert(strlen(p) == 1);
  md_assert(strcmp(p, q) == 0);
  md_assert(strcmp(p, "a") == 0);
  free(p);

  // check empty string
  char a[1];
  a[0] = '\0';
  q = mid_strrev(a);
  md_assert(a == q); // should be the same pointer ref
  md_assert(strlen(a) == 0);
  md_assert(strcmp(a, q) == 0);
  md_assert(strcmp(a, "") == 0);
  
  return 0;
}

static char * test_mid_stricmp(void) {
  char *p = "HI MOM";
  char *q = "HI mom";
  md_assert( mid_stricmp(p,q) == 0 );
  md_assert( mid_stricmp(p,"hi momm") < 0 );
  md_assert( mid_stricmp("HI MOMM", p) > 0 );
  md_assert( mid_stricmp("", p) < 0 );
  md_assert( mid_stricmp("", "") == 0 );
  return 0;
}

static char * test_mid_ends_with(void) {
  char *p = "You don't understand something until you think it's obvious.";
  char *q = "1234 You don't understand something until you think it's obvious.";
  char *s = "1ou don't understand something until you think it's obvious.";

  md_assert( !mid_ends_with(p, "OBVIOUS.") );
  md_assert( !mid_ends_with(p, "obvious") );
  md_assert(  mid_ends_with(p, "obvious.") );
  md_assert(  mid_ends_with(p, "s.") );
  md_assert(  mid_ends_with(p, ".") );
  md_assert(  mid_ends_with(p, "") );
  md_assert(  mid_ends_with(p, p) );
  md_assert( !mid_ends_with(p, q) );
  md_assert( !mid_ends_with(p, s) );

  return 0;
}

static char * test_mid_starts_with(void) {
  char *p = "You don't understand something until you think it's obvious.";
  char *q = "1234 You don't understand something until you think it's obvious.";
  char *s = "1ou don't understand something until you think it's obvious.";

  md_assert( !mid_starts_with(p, "YOU") );
  md_assert( !mid_starts_with(p, "You people") );
  md_assert(  mid_starts_with(p, "You do") );
  md_assert(  mid_starts_with(p, "Yo") );
  md_assert(  mid_starts_with(p, "Y") );
  md_assert(  mid_starts_with(p, "") );
  md_assert(  mid_starts_with(p, p) );
  md_assert( !mid_starts_with(p, q) );
  md_assert( !mid_starts_with(p, s) );

  return 0;
}

static char * test_mid_strlower(void) {
  char *p      = "You don't Understand SOMETHING until you think it's obvious.";
  char *plower = "you don't understand something until you think it's obvious.";
  char *q = "123";
  char *s = "E";
  char *t = "";

  char *pl = mid_strlower(p);
  char *ql = mid_strlower(q);
  char *sl = mid_strlower(s);
  char *tl = mid_strlower(t);

  md_assert( pl != NULL );
  md_assert( ql != NULL );
  md_assert( sl != NULL );
  md_assert( tl != NULL );

  md_assert( strcmp(pl, plower) == 0 );
  md_assert( strcmp(ql, q) == 0 );
  md_assert( strcmp(sl, "e") == 0 );
  md_assert( strcmp(tl, t) == 0 );

  free(pl);
  free(ql);
  free(sl);
  free(tl);

  return 0;
}


static char * test_mid_strupper(void) {
  char *p      = "You don't Understand SOMETHING until you think it's obvious.";
  char *pupper = "YOU DON'T UNDERSTAND SOMETHING UNTIL YOU THINK IT'S OBVIOUS.";
  char *q = "123";
  char *s = "e";
  char *t = "";

  char *pl = mid_strupper(p);
  char *ql = mid_strupper(q);
  char *sl = mid_strupper(s);
  char *tl = mid_strupper(t);

  md_assert( pl != NULL );
  md_assert( ql != NULL );
  md_assert( sl != NULL );
  md_assert( tl != NULL );

  md_assert( strcmp(pl, pupper) == 0 );
  md_assert( strcmp(ql, q) == 0 );
  md_assert( strcmp(sl, "E") == 0 );
  md_assert( strcmp(tl, t) == 0 );

  free(pl);
  free(ql);
  free(sl);
  free(tl);

  return 0;
}

static char * test_mid_itoa(void) {
  char *s1 = mid_itoa(77);
  md_assert( strlen(s1) == 2 );
  md_assert( strcmp(s1, "77") == 0 );

  char *s2 = mid_itoa(-111114);
  md_assert( strlen(s2) == 7 );
  md_assert( strcmp(s2, "-111114") == 0 );

  char *s3 = mid_itoa(INT_MAX);
  md_assert( strlen(s3) > 9 );
  int max_int = (int) strtol(s3, NULL, 10);
  md_assert( max_int == INT_MAX );

  char *s4 = mid_itoa(0);
  md_assert( strlen(s4) == 1 );
  md_assert( strcmp(s4, "0") == 0 );

  free(s1);
  free(s2);
  free(s3);
  free(s4);
  return 0;
}

static char * test_mid_ltoa(void) {
  char *s1 = mid_ltoa((long int)77);
  md_assert( strlen(s1) == 2 );
  md_assert( strcmp(s1, "77") == 0 );

  char *s2 = mid_ltoa(-11551114);
  md_assert( strlen(s2) == 9 );
  md_assert( strcmp(s2, "-11551114") == 0 );

  char *s3 = mid_ltoa(LONG_MAX);
  md_assert( strlen(s3) > 9 );
  long int max_long = strtol(s3, NULL, 10);
  md_assert( max_long == LONG_MAX );

  free(s1);
  free(s2);
  free(s3);
  return 0;
}

static void all_tests(void) {
  md_run_test(test_mid_malloc);
  md_run_test(test_mid_calloc);
  md_run_test(test_mid_strdup);
  md_run_test(test_mid_strrev);
  md_run_test(test_mid_stricmp);
  md_run_test(test_mid_ends_with);
  md_run_test(test_mid_starts_with);
  md_run_test(test_mid_strlower);
  md_run_test(test_mid_strupper);
  md_run_test(test_mid_itoa);
  md_run_test(test_mid_ltoa);
  final_teardown();
}

int main() {
  RUN_TESTS(all_tests);
}


static void final_teardown(void) {
  remove(TEST_FILE);
}
