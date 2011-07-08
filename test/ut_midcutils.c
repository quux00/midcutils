#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "midcutils.h"
#include "midcunit.h"

#define TEST_FILE "test.out"

static void final_teardown();

/* unit test for midcutils.c */

static char * test_mid_malloc() {
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

static char * test_mid_calloc() {
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

static char * test_mid_strdup() {

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

static char * test_mid_strrev() {
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

static char * test_mid_stricmp() {
  char *p = "HI MOM";
  char *q = "HI mom";
  md_assert( mid_stricmp(p,q) == 0 );
  md_assert( mid_stricmp(p,"hi momm") < 0 );
  md_assert( mid_stricmp("HI MOMM", p) > 0 );
  md_assert( mid_stricmp("", p) < 0 );
  md_assert( mid_stricmp("", "") == 0 );
  return 0;
}

static char * test_mid_ends_with() {
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

static char * test_mid_starts_with() {
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

static void all_tests() {
  md_run_test(test_mid_malloc);
  md_run_test(test_mid_calloc);
  md_run_test(test_mid_strdup);
  md_run_test(test_mid_strrev);
  md_run_test(test_mid_stricmp);
  md_run_test(test_mid_ends_with);
  md_run_test(test_mid_starts_with);
  final_teardown();
}

int main() {
  RUN_TESTS(all_tests);
}


static void final_teardown() {
  remove(TEST_FILE);
}
