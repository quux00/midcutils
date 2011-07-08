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

/**
 * Copies the string passed in into a new char array
 * on the heap (via malloc) and returns a pointer to 
 * that new string, so the new string should be free'd
 * at a later time.

 * @param str string to copy/duplicate
 */
char *mid_strdup(const char *str);

/**
 * Reverse a string in place, leaving the null character
 * in place.  Thus it will change the string you pass in.
 * @param str to change in place - Note *cannot* be a string
 *            literal, since it will modified in place
 * @return Pointer to string passed in, now reversed
 */
char *mid_strrev(char *str);

/**
 * Case insensitive version of strcmp. Looks for a complete
 * match, character by character, except for case differences.
 * Behaves same as strcmp for inputs and output.
 * @param s1 first string to compare
 * @param s2 second string to compare
 * @return -1 if s1 < s2, +1 if s1 > s2 and 0 if s1 equals s2
 */
int mid_stricmp(const char *s1, const char *s2);

/**
 * Checks whether a string ends with the suffix
 * provided.  Must be an exact match (i.e., search
 * is case-sensitive).
 * @param s1 string template to compare
 * @param s2 suffix to check against end of string template
 */
bool mid_ends_with(const char *str, const char *suffix);

/**
 * Checks whether a string starts with with the prefix
 * provided.  Must be an exact match (i.e., search
 * is case-sensitive).
 * @param s1 string template to compare
 * @param s2 prefix to check against start of string template
 */
bool mid_starts_with(const char *str, const char *prefix);

/**
 * Creates a lower case version of the string passed in.
 * The string returned was created via malloc(3) and can
 * be freed with free(3).
 * @param str string to create a lower case version of
 * @return new string on the heap with lower case value of str
 */
char *mid_strlower(const char *str);

/**
 * Creates an upper case version of the string passed in.
 * The string returned was created via malloc(3) and can
 * be freed with free(3).
 * @param str string to create a upper case version of
 * @return new string on the heap with upper case value of str
 */
char *mid_strupper(const char *str);

/**
 * Creates a string representation of the integer passed in.
 * The string returned was created via malloc(3) and can
 * be freed with free(3).
 * @param n integer to create the string representation from
 * @return new string on the heap with string value of n
 */
char *itoa(int n);

//~TODO: None of this has been tested yet
/* #define MALLOC(sz) (malloc(sz) || fputs("Unable to allocate heap memory in call to malloc.", stderr) && exit(EXIT_FAILURE)) */


#endif
