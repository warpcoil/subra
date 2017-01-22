/* Project:     OSLib
 * Description: The OS Construction Kit
 * Date:                1.6.2000
 * Idea by:             Luca Abeni & Gerardo Lamastra
 *
 * OSLib is an SO project aimed at developing a common, easy-to-use
 * low-level infrastructure for developing OS kernels and Embedded
 * Applications; it partially derives from the HARTIK project but it
 * currently is independently developed.
 *
 * OSLib is distributed under GPL License, and some of its code has
 * been derived from the Linux kernel source; also some important
 * ideas come from studying the DJGPP go32 extender.
 *
 * We acknowledge the Linux Community, Free Software Foundation,
 * D.J. Delorie and all the other developers who believe in the
 * freedom of software and ideas.
 *
 * For legalese, check out the included GPL license.
 */

/*	String manipulation functions	*/

#ifndef __STRING_H__
#define __STRING_H__

#include "mem.h"

#include "defs.h"
BEGIN_DEF

/* Various string manipulation functions */

/* File: String.C		*/
#ifdef __cplusplus
extern "C" {
#endif

char *strcpy(char *dst,const char *src);
char *strncpy(char *dst,const char *src,int n);
int strcmp(const char *s1,const char *s2);
int strncmp(const char *s1,const char *s2,int n);
int strlen(const char *s);
char *strscn(const char *s,char *pattern);
char *strchr(const char *s,int c);
char *strrchr (const char *s, int c);
char *strupr(char *s);
char *strlwr(char *s);
char *strcat(char *dst, const char *src);
long unsigned strtoul(const char *s, char **scan_end, int base);
long strtol(const char* s, char** scan_end, int base);
char *strstr (const char * s, const char * e);

int atoi(const char * s);

//ctype
int tolower(int c);
int tonumber(char c);
int isalpha(int c);
int isdigit(int c);
int isspace(int c);
int isalnum(int c);
int isxdigit(int c);
int isprint(int c);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

END_DEF

#endif

