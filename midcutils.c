#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "midcutils.h"

void *mid_malloc(size_t size) {
  void *p;
  if ((p = malloc(size)) == NULL) {
    fputs("Unable to allocate heap memory in call to malloc.\n", stderr);
  }
  return p;
}

void *mid_calloc(size_t nmemb, size_t size) {
  void *p;
  if ((p = calloc(nmemb, size)) == NULL) {
    fputs("Unable to allocate and zero out heap memory in call to calloc.\n", stderr);
  }
  return p;
}


char *mid_strdup(const char *str) {
  int len = (int)strlen(str) + 1;
  char *p = malloc(len);
  if (p == NULL) return NULL;
  return (char *)memcpy(p, str, len);
}


char *mid_strrev(char *str) {
  int i, len = strlen(str);
  char buf, *p = str + (len - 1);
  len /= 2;
  for (i = 0; i < len; i++, --p) {
    buf = str[i];
    str[i] = *p;
    *p = buf;
  }
  return str;
}


int mid_stricmp(const char *s1, const char *s2) {
  int diff;
  const char *p1 = s1;
  const char *p2 = s2;
  
  for (; *p1 && *p2; p1++, p2++) {
    diff = tolower(*p1) - tolower(*p2);
    if (diff != 0) return diff;
  }

  if (*p1 == *p2) return 0;
  else if (*p1 == '\0') return -1;
  else return 1;
}


bool mid_ends_with(const char *str, const char *suffix) {
  int len = strlen(str);
  int suflen = strlen(suffix);
  if (suflen > len) return false;
  
  for (str += (len - suflen); *suffix; suffix++, str++) {
    if (*str != *suffix) return false;
  }
  return true;
}

bool mid_starts_with(const char *str, const char *prefix) {
  if (strlen(prefix) > strlen(str)) return false;
  
  for (; *prefix; prefix++, str++) {
    if (*str != *prefix) return false;
  }
  return true;
}


/**
 * strlower and strupper do the same thing except for which 
 * case-change method to call, so abstract the functionality
 * into one function and have each externally visible function
 * call into this one, passing a reference to the case 
 * changing function to use.
 */
static char *_mid_strsamecase(const char *str, int (*f)(int));

static char *_mid_strsamecase(const char *str, int (*f)(int)) {
  int len = strlen(str);
  char *strsame = malloc(len + 1);
  if (strsame == NULL) return NULL;
  strsame[len] = '\0';  /* make sure string is null-char terminated */

  char *p;
  for (p = strsame; *str; p++, str++) {
    if (isalpha((int)*str)) *p = (char)f((int)*str);
    else                    *p = *str;
  }
  return strsame;
}

char *mid_strlower(const char *str) {
 return _mid_strsamecase(str, tolower);
}

char *mid_strupper(const char *str) {
 return _mid_strsamecase(str, toupper);
}

char *mid_itoa(int n) {
  return mid_ltoa((long int)n);
}

char *mid_ltoa(long int n) {
  int sz = 11;
  if (LONG_MAX < 2147483647) sz = 20; // handle 64-bit implementations
  char *s = malloc(sz);
  if (s == NULL) return NULL;
  sprintf(s, "%ld", n);
  return s;
}
