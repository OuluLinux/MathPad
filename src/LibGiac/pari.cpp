#include "giacPCH.h"

#ifndef NSPIRE
using namespace std;
#endif

#include "config.h"
#include <stdexcept>
#include "gen.h"
#include "identificateur.h"
#include "sym2poly.h"
#include "plot.h"
#include "prog.h"
#include "usual.h"
#include "input_lexer.h"
#include "modpoly.h"
#include "giacintl.h"



namespace giac {


static gen pari_error() {
	return undeferr(gettext("Not implemented, please recompile giac with PARI"));
}

gen pari_isprime(const gen & e, int certif) {
	return string2gen("please recompile giac with PARI", false);
}
std::string pari_ifactor(const gen & e) {
	return "please recompile giac with PARI";
}

gen pari_ffinit(const gen & p, int n) {
	return string2gen("please recompile giac with PARI", false);
}

gen pari_gamma(const gen & e) {
	return pari_error();
}
gen pari_zeta(const gen & e) {
	return pari_error();
}
gen pari_psi(const gen & e) {
	return pari_error();
}

bool pari_lift_combine(const vecteur& a, const std::vector<vecteur>& factmod,
		gen& modulo, std::vector<vecteur>& res) {
	vecteur tmp(1, pari_error());
	res = std::vector<vecteur>(1, tmp);
	return false;
}

bool pari_galoisconj(const gen & g, vecteur & w, GIAC_CONTEXT) {
	return false;
}

gen _pari(const gen & args, GIAC_CONTEXT) {
	if (args.type == _STRNG && args.subtype == -1) return  args;
	return pari_error();
}

std::string pari_help(const gen & g) {
	return "please recompile giac with PARI";
}

gen GEN2gen(const GEN & G, const vecteur & vars) {
	return gensizeerr("please recompile giac with PARI");
}
GEN gen2GEN(const gen & e, const vecteur & vars, GIAC_CONTEXT) {
	return 0;
}

bool pari_polroots(const vecteur & p, vecteur & res, long prec, GIAC_CONTEXT) {
	return false;
}
bool pari_polresultant(const gen & p, const gen & q, const vecteur & lv, gen & res, GIAC_CONTEXT) {
	return false;
}
bool pari_nffactor(const gen & p, const gen & pmin, const vecteur & lv, gen & res, GIAC_CONTEXT) {
	return false;
}
static const char _pari_s [] = "pari";
static define_unary_function_eval(__pari, &giac::_pari, _pari_s);
define_unary_function_ptr5(at_pari , alias_at_pari, &__pari, _QUOTE_ARGUMENTS, true);

static const char _pari_unlock_s [] = "pari_unlock";
static define_unary_function_eval(__pari_unlock, &giac::_pari, _pari_unlock_s);
define_unary_function_ptr5(at_pari_unlock , alias_at_pari_unlock, &__pari_unlock, _QUOTE_ARGUMENTS, true);


}


