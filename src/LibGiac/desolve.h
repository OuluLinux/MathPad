#ifndef _GIAC_DESOLVE_H
#define _GIAC_DESOLVE_H

#include "first.h"
#include "gen.h"
#include "identificateur.h"


namespace giac {


// f must be a vector obtained using factors
// x, y are 2 idnt
// xfact and yfact should be initialized to 1
// return true if f=xfact*yfact where xfact depends on x and yfact on y only
bool separate_variables(const gen & f, const gen & x, const gen & y, gen & xfact, gen & yfact, GIAC_CONTEXT);

gen laplace(const gen & f, const gen & x, const gen & s, GIAC_CONTEXT);
gen _laplace(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_laplace ;
polynome cstcoeff(const polynome & p);
gen diffeq_constante(int i, GIAC_CONTEXT);

gen ilaplace(const gen & f, const gen & x, const gen & s, GIAC_CONTEXT);
gen _ilaplace(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_ilaplace ;

gen desolve(const gen & f, const gen & x, const gen & y, int & ordre, vecteur & parameters, GIAC_CONTEXT);
gen desolve_f(const gen & f_orig, const gen & x_orig, const gen & y_orig, int & ordre, vecteur & parameters, gen & f, GIAC_CONTEXT);
gen _desolve(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_desolve ;
gen ztrans(const gen & f, const gen & x, const gen & s, GIAC_CONTEXT);
gen _ztrans(const gen & args, GIAC_CONTEXT);
gen invztrans(const gen & f, const gen & x, const gen & s, GIAC_CONTEXT);
gen _invztrans(const gen & args, GIAC_CONTEXT);

gen integrate_without_lnabs(const gen & e, const gen & x, GIAC_CONTEXT);



}


#endif // _GIAC_DESOLVE_H
