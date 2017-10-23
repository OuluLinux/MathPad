#ifndef _GIAC_IFACTOR_H_
#define _GIAC_IFACTOR_H_

#include "first.h"
#include "global.h"
#include "gen.h"
#include "unary.h"
#include "symbolic.h"
#include "identificateur.h"


namespace giac {

inline unsigned sizeinbase2(unsigned n) {
	unsigned i = 0;
	for (;n;++i) {
		n >>= 1;
	}
	return i;
}

extern const short int giac_primes[];
gen _ithprime(const gen & args, GIAC_CONTEXT);
gen _ifactors(const gen & args, GIAC_CONTEXT);
gen _maple_ifactors(const gen & args, GIAC_CONTEXT);

symbolic symb_ifactor(const gen & args);
vecteur ifactors(const gen & n0, GIAC_CONTEXT);
gen ifactors(const gen & args, int maplemode, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_ifactors;
extern const unary_function_ptr * const  at_maple_ifactors;
extern const unary_function_ptr * const  at_nprimes;

vecteur factors(const gen & g, const gen & x, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_factors;
gen _factors(const gen & args, GIAC_CONTEXT);

gen _divis(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_divis ;

gen idivis(const gen & n, GIAC_CONTEXT);
gen _idivis(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_idivis ;
int modulo(const mpz_t & a, unsigned b);

vecteur pfacprem(gen & n, bool addlast, GIAC_CONTEXT);

gen ifactor(const gen & n, GIAC_CONTEXT);
gen _ifactor(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_ifactor ;

gen euler(const gen & e, GIAC_CONTEXT);
gen _euler(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_euler;

gen pa2b2(const gen & p, GIAC_CONTEXT);
gen _pa2b2(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_pa2b2 ;

gen _propfrac(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_propfrac ;

gen iabcuv(const gen & a, const gen & b, const gen & c);
gen _iabcuv(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_iabcuv ;

gen abcuv(const gen & a, const gen & b, const gen & c, const gen & x, GIAC_CONTEXT);
gen _abcuv(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_abcuv ;

gen simp2(const gen & a, const gen & b, GIAC_CONTEXT);
gen _simp2(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_simp2 ;

gen fxnd(const gen & a);
gen _fxnd(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_fxnd ;

extern const unary_function_ptr * const  at_ithprime ;


}


#endif // _GIAC_IFACTOR_H
