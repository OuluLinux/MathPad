#ifndef _GIAC_SYMBOLIC_H
#define _GIAC_SYMBOLIC_H

#include "first.h"
#include <iostream>
#include <string>
#include "vector.h"
#include "unary.h"
#include "gen.h"


namespace giac {


const unary_function_ptr * archive_function_tab();
extern int archive_function_tab_length;
int archive_function_index(const unary_function_ptr & f);
inline int archive_function_index(const unary_function_ptr * f) { return archive_function_index(*f); };

std::ostream & operator << (std::ostream & os, const symbolic & s);

int equalposcomp(const unary_function_ptr tab[], const unary_function_ptr & f);
inline int equalposcomp(const unary_function_ptr tab[], const unary_function_ptr * f){ return equalposcomp(tab, *f);} ;
int equalposcomp(const std::vector<const unary_function_ptr *> & v, const unary_function_ptr * f);
int equalposcomp(const const_unary_function_ptr_ptr tab[], const unary_function_ptr & f);
inline int equalposcomp(const const_unary_function_ptr_ptr tab[], const unary_function_ptr * f){ return equalposcomp(tab, *f);};

// find the "size" of g but limited by max
unsigned taille(const gen & g, unsigned max);
extern bool print_rewrite_prod_inv;
// try to rewrite arg the argument of a product as a fraction n/d
bool rewrite_prod_inv(const gen & arg, gen & n, gen & d);
std::string & add_print(std::string & s, const gen & g, GIAC_CONTEXT);

// non recursive eval function
gen nr_eval(const gen & g, int level, const context * ct);


}


#endif // _GIAC_SYMBOLIC_H
