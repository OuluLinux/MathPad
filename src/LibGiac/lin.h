#ifndef _GIAC_LIN_H
#define _GIAC_LIN_H

#include "first.h"
#include "vector.h"


namespace giac {

class gen;
bool contains(const gen & e, const unary_function_ptr & mys);
inline bool contains(const gen & e, const unary_function_ptr * mys){ return contains(e, *mys); }
void compress(vecteur & res, GIAC_CONTEXT);

gen prod_expand(const gen & a, const gen & b, GIAC_CONTEXT);

gen unlin(vecteur & v, GIAC_CONTEXT);
void convolution(const gen & coeff, const gen & arg, const vecteur & w, vecteur & res, GIAC_CONTEXT);
void convolution(const vecteur & v, const vecteur & w, vecteur & res, GIAC_CONTEXT);
void convolutionpower(const vecteur & v, int k, vecteur & res, GIAC_CONTEXT);
void lin(const gen & e, vecteur & v, GIAC_CONTEXT);

symbolic symb_lin(const gen & e);
gen _lin(const gen & e, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_lin ;
gen lnexpand(const gen & e, GIAC_CONTEXT);

vecteur tchebycheff(int n, bool first_kind = true);
gen tunlin(vecteur & v, GIAC_CONTEXT);
void tconvolution(const gen & coeff, const gen & arg, const vecteur & w, vecteur & res, GIAC_CONTEXT);
void tconvolution(const vecteur & v, const vecteur & w, vecteur & res, GIAC_CONTEXT);
void tconvolutionpower(const vecteur & v, int k, vecteur & res, GIAC_CONTEXT);
void tlin(const gen & e, vecteur & v, GIAC_CONTEXT);

symbolic symb_tlin(const gen & e);
gen _tlin(const gen & e, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_tlin ;

symbolic symb_texpand(const gen & e);
gen _texpand(const gen & e, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_texpand ;

symbolic symb_expand(const gen & e);
gen expand(const gen & e, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_expand ;
vecteur tchebycheff(int n, bool first_kind);
gen ln_expand(const gen & e0, GIAC_CONTEXT);
gen symhorner(const vecteur & v, const gen & e);
gen _texpand(const gen & args, GIAC_CONTEXT);
gen expexpand(const gen & e, GIAC_CONTEXT);
gen lnexpand(const gen & e, GIAC_CONTEXT);
gen trigexpand(const gen & e, GIAC_CONTEXT);



}



#endif // _GIAC_LIN_H
