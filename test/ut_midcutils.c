#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "midcutils.h"
#include "midcunit.h"

#define TEST_FILE "test.out"

int foo = 7;
int bar = 8;

void final_teardown();

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
  md_assert(bar == bar);
  md_assertm("foo should not equal bar", foo != bar);
  return 0;
}

static char * test_mid_strrev() {
  md_assert(bar == bar);
  md_assertm("foo should not equal bar", foo != bar);
  return 0;
}

static char * test_mid_stricmp() {
  md_assert(bar == bar);
  md_assertm("foo should not equal bar", foo != bar);
  return 0;
}

static char * test_mid_ends_with() {
  md_assert(bar == bar);
  md_assertm("foo should not equal bar", foo != bar);
  return 0;
}

static char * test_mid_starts_with() {
  md_assert(bar == bar);
  md_assertm("foo should not equal bar", foo != bar);
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


void final_teardown() {
  remove(TEST_FILE);
}
