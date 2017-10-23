#ifndef _GIAC_COCOA_H_
#define _GIAC_COCOA_H_

#include "first.h"
#include "gausspol.h"
#include "gen.h"

// special code for polynomial up to 11 variables (max deg<32768)
//#define GROEBNER_VARS 11
#define GROEBNER_VARS 15
#define GIAC_REDUCEMODULO // reduce initial input only at modular step


namespace giac {


bool f5(vectpoly &, const gen & order);
bool cocoa_gbasis(vectpoly &, const gen & order);
vecteur cocoa_in_ideal(const vectpoly & g, const vectpoly & v, const gen & ordre);
bool cocoa_greduce(const vectpoly & r, const vectpoly & v, const gen & order, vectpoly & res);

#ifndef CAS38_DISABLED
// giac code for poly (fast up to 15 variables)

struct order_t {
	short o;
	unsigned char dim;
	unsigned char lex;
};

bool gbasis8(const vectpoly & v, order_t & order, vectpoly & res, environment * env, bool modularalgo, bool modularcheck, bool & rur, GIAC_CONTEXT);
bool greduce8(const vectpoly & v, const vectpoly & G, order_t & order, vectpoly & res, environment * env, GIAC_CONTEXT);
#endif


}


#endif
