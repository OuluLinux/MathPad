#ifndef _GIAC_RISCH_H
#define _GIAC_RISCH_H

#include "first.h"
#include <string>
#include "vector.h"


namespace giac {

class gen;
class identificateur;
struct unary_function_ptr;

gen risch(const gen & e_orig, const identificateur & x, gen & remains_to_integrate, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_risch;
gen _risch(const gen & g, GIAC_CONTEXT);

// integer roots of a polynomial
vecteur iroots(const polynome & p);

}



#endif // _GIAC_RISCH_H
