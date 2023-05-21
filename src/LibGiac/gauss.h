#ifndef _GIAC_GAUSS_H
#define _GIAC_GAUSS_H

#include "first.h"
#include "gen.h"


namespace giac {


vecteur quad(int &b, const gen & q, const vecteur & x, GIAC_CONTEXT);
// find matrix of a quadratic form
// q=symbolic expression with respect to the vecteur of variables x
// b=2 if q is quadratique,=0,1 ou 3 si il y des termes d'ordre 0,1 ou 3
vecteur qxa(const gen &q, const vecteur & x, GIAC_CONTEXT);
gen _q2a(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_q2a;

vecteur gauss(const gen & q, const vecteur & x, vecteur & D, vecteur & U, vecteur & P, GIAC_CONTEXT);
vecteur gauss(const gen & q, const vecteur & x, GIAC_CONTEXT);
gen _gauss(const gen & args, GIAC_CONTEXT);
extern const unary_function_ptr * const  at_gauss;

extern const unary_function_ptr * const  at_a2q;
gen axq(const vecteur &A, const vecteur & x, GIAC_CONTEXT);
gen _a2q(const gen & args, GIAC_CONTEXT);
vecteur qxac(const gen &q, const vecteur & x, GIAC_CONTEXT);

// rational parametrization of a conic, given cartesian equation and point over
gen conique_ratparam(const gen & eq, const gen & M, GIAC_CONTEXT);

// given a 2-var 2nd order equation `equation_conique'
// and the vecteur of variables 'nom_des_variables'
// conique_reduite returns
// the coordinates of the center of the conic in x0 and y0
// the eigenvectors in V1 and V2
// a parameter `propre'
// and the reduced equation in equation_reduite
// param_curves is a vecteur containing vecteurs of size 2 (lines)
// or 5 (parametric curves with equation,identificateur,tmin,tmax,tstep)
bool conique_reduite(const gen & equation_conique, const gen & pointsurconique, const vecteur & nom_des_variables, gen & x0, gen & y0, vecteur & V1, vecteur &V2, gen & propre, gen & equation_reduite, vecteur & param_curves, gen & ratparam, bool numeric, GIAC_CONTEXT);
bool quadrique_reduite(const gen & q, const gen & pointsurquadrique, const vecteur & vxyz, gen & x, gen & y, gen & z, vecteur & u, vecteur & v, vecteur & w, vecteur & propre, gen & equation_reduite, vecteur & param_surface, vecteur & centre, bool numeric, GIAC_CONTEXT);

extern const unary_function_ptr * const  at_conique_reduite;
gen _conique_reduite(const gen & args, GIAC_CONTEXT);
gen _quadrique_reduite(const gen & args, GIAC_CONTEXT);


}


#endif // _GIAC_GAUSS_H
