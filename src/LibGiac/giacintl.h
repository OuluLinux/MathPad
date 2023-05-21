#ifndef _GIACINTL_H
#define _GIACINTL_H

#include "config.h"
#include "first.h"

#ifndef FIR
#if (defined(__APPLE__) && !defined(INT128))
#include <libintl.h>
#ifndef _LIBINTL_H
#define _LIBINTL_H      1
#endif
#endif
#endif

#ifndef _LIBINTL_H
#define _LIBINTL_H      1
#define __LIBINTL_H_DEFINED__ // Pour NetBSD
#if defined GIAC_HAS_STO_38 || defined EMCC
const char * gettext(const char * s); // in aspen.cc or opengl.cc
#else
inline const char * gettext(const char * s) { return s; };
#endif
#endif


#endif
