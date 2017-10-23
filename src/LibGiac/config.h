#include <uppconfig.h>


/* Set if debugging is enabled */
#define DEBUG_SUPPORT /**/

/* Define if <ext/hash_map> header is aviailable */
#define EXT_HASH_MAP 1

/* Define if your <locale.h> file defines LC_MESSAGES. */
#define HAVE_LC_MESSAGES 1

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the `rt' library (-lrt). */
#if !defined NDLESS && !defined flagWIN32
#define HAVE_LIBRT 1

/* Define to 1 if you have the <sys/times.h> header file. */
#define HAVE_SYS_TIMES_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

#else
#define HAVE_NO_SYS_TIMES_H
#endif

#if defined flagWIN32 && !defined flagMSC
#define HAVE_SYS_TIME_H 1
#else

#endif

/* Define to 1 if you have the `z' library (-lz). */
#define HAVE_LIBZ 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define if you have the 'long double' type. */
#define HAVE_LONG_DOUBLE 1


#ifdef flagMSC
#define HAVE_NO_CWD 1
#define HAVE_NO_SIGNAL_H 1
#define HAVE_NO_HOME_DIRECTORY 1
#define WIN32 1
#endif

/* Define to 1 if you have the <pwd.h> header file. */
#ifndef flagWIN32
#define HAVE_PWD_H 1
#else
#undef HAVE_NO_HOME_DIRECTORY
#define BESTA_OS 1
#define BESTA_WIN32_TARGET 1
#define HAVE_NO_HOME_DIRECTORY 1
#define HAVE_NO_SYS_RESOURCE_WAIT_H 1
#ifndef HAVE_NO_SYS_TIMES_H
#define HAVE_NO_SYS_TIMES_H 1
#endif
#define __MINGW_H 1
#ifdef flagMSC
#define VISUALC 1
#else
#define _isnan(x) isnan(x)
#endif
#endif

/* Define to 1 if you have the <signal.h> header file. */
#define HAVE_SIGNAL_H 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the <sstream> header file. */
#define HAVE_SSTREAM 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define if <stdint.h> exists, doesn't clash with <sys/types.h>, and declares
   uintmax_t. */
#define HAVE_STDINT_H_WITH_UINTMAX 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `stpcpy' function. */
#define HAVE_STPCPY 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strtoul' function. */
#define HAVE_STRTOUL 1

/* Define to 1 if you have the `sysconf' function. */
#define HAVE_SYSCONF 1

/* Define to 1 if you have the `system' function. */
#define HAVE_SYSTEM 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the `tsearch' function. */
#define HAVE_TSEARCH 1

/* Define if you have the 'uintmax_t' type in <stdint.h> or <inttypes.h>. */
#define HAVE_UINTMAX_T 1

/* Define to 1 if you have the <unistd.h> header file. */
#ifndef flagMSC
#define HAVE_UNISTD_H 1
#endif

/* Define if you have the 'unsigned long long' type. */
#define HAVE_UNSIGNED_LONG_LONG 1

/* Define if you have the 'wchar_t' type. */
#define HAVE_WCHAR_T 1

/* Define to 1 if you have the `wcslen' function. */
#define HAVE_WCSLEN 1

/* Define if you have the 'wint_t' type. */
#define HAVE_WINT_T 1

/* Define as const if the declaration of iconv() needs const. */
#define ICONV_CONST

/* Define if integer division by zero raises signal SIGFPE. */
#define INTDIV0_RAISES_SIGFPE 0

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "giac"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "giac"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "giac 1.2.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "giac"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.2.1"

/* Define if <inttypes.h> exists and defines unusable PRI* macros. */
/* #undef PRI_MACROS_BROKEN */

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if <tr1/unordered_map> header is available */
#define UNORDERED_MAP 1

/* Replace GMP by tommath */
#ifndef USE_GMP_REPLACEMENTS
#define USE_GMP_REPLACEMENTS
#endif

/* Version number of package */
#define VERSION "1.2.1"



#if defined(flagWIN32) && defined(flagGCC)
#include <sys/types.h>
#endif

#if defined(flagWIN32) && defined(flagMSC)
#define VISUALC 1
#endif



#if !defined DOUBLEVAL && (defined __amd64 || defined __x86_64 || defined _WIN64)
#define DOUBLEVAL 1
#define __x86_64__ 1



#endif

