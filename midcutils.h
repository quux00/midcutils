#ifndef MIDUTILS_H
#define MIDUTILS_H

/* "put" arguments to supplement puts */
/* includes "debug" versions that start with 'd' */
#define PUTI(x)  printf("%d\n", x)
#define dPUTI(x) printf(#x " = %d\n", x)

#define PUTL(x)  printf("%ld\n", x)
#define dPUTL(x) printf(#x " = %ld\n", x)

#define PUTF(x)  printf("%.2f\n", x)
#define dPUTF(x) printf(#x " = %.2f\n", x)

#define PUTC(x)  printf("%c\n", x)
#define dPUTC(x) printf(#x " = %c\n", x)

#define dPUTS(x) printf(#x " = %s\n", x)

#define Str(x) #x

#include <stdlib.h>
#include <stdbool.h>

void *mid_malloc(size_t size);
void *mid_calloc(size_t nmemb, size_t size);
char *mid_strdup(const char *str);
char *mid_strrev(char *str);

/**
 * Case insensitive version of strcmp.
 * Behaves same as strcmp for inputs and output.
 * @param s1 first string to compare
 * @param s2 second string to compare
 */
int mid_stricmp(const char *s1, const char *s2);
bool mid_ends_with(const char *str, const char *suffix);
bool mid_starts_with(const char *str, const char *suffix);

//~TODO: None of this has been tested yet
/* #define MALLOC(sz) (malloc(sz) || fputs("Unable to allocate heap memory in call to malloc.", stderr) && exit(EXIT_FAILURE)) */


#endif
