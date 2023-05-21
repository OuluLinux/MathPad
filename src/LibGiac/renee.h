#ifndef __RENEE_H
#define __RENEE_H

#include "gen.h"
#include "unary.h"


namespace giac {


vector<int> vecteur_2_vector_int(const vecteur & v);
gen _exemple(const gen & args);
extern unary_function_ptr at_exemple ;

gen _ppcm(const gen & args);
extern unary_function_ptr at_ppcm ;


}



#endif
