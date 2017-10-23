#ifndef _GIAC_EZGCD_H_
#define _GIAC_EZGCD_H_

#include "first.h"
#include "gausspol.h"


namespace giac {


void change_dim(polynome & p, int dim);

// Hensel quadratic lift
// Lift the equality p(b)=qb*rb [where b is a vecteur like for peval
// assumed to have p.dim-1 coordinates] to p=q*r mod (X-b)^deg
// Assuming that lcoeff(q)=lcp, lcoeff(r)=lcp, lcoeff(p)=lcp^2
// If you want to find factors of a poly P such that P(b)=Qb*Rb,
// if lcp is the leading coeff of P
// then p=P*lcp, qb=Qb*lcp(b)/lcoeff(Qb), rb=Rb*lcp(b)/lcoeff(Rb)
bool hensel_lift(const polynome & p, const polynome & lcp, const polynome & qb, const polynome & rb, const vecteur & b, polynome & q, polynome & r, bool linear_lift = true, double maxop = -1);

bool find_good_eval(const polynome & F, const polynome & G, polynome & Fb, polynome & Gb, vecteur & b, bool debuglog = false, const gen & mod = zero);
polynome peval_1(const polynome & p, const vecteur &v, const gen & mod);
// Replace the last coordinates of p with b instead of the first
gen peval_back(const polynome & p, const vecteur & b);
polynome unmodularize(const std::vector<int> & a);

// reduce_divrem does a mixed division: euclidean w.r.t. the first var
// and ascending power of X-v for the other vars
// FIXME: this implementation does not work currently, except if other
// depends only on the first var
bool reduce_divrem(const polynome & a, const polynome & other, const vecteur & v, int n, polynome & quo, polynome & rem) ;

// find the "remainder of p mod (X-v)^degree"
// dim(p) = size(v)+1 (reduction for all variables of p except the main var)
polynome reduce_poly(const polynome & p, const vecteur & v, int degree);

bool try_sparse_factor(const polynome & pcur, const factorization & v0, int mult, factorization & f);

bool try_hensel_lift_factor(const polynome & pcur, const polynome & F0, const factorization & v0, int mult, factorization & f);

// max_gcddeg is used when ezgcd was not successfull to find
// the gcd even with 2 evaluations leading to the same gcd degree
// in this case ezgcd calls itself with a bound on the gcd degree
// is_sqff is true if we know that F_orig or G_orig is squarefree
// is_primitive is true if F_orig and G_orig is primitive
bool ezgcd(const polynome & F_orig, const polynome & G_orig, polynome & GCD, bool is_sqff = false, bool is_primitive = false, int max_gcddeg = 0, double maxop = -1);

gen _ezgcd(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_ezgcd;

gen _modgcd(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_modgcd;

gen _heugcd(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_heugcd;

gen _psrgcd(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_psrgcd;


}


#endif // _GIAC_EZGCD_H
