#ifndef _GIAC_ALG_EXT_H
#define _GIAC_ALG_EXT_H

#include "first.h"
#include <string>
#include "global.h"


namespace giac {

  class gen;
  struct unary_function_ptr;
  struct symbolic;

  bool proot_cached(const vecteur & v,double eps,vecteur & res);
  bool proot_cache(const vecteur & v,double eps,const vecteur & res);

  bool galoisconj_cached(const vecteur & v,vecteur & res);
  bool galoisconj_cache(const vecteur & v,const vecteur & res);
  vecteur galoisconj(const vecteur & v,GIAC_CONTEXT);
  bool conj_in_nf(const vecteur & w,gen & g,GIAC_CONTEXT);

  bool islesscomplex(const gen & a,const gen & b);
  bool is_sqrt(const gen & a,gen & arg);
  gen select_root(const vecteur & v,GIAC_CONTEXT);
  gen in_select_root(const vecteur & a,bool reel,GIAC_CONTEXT);
  bool is_known_rootof(const vecteur & v,gen & symroot,GIAC_CONTEXT);
  gen horner_rootof(const vecteur & p,const gen & g,GIAC_CONTEXT);
  bool has_rootof_value(const gen & Pmin,gen & value,GIAC_CONTEXT);

  gen alg_evalf(const gen & a,const gen &b,GIAC_CONTEXT);
  gen approx_rootof(const gen & e,GIAC_CONTEXT);
  gen common_EXT(gen & a,gen & b,const vecteur * l,GIAC_CONTEXT);
  gen common_minimal_POLY(const gen & ga,const gen & gb, gen & a,gen & b,int &k,GIAC_CONTEXT);
  gen algebraic_EXTension(const gen & a,const gen & v);
  gen ext_reduce(const gen & a, const gen & v);
  gen ext_reduce(const gen & e);
  gen ext_add(const gen & a,const gen & b,GIAC_CONTEXT);
  gen ext_sub(const gen & a,const gen & b,GIAC_CONTEXT);
  gen ext_mul(const gen & a,const gen & b,GIAC_CONTEXT);
  gen inv_EXT(const gen & a);
  gen symb_rootof(const gen & p,const gen &pmin,GIAC_CONTEXT);
  gen rootof(const gen & e,GIAC_CONTEXT);
  extern const unary_function_ptr * const  at_rootof ;
  vecteur min_pol(gen & a);
  
  // Return the signed subresultant Sturm sequence for a rational
  // fraction g with respect to x
  // A squarefree factorization is performed first
  // Factors of even mult are discarded
  // Factors of odd multiplicities generate one vecteur of dense
  // polynomials (also coded as vecteur)
  // The content of the numerator and denominator are returned as well
  vecteur sturm(const gen &g,const gen & x,GIAC_CONTEXT);
  extern const unary_function_ptr * const  at_sturm ;
  // Number of sign changes of g when x is inside the ]a,b[ interval
  // Zeros of g of even multiplicities are not counted
  // Zeros of g of odd multiplicites are counted once
  // g must be a rational fraction with respect to x
  // a should be < b
  // If sturmab returns 0, then the sign is constant positive
  // If sturmab returns -1, the sign is constant negative
  int sturmab(const gen & g,const gen &x,const gen & a,const gen & b,GIAC_CONTEXT);
  gen _sturmab(const gen & g_orig,GIAC_CONTEXT);
  gen _sturm(const gen & g,GIAC_CONTEXT);
  gen _sturmseq(const gen & g,GIAC_CONTEXT);

  extern const unary_function_ptr * const  at_sturmab ;
  int sturmsign(const gen & a,bool strict,GIAC_CONTEXT);
  // find extremals values of g, return type of g (0 nothing assumed, 1 real, 2 integer)
  int find_range(const gen & g,vecteur & a,GIAC_CONTEXT);
  // minmax=-1 min 0 both 1 max
  gen fminmax(const gen & g,int minmax,GIAC_CONTEXT);
  bool find_good_eval(const polynome & F,polynome & Fb,vecteur & b);


}


#endif // _GIAC_ALG_EXT_H
