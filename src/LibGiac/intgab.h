#ifndef _GIAC_INTGAB_H
#define _GIAC_INTGAB_H

#include "first.h"
#include <string>


namespace giac {


class gen;

class identificateur;
struct unary_function_ptr;

bool assume_t_in_ab(const gen & t, const gen & a, const gen & b, bool exclude_a, bool exclude_b, GIAC_CONTEXT);
// check whether an expression is meromorphic wrt x
int is_meromorphic(const gen & g, const gen & x, gen &a, gen &b, gen & P, GIAC_CONTEXT);
// 0 none, 1 even, 2 odd
int is_even_odd(const gen & f, const gen & x, GIAC_CONTEXT);
// residue of g at x=a
gen residue(const gen & g, const gen & x, const gen & a, GIAC_CONTEXT);
gen _residue(const gen & args, GIAC_CONTEXT);


// if true put int(g,x=a..b) into res
bool intgab(const gen & g, const gen & x, const gen & a, const gen & b, gen & res, GIAC_CONTEXT);
// if true put int(g,x=a..b) into res
bool sumab(const gen & g, const gen & x, const gen & a, const gen & b, gen & res, bool testi, GIAC_CONTEXT);

// singular values of g wrt x (in complex plan)
vecteur singular(const gen & g, const gen & x, GIAC_CONTEXT);
gen _singular(const gen & args, GIAC_CONTEXT);
// check whether P has only integer roots
bool is_admissible_poly(const polynome & P, int & deg, polynome & lcoeff, vecteur & roots, GIAC_CONTEXT);


}



#endif // _GIAC_INTGAB_H
