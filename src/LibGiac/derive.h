#ifndef _GIAC_DERIVE_H
#define _GIAC_DERIVE_H

#include "first.h"
#include "gen.h"
#include "identificateur.h"


namespace giac {

struct unary_function_ptr;
gen eval_before_diff(const gen & expr, const gen & variable, GIAC_CONTEXT);
gen derive(const gen & e, const identificateur & i, GIAC_CONTEXT);
gen derive(const gen & e, const gen & vars, GIAC_CONTEXT);
gen derive(const gen & e, const gen & vars, const gen & nderiv, GIAC_CONTEXT);
gen _derive(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_derive ;
extern const unary_function_ptr * const  at_grad ;
extern const unary_function_ptr * const  at_function_diff ;
extern const unary_function_ptr * const  at_implicit_diff ;
symbolic symb_derive(const gen & a, const gen & b);
gen symb_derive(const gen & a, const gen & b, const gen &c);
gen _function_diff(const gen & g, GIAC_CONTEXT);
bool depend(const gen & g, const identificateur & i);
gen _grad(const gen & args, GIAC_CONTEXT);

}


#endif
