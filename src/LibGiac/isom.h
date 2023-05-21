#ifndef _GIAC_ISOM_H
#define _GIAC_ISOM_H

#include "first.h"
#include "gen.h"


namespace giac {


vecteur isom(const vecteur & M, GIAC_CONTEXT);
gen _isom(const gen & args, GIAC_CONTEXT);
gen _mkisom(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_isom ;

vecteur mkisom(const gen & n, int b, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_mkisom;


}


#endif // _GIAC_ISOM_H
