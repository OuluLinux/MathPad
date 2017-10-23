#ifndef _GIAC_QUATER_H
#define _GIAC_QUATER_H

#include "first.h"
#include "gen.h"
#include <string>



namespace giac {


extern const unary_function_ptr * const  at_quaternion; // user-level quaternion constructor

class quaternion : public gen_user {
public:
	gen r, i, j, k;
	virtual gen_user * memory_alloc() const {
		quaternion * ptr = new quaternion(*this);
		return dynamic_cast<gen_user *>(ptr);
	}
	quaternion(const quaternion & q): r(q.r), i(q.i), j(q.j), k(q.k) {};
	quaternion(const gen & myr, const gen & myi, const gen & myj, const gen & myk): r(myr), i(myi), j(myj), k(myk) {};
	quaternion(): r(zero), i(zero), j(zero), k(zero) {};
	quaternion(const gen & g);
	virtual gen operator + (const gen & g) const {
		quaternion q(g);
		return quaternion(r + q.r, i + q.i, j + q.j, k + q.k);
	}
	virtual gen operator - (const gen & g) const {
		quaternion q(g);
		return quaternion(r -q.r, i - q.i, j - q.j, k - q.k);
	}
	virtual std::string print(GIAC_CONTEXT) const ;
};
gen _quaternion(const gen & args, GIAC_CONTEXT);

class galois_field : public gen_user {
public:
	gen p; // F_p^m, characteristic of the field
	gen P; // minimal irreducible polynomial of degree m, as vector
	gen x; // the name of the variable for construction
	gen a; // value as a vector polynomial or undef (whole field)
	virtual gen_user * memory_alloc() const {
		galois_field * ptr = new galois_field(*this);
		return ptr;
	}
	galois_field(const galois_field & q);
	galois_field(const gen p_, const gen & P_, const gen & x_, const gen & a_);
	galois_field(const gen & g, bool primitive, GIAC_CONTEXT);
	void reduce(); // reduce a
	virtual gen operator + (const gen & g) const;
	virtual gen operator - (const gen & g) const;
	virtual gen operator - () const;
	virtual gen operator * (const gen & g) const;
	virtual gen operator / (const gen & g) const;
	virtual gen inv() const ;
	virtual std::string print(GIAC_CONTEXT) const ;
	virtual std::string texprint(GIAC_CONTEXT) const ;
	virtual bool operator == (const gen &) const ;
	virtual bool is_zero() const;
	virtual bool is_one() const;
	virtual bool is_minus_one() const;
	virtual gen operator()(const gen &, GIAC_CONTEXT) const;
	virtual gen operator [](const gen &) ;
	virtual gen operator > (const gen & g) const;
	virtual gen operator < (const gen & g) const;
	virtual gen operator >= (const gen & g) const;
	virtual gen operator <= (const gen & g) const;
	virtual gen gcd(const gen &) const { return plus_one;}
	virtual gen gcd(const gen_user & a) const { return plus_one; }
	virtual gen polygcd(const polynome &, const polynome &, polynome &) const ;
	virtual gen makegen(int i) const ;
	virtual gen polyfactor(const polynome & p, factorization & f) const ;
	virtual gen conj(GIAC_CONTEXT) const { return *this;}
	virtual gen re(GIAC_CONTEXT) const { return *this;}
	virtual gen im(GIAC_CONTEXT) const {  return 0;}
	virtual gen sqrt(GIAC_CONTEXT) const;
	virtual gen rand(GIAC_CONTEXT) const;
	polynome poly_reduce(const polynome & p) const ;
};

// Is the polynomial v irreducible and primitive modulo p?
// If it is only irreducible, returns 2 and sets vmin
int is_irreducible_primitive(const vecteur & v, const gen & p, vecteur & vmin, bool primitive, GIAC_CONTEXT);
vecteur find_irreducible_primitive(int p, int m, bool primitive, GIAC_CONTEXT);
gen _galois_field(const gen & args, GIAC_CONTEXT);




}



#endif // _GIAC_QUATER_H
