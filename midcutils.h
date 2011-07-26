#ifndef MIDUTILS_H
#define MIDUTILS_H

/* --- MACRO FUNCTIONS --- */

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

/* 
 * Use Str() to print a value as a string
 * Example:
 *  #include <limits.h>
 *  Str(INT_MAX)  #=> preprocessor will insert string value of INT_MAX
 * See C-FAQ for more details:
 *  http://c-faq.com/ansi/stringize.html
 */
#define XStr(x) #x      /* this one stringizes a value, rather than the MACRO name */
#define Str(x) XStr(x)  /* this one expands x to its actual value */

/* --- FUNCTIONS --- */

#include <stdlib.h>
#include <stdbool.h>


/**
 * A very thin wrapper around malloc that check the return
 * value and if it is NULL prints an error message to stderr.
 * Then it returns the return value from malloc to its calling
 * function, so you still have to check the return value before
 * proceeding, but you can skip printing the error message.
 * 
 * @param size amount of memory to allocate
 * @return pointer to memory on heap or NULL if malloc failed
 */
void *mid_malloc(size_t size);

/**
 * A very thin wrapper around calloc that check the return
 * value and if it is NULL prints an error message to stderr.
 * Then it returns the return value from calloc to its calling
 * function, so you still have to check the return value before
 * proceeding, but you can skip printing the error message.
 * 
 * @param nmemb number of entries of size 'size' to create
 * @param size of the primitive (e.g., int, char *) to create memory for
 * @return pointer to memory on heap or NULL if calloc failed
 */
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
char *mid_itoa(int n);

/**
 * Creates a string representation of the integer passed in.
 * The string returned was created via malloc(3) and can
 * be freed with free(3).
 * @param n integer to create the string representation from
 * @return new string on the heap with string value of n
 */
char *mid_ltoa(long int n);


/* --- HELPER METHODS FROM APUE --- */
/* APUE: Advanced Programming in the Unix Environment, v. 2 
 * link: http://www.apuebook.com/
 */

/* ~TODO: Not implemented yet */
void err_dump(const char *, ...);		/* {App misc_source} */
void err_msg(const char *, ...);
void err_quit(const char *, ...);
void err_exit(int, const char *, ...);
void err_ret(const char *, ...);
void err_sys(const char *, ...);

void log_msg(const char *, ...);			/* {App misc_source} */
void log_open(const char *, int, int);
void log_quit(const char *, ...);
void log_ret(const char *, ...);
void log_sys(const char *, ...);

//~TODO: None of this has been tested yet
/* #define MALLOC(sz) (malloc(sz) || fputs("Unable to allocate heap memory in call to malloc.", stderr) && exit(EXIT_FAILURE)) */

#endif
