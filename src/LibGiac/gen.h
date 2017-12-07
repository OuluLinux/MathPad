#ifndef _GIAC_GEN_H
#define _GIAC_GEN_H

#include "config.h"
#include "first.h"
#include <iostream>
#include <string>
#include "vector.h"
#include <map>
#include "dispatch.h"
#include "vecteur.h"
#include "fraction.h"
#include "poly.h"
#include "giacintl.h"
#include <complex>
#include <stdlib.h>

#ifdef STATIC_BUILTIN_LEXER_FUNCTIONS
#include "static.h"
#endif


namespace giac {




void my_mpz_gcd(mpz_t &z, const mpz_t & A, const mpz_t & B);

class gen ;
// errors
void settypeerr(GIAC_CONTEXT0);
void setsizeerr(GIAC_CONTEXT0);
void setdimerr(GIAC_CONTEXT0);
void settypeerr(const std::string & s);
void setsizeerr(const std::string & s);
void setdimerr(const std::string & s);
void divisionby0err(const gen &, GIAC_CONTEXT0);
void cksignerr(const gen &, GIAC_CONTEXT0);
void invalidserieserr(const std::string &, GIAC_CONTEXT0);
void toofewargs(const std::string & s, GIAC_CONTEXT0);
void toomanyargs(const std::string & s, GIAC_CONTEXT0);
void maxordererr(GIAC_CONTEXT0);
void setstabilityerr(GIAC_CONTEXT0);

gen undeferr(const std::string & s);
gen gentypeerr(GIAC_CONTEXT0);
void gentypeerr(gen & g, GIAC_CONTEXT);
gen gensizeerr(GIAC_CONTEXT0);
void gensizeerr(gen & g, GIAC_CONTEXT);
gen gendimerr(GIAC_CONTEXT0);
void gendimerr(gen & g, GIAC_CONTEXT);
gen gentypeerr(const std::string & s);
void gentypeerr(const char * ch, gen & g);
gen gensizeerr(const std::string & s);
void gensizeerr(const char * ch, gen & g);
gen gendimerr(const std::string & s);
void gensizeerr(const char * ch, gen & g);
gen gendivisionby0err(const gen &, GIAC_CONTEXT0);
gen gencksignerr(const gen &, GIAC_CONTEXT0);
gen geninvalidserieserr(const std::string &, GIAC_CONTEXT0);
gen gentoofewargs(const std::string & s, GIAC_CONTEXT0);
gen gentoomanyargs(const std::string & s, GIAC_CONTEXT0);
gen genmaxordererr(GIAC_CONTEXT0);
gen genstabilityerr(GIAC_CONTEXT0);

// short integer arithmetic
int absint(int a);
int giacmin(int a, int b);
int giacmax(int a, int b);
int invmod(int n, int modulo);
unsigned invmod(unsigned a, int b);
int invmod(longlong a, int b);
#ifdef INT128
int invmod(int128_t a, int b);
inline int smod(int128_t r, int m) {
	int R = r % m;
	return smod(R, m);
}
#endif
int gcd(int a, int b);
int smod(int a, int b); // where b is assumed to be positive
inline int smod(longlong r, int m) {
	int R = r % m;
	return smod(R, m);
}
int simplify(int & a, int & b);

struct ref_mpz_t {
	volatile ref_count_t ref_count;
	mpz_t z;
	ref_mpz_t(): ref_count(1) {mpz_init(z);}
	ref_mpz_t(size_t nbits): ref_count(1) {mpz_init2(z, int(nbits));}
	ref_mpz_t(const mpz_t & Z): ref_count(1) { mpz_init_set(z, Z); }
	~ref_mpz_t() { mpz_clear(z); }
};
class identificateur;
struct ref_identificateur; // in identificateur.h
struct symbolic;
struct ref_symbolic; // in symbolic.h
class unary_function_eval;
struct unary_function_ptr;
typedef const unary_function_ptr *  const_unary_function_ptr_ptr;
typedef const unary_function_eval *  const_unary_function_eval_ptr;
struct ref_unary_function_ptr; // in unary.h
struct eqwdata;
struct ref_eqwdata ; // defined below after gen
struct ref_complex;
struct ref_algext;
struct ref_modulo;
// Graphic object
struct grob {
	void (* grob_draw)(void);
	int (* grob_handle)(int);
	void * grob_data;
};
struct ref_grob {
	volatile ref_count_t ref_count;
	grob g;
	ref_grob(const grob & G): ref_count(1), g(G) {}
};
class gen_user;
struct ref_gen_user ; // user defined type
struct ref_string {
	volatile ref_count_t ref_count;
	std::string s;
	ref_string(const std::string & S): ref_count(1), s(S) {}
};
template <class T> class tensor;

typedef tensor<gen> polynome;
typedef std::vector< facteur< polynome > > factorization;

template<class T> class Tref_tensor; // in poly.h
typedef Tref_tensor<gen> ref_polynome;
typedef Tfraction<gen> fraction;
template<class T> class Tref_fraction;
typedef Tref_fraction<gen> ref_fraction;

struct ref_vecteur;
void delete_ref_vecteur(ref_vecteur * ptr);
ref_vecteur * new_ref_vecteur(const vecteur & v);
ref_symbolic * new_ref_symbolic(const symbolic & s);

template<class T> class Tref_fraction; // in fraction.h
struct ref_void_pointer {
	volatile ref_count_t ref_count;
	void * p;
	ref_void_pointer(void * P): ref_count(1), p(P) {}
};


struct monome;
// sparse polynomials: uncomment one of the 2 next lines
#ifdef DEBUG_SUPPORT
typedef dbgprint_vector<monome> sparse_poly1; // debugging support
#else
typedef std::vector<monome> sparse_poly1; // no debug. support
#endif
struct ref_sparse_poly1;

// arbitrary precision floats hierarchy (value or interval)
std::string printmpf_t(const mpf_t & inf);
class real_object {
public:
	mpf_t inf;
	real_object(double d);
	real_object(const mpf_t & d);
	real_object(const gen & g);
	real_object(const gen & g, unsigned int precision);
	real_object() ;
	virtual std::string print(GIAC_CONTEXT) const;
	void dbgprint() const { CERR << this->print(0) << std::endl; }
	virtual ~real_object() {
		mpf_clear(inf);
	}
	virtual real_object & operator = (const real_object & g);
	real_object(const real_object & g) ;
	virtual gen addition(const gen & g, GIAC_CONTEXT) const;
	gen operator + (const gen & g) const;
	virtual gen operator + (const real_object & g) const;
	virtual gen multiply(const gen & g, GIAC_CONTEXT) const;
	gen operator * (const gen & g) const;
	virtual gen operator * (const real_object & g) const;
	virtual gen divide(const gen & g, GIAC_CONTEXT) const;
	gen operator / (const gen & g) const;
	virtual gen substract(const gen & g, GIAC_CONTEXT) const;
	virtual gen operator / (const real_object & g) const;
	gen operator - (const gen & g) const;
	virtual gen operator - (const real_object & g) const;
	virtual gen operator -() const;
	virtual gen inv() const;
	virtual gen sqrt() const;
	virtual gen abs() const;
	virtual gen exp() const;
	virtual gen log() const;
	virtual gen sin() const;
	virtual gen cos() const;
	virtual gen tan() const;
	virtual gen sinh() const;
	virtual gen cosh() const;
	virtual gen tanh() const;
	virtual gen asin() const;
	virtual gen acos() const;
	virtual gen atan() const;
	virtual gen asinh() const;
	virtual gen acosh() const;
	virtual gen atanh() const;
	virtual bool is_zero() const;
	virtual bool maybe_zero() const;
	virtual bool is_inf() const;
	virtual bool is_nan() const;
	virtual int is_positive() const;
	virtual double evalf_double() const;
};
struct ref_real_object {
	volatile ref_count_t ref_count;
	real_object r;
	ref_real_object(): ref_count(1) {}
	ref_real_object(const real_object & R): ref_count(1), r(R) {}
};
gen real2int(const gen & g, GIAC_CONTEXT);
gen real2double(const gen & g);
class real_interval : public real_object {
public:

	mpf_t sup;
	real_interval() {
		mpf_init_set(sup, inf);
	}
	real_interval(const real_object & r): real_object(r) {
		mpf_init_set(sup, r.inf);
	}
	real_interval(const real_interval & r): real_object(r) {
		mpf_init_set(sup, r.sup);
	}
	virtual ~real_interval() {
		mpf_clear(sup);
	}
	virtual real_object & operator = (const real_interval & g) ;
	virtual real_object & operator = (const real_object & g) ;
	virtual gen addition(const gen & g, GIAC_CONTEXT) const;
	virtual gen operator + (const real_object & g) const;
	virtual real_interval operator + (const real_interval & g) const;
	virtual gen multiply(const gen & g, GIAC_CONTEXT) const;
	virtual gen operator * (const real_object & g) const;
	virtual real_interval operator * (const real_interval & g) const;
	virtual gen divide(const gen & g, GIAC_CONTEXT) const;
	virtual gen substract(const gen & g, GIAC_CONTEXT) const;
	virtual gen operator - (const real_object & g) const;
	virtual real_interval operator - (const real_interval & g) const ;
	virtual gen operator -() const;
	virtual bool is_zero() const ;
	virtual bool maybe_zero() const ;
	virtual int is_positive() const ;
	virtual bool is_inf() const;
	virtual bool is_nan() const;
	virtual gen inv() const;
	virtual gen sqrt() const;
	virtual gen abs() const;
	virtual gen exp() const;
	virtual gen log() const;
	virtual gen sin() const;
	virtual gen cos() const;
	virtual gen tan() const;
	virtual gen sinh() const;
	virtual gen cosh() const;
	virtual gen tanh() const;
	virtual gen asin() const;
	virtual gen acos() const;
	virtual gen atan() const;
	virtual gen asinh() const;
	virtual gen acosh() const;
	virtual gen atanh() const;
};
struct ref_real_interval {
	volatile ref_count_t ref_count;
	real_interval r; // assumes that storage of real_object inside real_interval is first
	ref_real_interval(): ref_count(1) {}
	ref_real_interval(const real_interval & R): ref_count(1), r(R) {}
};
std::string print_binary(const real_object & r);
gen read_binary(const std::string & s, unsigned int precision);
// Convert g to a real or complex object of precision nbits
gen accurate_evalf(const gen & g, int nbits);
vecteur accurate_evalf(const vecteur & v, int nbits);
std::string print_DOUBLE_(double d, GIAC_CONTEXT);

#if 1 // def NSPIRE
class comparegen {
public:
	bool operator()(const gen & a, const gen & b) const;
};
typedef std::map<gen, gen, comparegen> gen_map;
#else
typedef std::map<gen, gen, const std::pointer_to_binary_function < const gen &, const gen &, bool> > gen_map;
#endif
struct ref_gen_map;

class my_mpz;

#ifdef NO_UNARY_FUNCTION_COMPOSE
class unary_function_eval;
#else
class unary_function_abstract;
#endif

struct alias_unary_function_eval;
struct unary_function_ptr {
#ifdef NO_UNARY_FUNCTION_COMPOSE
	// const unary_function_eval * _ptr;
	size_t _ptr = 0;
	// int quoted; // will be used to avoid evaluation of args by eval
	// constructors
	// lexer_register is true to add dynamically the function name
	// to the list of functions names recognized by the lexer
	unary_function_ptr(): _ptr(0) {} ;
	unary_function_ptr(const unary_function_eval * myptr): _ptr((size_t)myptr) {} ;
	// unary_function_ptr(const unary_function_eval * myptr,int parser_token);
	unary_function_ptr(const unary_function_eval * myptr, int myquoted, int parser_token);
	// unary_function_ptr(const alias_unary_function_eval * myptr,int parser_token);
	unary_function_ptr(const alias_unary_function_eval * myptr, int myquoted, int parser_token);
#else
	const unary_function_abstract * _ptr = NULL;
	// long * ref_count;
	// int quoted; // will be used to avoid evaluation of args by eval
	// constructors
	// lexer_register is true to add dynamically the function name
	// to the list of functions names recognized by the lexer
	// unary_function_ptr(const unary_function_abstract & myptr);
	unary_function_ptr(): _ptr(0) {chk();} ;
	unary_function_ptr(const unary_function_abstract * myptr): _ptr(myptr) {chk();} ;
	// unary_function_ptr(const unary_function_abstract * myptr,int parser_token) ;
	// unary_function_ptr(const unary_function_abstract & myptr,int myquoted,int parser_token=0);
	unary_function_ptr(const unary_function_abstract * myptr, int myquoted, int parser_token);
	// unary_function_ptr(const unary_function_ptr & myptr);
	// unary_function_ptr(const alias_unary_function_eval * myptr,int parser_token);
	unary_function_ptr(const alias_unary_function_eval * myptr, int myquoted, int parser_token);
#endif
	// ~unary_function_ptr();
	// unary_function_ptr & operator = (const unary_function_ptr & acopier);
	gen operator()(const gen & arg, GIAC_CONTEXT) const;
#ifdef NO_UNARY_FUNCTION_COMPOSE
	inline unary_function_eval * ptr() const {
#ifdef __x86_64__
		return (unary_function_eval *)(((ulonglong) _ptr & ~((ulonglong)1)));
#else
		return (unary_function_eval *)(((size_t) _ptr & ~((size_t)1)));
#endif
	}
#else
	inline unary_function_abstract * ptr() const {
#ifdef __x86_64__
		return (unary_function_abstract *)(((ulonglong) _ptr & ~((ulonglong)1)));
#else
	return (unary_function_abstract *)(((size_t) _ptr & ~((size_t)1)));
#endif
	}
#endif
	bool quoted() const ;
	inline bool operator ==(const unary_function_ptr & u) const {
		// if (&u==this) return true;
#ifdef __x86_64__
		return ((ulonglong)(_ptr) & ~((ulonglong)1))  == ((ulonglong)(u._ptr) & ~((ulonglong)1));
#else
		return ((size_t)(_ptr) & ~((size_t)1)) == ((size_t)(u._ptr) & ~((size_t)1));
#endif
	}
	inline bool operator !=(const unary_function_ptr & u) const { return !(*this == u); }
	inline bool operator ==(const unary_function_ptr * u) const {
		// if (&u==this) return true;
#ifdef __x86_64__
		return u && (((ulonglong)(_ptr) & ~((ulonglong)1)) == ((ulonglong)(u->_ptr) & ~((ulonglong)1)));
#else
		return u && (((size_t)(_ptr) & ~((size_t)1)) == ((size_t)(u->_ptr) & ~((size_t)1)));
#endif
	}
	inline bool operator !=(const unary_function_ptr * u) const { return !(*this == u); }
	void dbgprint() const;
	void chk() const;
};

// FIXME: for little-endian check if type/unused/subtype order is correct!
class gen {
	
public:
#ifdef GIAC_TYPE_ON_8BITS
	unsigned char type = 0;  // see dispatch.h
#else
	unsigned char type = 0; // 32 types is enough, keep 3 bits more for double
	unsigned char type_unused = 0;
#endif
	signed char subtype = 0;
	unsigned short reserved = 0; // used if SMARTPTR is defined on 64 bit CPU (16 bits for pointer val)
	union {
		// immediate types
		int64_t val64 = 0; // immediate int (type _INT_)
		int val; // immediate int (type _INT_)
#ifdef DOUBLEVAL
		double _DOUBLE_val; // immediate float (type _DOUBLE_)
		giac_float _FLOAT_val;
#endif
		
		// pointer types
		ref_mpz_t * __ZINTptr; // long int (type _ZINT)
		ref_real_object * __REALptr; // extended double (type _REAL)
		ref_complex * __CPLXptr ; // complex as an gen[2] array (type _CPLX)
		ref_identificateur * __IDNTptr; // global name identifier (type _IDNT)
		ref_symbolic * __SYMBptr; // for symbolic objects (type _SYMB)
		ref_modulo * __MODptr;
		ref_algext * __EXTptr; // 2 gens for alg. extension (type ext)
		// alg ext: 1st gen is a std::vector or a fraction, 2nd gen is
		// a/ a std::vector, the minimal monic polynomial (the roots are permutable)
		// b/ a real_complex_rootof given by it's min poly and
		// c/ another type meaning that the root is expressed in terms
		//    of another rootof, in this case ext_reduce should be called
		// For 2nd order extension, X^2=d is used if d!=1 mod 4
		// X is the positive solution
		// if d=1 mod 4 the equation is X^2-X=(d-1)/4
		Tref_fraction<gen> * __FRACptr; // fraction (type _FRAC)
		Tref_tensor<gen> * __POLYptr ; // multidim. sparse polynomials (type poly)
		// _VECTosite types (std::vector<>)
		ref_vecteur * __VECTptr ; // vecteur: std::vectors & dense_POLY1 (type _VECT)
		ref_sparse_poly1 * __SPOL1ptr ; // std::vector<monome>: sparse 1-d poly (type _SPOL1)
		ref_string * __STRNGptr;
		unsigned _FUNC_;
		// ref_unary_function_ptr * __FUNCptr;
		ref_gen_user * __USERptr;
		ref_gen_map * __MAPptr;
		ref_eqwdata * __EQWptr;
		ref_grob * __GROBptr;
		ref_void_pointer * __POINTERptr;
		
	};
	inline volatile ref_count_t & ref_count() const {
		return __ZINTptr->ref_count;
	}
	gen(): type(_INT_), subtype(0), __POINTERptr(0) {
	};
	
	gen(void *ptr, short int subt): type(_POINTER_), subtype(char(subt)) {
		__POINTERptr = new ref_void_pointer(ptr);
	};
	
	gen(int i): type(_INT_), subtype(0), val(i) {
	};
	gen(size_t i): type(_INT_), subtype(0), val((int)i)  {
	};
	gen(long i);
	gen(longlong i);
#ifdef INT128
	gen(int128_t i);
#endif
	gen(const mpz_t & m);
	// WARNING coerce *mptr to an int if possible, in this case delete mptr
	// Pls do not use this constructor unless you know exactly what you do!!
	gen(ref_mpz_t * mptr);
#ifdef DOUBLEVAL
	gen(double d): type(_DOUBLE_), _DOUBLE_val(d) {};
#else
	// may not work on ia64 with -O2
	gen(double d);
#endif
	gen(const giac_float & f);
#ifdef BCD
	gen(accurate_bcd_float * b);
#endif
	// inline
	double DOUBLE_val() const ;
	giac_float FLOAT_val() const ;
	gen(int a, int b);
	gen(double a, double b);
	gen(const gen & a, const gen & b);
	gen(const std::complex<double> & c);
	gen(const gen & e);
	gen(const identificateur & s);
	gen(ref_identificateur * sptr);
	gen(const vecteur & v, short int s = 0);
	gen(ref_vecteur * vptr, short int s = 0);
	// vptr must be a pointer allocated by new, do not delete it explicitly
	gen(const symbolic & s);
	gen(ref_symbolic * sptr);
	gen(const gen_user & g);
	gen(const real_object & g);
	gen(const real_interval & g);
	// Pls do not use this constructor unless you know exactly what you do
	gen(Tref_tensor<gen> * pptr);
	gen(const polynome & p);
	gen(const fraction & p);
	gen(const std::string & s, GIAC_CONTEXT);
	gen(const wchar_t * s, GIAC_CONTEXT);
	gen(const char * s, GIAC_CONTEXT){ type = 0; *this = gen(std::string(s), contextptr); };
	gen(const sparse_poly1 & p);
	gen(const unary_function_ptr & f, int nargs = 1);
	gen(const unary_function_ptr * f, int nargs = 1);
	gen(const gen_map & m);
	gen(const eqwdata &);
	gen(const grob &);
	gen(const my_mpz &);
	~gen();
	
	bool in_eval(int level, gen & evaled, const context * contextptr) const;
	gen eval(int level, const context * contextptr) const;
	// inline gen eval() const { return eval(DEFAULT_EVAL_LEVEL,context0); }
	bool in_evalf(int level, gen & evaled, const context * contextptr) const;
	gen evalf(int level, const context * contextptr) const;
	// inline gen evalf() const { return evalf(DEFAULT_EVAL_LEVEL,context0); }
	gen evalf_double(int level, const context * contextptr) const ;
	gen evalf2double(int level, const context * contextptr) const;
	gen & operator = (const gen & a);
	int to_int() const ;
	bool is_real(GIAC_CONTEXT) const ;
	bool is_cinteger() const ;
	bool is_integer() const ;
	bool is_constant() const;
	std::string print(GIAC_CONTEXT) const;
	inline const char * printcharptr(GIAC_CONTEXT) const { return print(contextptr).c_str(); };
	// if sptr==0, return length required, otherwise print at end of *sptr
	int sprint(std::string * sptr, GIAC_CONTEXT) const;
	std::string print_universal(GIAC_CONTEXT) const;
	std::string print() const;
	inline const char * printcharptr() const { return print().c_str(); };
	wchar_t * wprint(GIAC_CONTEXT) const ;
	// print then convert to a malloc-ated wchar_t *
	void modify(int i) { *this = gen(i); };
	void dbgprint() const;
	void uncoerce() ;
	gen conj(GIAC_CONTEXT) const;
	gen re(GIAC_CONTEXT) const ;
	gen im(GIAC_CONTEXT) const ;
	gen inverse(GIAC_CONTEXT) const;
	gen squarenorm(GIAC_CONTEXT) const;
	int bindigits() const ;
	gen operator [](int i) const ;
	gen operator [](const gen & i) const;
	gen operator_at(int i, GIAC_CONTEXT) const;
	gen operator_at(const gen & i, GIAC_CONTEXT) const;
	gen & operator [](int i) ;
	gen & operator [](const gen & i) ;
	gen operator()(const gen & i, GIAC_CONTEXT) const;
	gen operator()(const gen & i, const gen & progname, GIAC_CONTEXT) const;
	bool islesscomplexthan(const gen & other) const;
	bool is_approx() const ; // true if double/real or cmplx with re/im
	int symb_size() const;
	gen change_subtype(int newsubtype);
	bool is_symb_of_sommet(const unary_function_ptr & u) const ;
	bool is_symb_of_sommet(const unary_function_ptr * u) const ;
	gen makegen(int i) const; // make a gen of same type as this with integer i
	// For compatibility with older versions
	inline mpz_t * ref_ZINTptr() const ;
	inline real_object * ref_REALptr() const ;
	inline gen * ref_CPLXptr() const ;
	inline identificateur * ref_IDNTptr() const ;
	inline symbolic * ref_SYMBptr() const ;
	inline gen * ref_MODptr() const ;
	inline Tfraction<gen> * ref_FRACptr() const ;
	inline gen * ref_EXTptr() const ;
	inline polynome * ref_POLYptr() const ;
	inline vecteur * ref_VECTptr() const ;
	inline sparse_poly1 * ref_SPOL1ptr() const ;
	inline std::string * ref_STRNGptr() const ;
	inline unary_function_ptr * ref_FUNCptr() const ;
	inline gen_user * ref_USERptr() const ;
	inline gen_map * ref_MAPptr() const ;
	inline eqwdata * ref_EQWptr() const ;
	inline grob * ref_GROBptr() const ;
	inline void * ref_POINTER_val() const ;
};

gen change_subtype(const gen &g, int newsubtype);
gen genfromstring(const std::string & s);
// pointer to an int describing display mode for complex numbers
int * complex_display_ptr(const gen & g);
// value==0 to cartesian, 1 to polar, 2 toggle, 3 count complex
// returns the number of complex
int adjust_complex_display(gen & res, int value);

struct alias_gen {
	unsigned char type;  // see dispatch.h
	signed char subtype;
	unsigned short reserved; // not used
#ifdef DOUBLEVAL
	longlong value;
#else
	long value ;
#endif
};


class vectpoly: public std::vector<polynome> {
public:
	vectpoly(): std::vector<polynome>::vector() {};
	vectpoly(size_t i, const polynome & p): std::vector<polynome>::vector(i, p) {};
	void dbgprint() {
#ifndef NSPIRE
		CERR << *this << std::endl;
#endif
	}
};

struct ref_gen_map {
	volatile ref_count_t ref_count;
	gen_map m;
#if 1 // def NSPIRE
	ref_gen_map(): ref_count(1), m() {}
#else
	ref_gen_map(const std::pointer_to_binary_function < const gen &, const gen &, bool> & p): ref_count(1), m(p) {}
#endif
	ref_gen_map(const gen_map & M): ref_count(1), m(M) {}
};

struct alias_ref_fraction { ref_count_t ref_count; alias_gen num; alias_gen den; };
struct alias_ref_complex {
	ref_count_t ref_count;
	int display;
	alias_gen re, im;
};

struct ref_vecteur {
	volatile ref_count_t ref_count;
	vecteur v;
	ref_vecteur(): ref_count(1) {}
	ref_vecteur(unsigned s): ref_count(1), v(s) {}
	ref_vecteur(unsigned s, const gen & g): ref_count(1), v(s, g) {}
	ref_vecteur(const_iterateur it, const_iterateur itend): ref_count(1), v(it, itend) {}
	ref_vecteur(const vecteur & w): ref_count(1), v(w) {}
};



#ifdef DOUBLEVAL
#define define_alias_gen(name,type,subtype,ptr) alias_gen name={type,subtype,0,ulonglong(ptr)};
#define define_alias_ref_symbolic(name,sommet,type,subtype,ptr) alias_ref_symbolic name={-1,(unary_function_eval *)sommet,type,subtype,0,ulonglong(ptr)};
#define define_alias_ref_fraction(name,numtype,numsubtype,numptr,dentype,densubtype,denptr) alias_ref_fraction name={-1,{numtype,numsubtype,0,ulonglong(numptr)},{dentype,densubtype,0,ulonglong(denptr)}};
#define define_alias_ref_complex(name,retype,resubtype,reptr,imtype,imsubtype,imptr) alias_ref_complex name={-1,0,{retype,resubtype,0,ulonglong(reptr)},{imtype,imsubtype,0,ulonglong(imptr)}};
#define define_tab2_alias_gen(name,retype,resubtype,reptr,imtype,imsubtype,imptr) alias_gen name[]={{retype,resubtype,0,ulonglong(reptr)},{imtype,imsubtype,0,ulonglong(imptr)}};
#else
#define define_alias_gen(name,type,subtype,ptr) alias_gen name={type,subtype,0,long(ptr)};
#define define_alias_ref_symbolic(name,sommet,type,subtype,ptr) alias_ref_symbolic name={-1,(unary_function_eval *)sommet,type,subtype,0,long(ptr)};
#define define_alias_ref_fraction(name,numtype,numsubtype,numptr,dentype,densubtype,denptr) alias_ref_fraction name={-1,{numtype,numsubtype,0,long(numptr)},{dentype,densubtype,0,long(denptr)}};
#define define_alias_ref_complex(name,retype,resubtype,reptr,imtype,imsubtype,imptr) alias_ref_complex name={-1,0,{retype,resubtype,0,long(reptr)},{imtype,imsubtype,0,long(imptr)}};
#define define_tab2_alias_gen(name,retype,resubtype,reptr,imtype,imsubtype,imptr) alias_gen name[]={{retype,resubtype,0,long(reptr)},{imtype,imsubtype,0,long(imptr)}};
#endif


// ? #ifdef __GNUC__
#ifdef IMMEDIATE_VECTOR
struct alias_ref_vecteur { ref_count_t ref_count; const int _taille; const alias_gen * begin_immediate_vect; const alias_gen * end_immediate_vect; void * ptr; };
#define define_alias_ref_vecteur(name,b) alias_ref_vecteur name={-1,sizeof(b)/sizeof(gen),(const alias_gen *)b,(const alias_gen *)b+sizeof(b)/sizeof(gen),0};
#define define_alias_ref_vecteur2(name,b) alias_ref_vecteur name={-1,2,&b[0],&b[2],0};
#else
struct alias_ref_vecteur { ref_count_t ref_count; const alias_gen * begin; const alias_gen * end; const alias_gen * finish; void * ptr; };
#define define_alias_ref_vecteur(name,b) alias_ref_vecteur name={-1,(const alias_gen *)b,(const alias_gen *)b+sizeof(b)/sizeof(gen),(const alias_gen *)b+sizeof(b)/sizeof(gen),0};
#define define_alias_ref_vecteur2(name,b) alias_ref_vecteur name={-1,&b[0],&b[2],&b[2],0};
#endif

struct ref_complex {
	volatile ref_count_t ref_count;
	int display;
	gen re, im;
	ref_complex(const gen & R, const gen & I): ref_count(1), display(0), re(R), im(I) {}
	ref_complex(const gen & R, const gen & I, int display_mode): ref_count(1), display(display_mode), re(R), im(I) {}
};
struct ref_modulo {
	volatile ref_count_t ref_count;
	gen n, modulo;
	ref_modulo(): ref_count(1) {}
	ref_modulo(const gen &N, const gen &M): ref_count(1), n(N), modulo(M) {}
};
struct ref_algext {
	volatile ref_count_t ref_count;
	gen P, Pmin, additional;
	ref_algext(): ref_count(1) {}
};

bool poly_is_real(const polynome & p);
polynome addpoly(const polynome & p, const gen & c);
polynome subpoly(const polynome & p, const gen & c);
bool islesscomplexthanf(const gen & a, const gen & b);
void islesscomplexthanf_sort(iterateur it, iterateur itend);
void gen_sort_f(iterateur it, iterateur itend, bool (*f)(const gen &a, const gen &b));
void gen_sort_f_context(iterateur it, iterateur itend, bool (*f)(const gen &a, const gen &b, GIAC_CONTEXT), GIAC_CONTEXT);
gen makemap(); // make a new map
gen chartab2gen(char * & s, GIAC_CONTEXT);


bool is_zero(const gen & a, GIAC_CONTEXT0);
bool is_exactly_zero(const gen & a);
bool is_one(const gen & a);
bool is_minus_one(const gen & a);
bool is_sq_minus_one(const gen & a);
bool is_inf(const gen & e);
bool is_undef(const gen & e);
bool is_undef(const polynome & p);
bool is_undef(const vecteur & v);
bool is_undef(const sparse_poly1 & s);
bool is_zero__VECT(const vecteur & a, GIAC_CONTEXT);
bool has_denominator(const gen & n);
bool has_i(const gen & g);

// basic arithmetic
gen operator && (const gen & a, const gen & b);
gen operator || (const gen & a, const gen & b);
gen operator_plus(const gen & a, const gen & b, GIAC_CONTEXT);
gen operator + (const gen & a, const gen & b);
gen operator_plus_eq(gen & a, const gen & b, GIAC_CONTEXT);
inline gen operator += (gen & a, const gen & b) {
	return operator_plus_eq(a, b, giac::context0);
}
Tfraction<gen> operator + (const Tfraction<gen> & a, const Tfraction<gen> & b); // specialization
gen sym_add(const gen & a, const gen & b, GIAC_CONTEXT);
gen operator_minus_eq(gen & a, const gen & b, GIAC_CONTEXT);
inline gen operator -= (gen & a, const gen & b) {
	return operator_minus_eq(a, b, giac::context0);
}
gen operator_minus(const gen & a, const gen & b, GIAC_CONTEXT);
gen operator - (const gen & a, const gen & b);
gen operator - (const gen & a);
gen sym_sub(const gen & a, const gen & b, GIAC_CONTEXT);
gen operator_times(const gen & a, const gen & b, GIAC_CONTEXT);
gen operator * (const gen & a, const gen & b);
inline gen operator * (int a, const gen & b){ return gen(a)*b; }
inline gen operator * (double a, const gen & b){ return gen(a)*b; }
gen sym_mult(const gen & a, const gen & b, GIAC_CONTEXT);
gen pow(const gen & base, const gen & exponent, GIAC_CONTEXT);
gen giac_pow(const gen & base, const gen & exponent, GIAC_CONTEXT);
gen iquo(const gen & a, const gen & b); // same
gen irem(const gen & a, const gen & b, gen & q); // same
gen smod(const gen & a, const gen & b); // same
void smod(const vecteur & v, const gen & g, vecteur & w);
vecteur smod(const vecteur & a, const gen & b); // same
gen rdiv(const gen & a, const gen & b, GIAC_CONTEXT0); // rational division
inline gen operator /(const gen & a, const gen & b){ return rdiv(a, b); };
gen operator %(const gen & a, const gen & b); // for int only
// gen inv(const gen & a);
gen inv(const gen & a, GIAC_CONTEXT);
inline wchar_t * wprint(const gen & g, GIAC_CONTEXT){ return g.wprint(contextptr); }

inline void swapgen(gen & a, gen &b) {
	gen tmp = a; a = b; b = tmp;
}
gen algebraic_EXTension(const gen & a, const gen & v);
gen ext_reduce(const gen & a, const gen & v);
gen maptoarray(const gen_map & m, GIAC_CONTEXT);
gen evalf_VECT(const vecteur & v, int subtype, int level, const context * contextptr);
gen m_gamma(int nbits); // Euler gamma constant precision nbits
gen m_gamma(GIAC_CONTEXT);
gen m_pi(int nbits); // pi precision nbits
gen m_pi(GIAC_CONTEXT);

// a*b -> tmp, may modify tmp in place
void type_operator_times(const gen & a, const gen &b, gen & tmp);
// c += a*b, may modify c in place
/*
inline void type_operator_plus_times(const gen & a,const gen & b,gen & c){
  gen g;
  type_operator_times(a,b,g);
  c += g;
}
*/
void type_operator_plus_times(const gen & a, const gen & b, gen & c);

inline void type_operator_plus_times_reduce(const gen & a, const gen & b, gen & c, int reduce) {
	type_operator_plus_times(a, b, c);
	if (reduce)
		c = smod(c, reduce);
}

inline void type_operator_reduce(const gen & a, const gen & b, gen & c, int reduce) {
	type_operator_times(a, b, c);
	if (reduce)
		c = smod(c, reduce);
}

bool operator ==(const gen & a, const gen & b);
bool operator ==(const gen & a, const identificateur & b);
bool operator_equal(const gen & a, const gen & b, GIAC_CONTEXT);
bool operator !=(const gen & a, const gen & b);
inline bool operator !=(const gen & a, const identificateur & b){ return !(a == b); }
gen equal(const gen & a, const gen &b, GIAC_CONTEXT);
gen equal2(const gen & a, const gen &b, GIAC_CONTEXT);

gen operator !(const gen & a);

int fastsign(const gen & a, GIAC_CONTEXT);  // 0 if unknown, 1 if >0, -1 if <0
gen sign(const gen & a, GIAC_CONTEXT);

// Large tests if strictly not precised, if sign is unknown return false
bool is_greater(const gen & a, const gen &b, GIAC_CONTEXT);
bool is_strictly_greater(const gen & a, const gen &b, GIAC_CONTEXT);
inline bool operator > (const gen & a, const gen & b) {
	return is_strictly_greater(a, b, giac::context0);
}
bool is_positive(const gen & a, GIAC_CONTEXT);
bool is_strictly_positive(const gen & a, GIAC_CONTEXT);
// Large tests if strictly not precised, if sign is unknown make an error
bool ck_is_greater(const gen & a, const gen &b, GIAC_CONTEXT);
bool ck_is_strictly_greater(const gen & a, const gen &b, GIAC_CONTEXT);
bool ck_is_positive(const gen & a, GIAC_CONTEXT);
bool ck_is_strictly_positive(const gen & a, GIAC_CONTEXT);
gen superieur_strict(const gen & a, const gen & b, GIAC_CONTEXT);
gen superieur_egal(const gen & a, const gen & b, GIAC_CONTEXT);
gen inferieur_strict(const gen & a, const gen & b, GIAC_CONTEXT);
gen inferieur_egal(const gen & a, const gen & b, GIAC_CONTEXT);
bool symb_size_less(const gen & a, const gen & b);

gen min(const gen & a, const gen & b, GIAC_CONTEXT);
gen max(const gen & a, const gen & b, GIAC_CONTEXT = context0);
// default context0 is required for instantiation in poly.h
gen factorial(unsigned long int i);
gen comb(unsigned long int i, unsigned long j);
gen perm(unsigned long int i, unsigned long j);
gen pow(const gen & base, unsigned long int exponent);
gen pow(const gen & base, int exponent);
gen pow(unsigned long int base, unsigned long int exponent);

// more advanced arithmetic
gen gcd(const gen & A, const gen & B, GIAC_CONTEXT);
gen gcd(const gen & A, const gen & B);
gen lcm(const gen & a, const gen & b);
gen simplify(gen & n, gen & d);
void egcd(const gen &a, const gen &b, gen & u, gen &v, gen &d);
gen ichinrem(const gen & a, const gen &b, const gen & amod, const gen & bmod);
gen invmod(const gen & A, const gen & modulo);
gen fracmod(const gen & a_orig, const gen & modulo); // -> p/q=a mod modulo
bool fracmod(const gen & a_orig, const gen & modulo, gen & res);
bool in_fracmod(const gen &m, const gen & a, mpz_t & d, mpz_t & d1, mpz_t & absd1, mpz_t &u, mpz_t & u1, mpz_t & ur, mpz_t & q, mpz_t & r, mpz_t &sqrtm, mpz_t & tmp, gen & num, gen & den);
gen powmod(const gen &base, const gen & expo, const gen & modulo);
gen isqrt(const gen & A);
gen re(const gen & a, GIAC_CONTEXT);
gen no_context_re(const gen & a);
gen im(const gen & a, GIAC_CONTEXT);
gen no_context_im(const gen & a);
void reim(const gen & g, gen & r, gen & i, GIAC_CONTEXT);
gen conj(const gen & a, GIAC_CONTEXT);
gen no_context_conj(const gen & a);
gen sq(const gen & a);
gen abs(const gen & a, const context * contextptr = context0);
// default context0 is required for instantiation in poly.h
gen linfnorm(const gen & a, const context * contextptr = context0);
// default context0 is required for instantiation in poly.h
gen arg(const gen & a, GIAC_CONTEXT);
gen arg_CPLX(const gen & a, GIAC_CONTEXT);
int is_perfect_square(const gen & A);
int is_probab_prime_p(const gen & A);
gen nextprime(const gen & a); // more precisely next probably prime
gen prevprime(const gen & a); // more precisely prev probably prime
int jacobi(const gen & A, const gen &B);
int legendre(const gen & A, const gen & B);
vecteur pascal_next_line(const vecteur & v);
vecteur pascal_nth_line(int n);
// convert a __VECTOR__VECT vecteur to a normal vecteur
gen vector2vecteur(const vecteur & v);

// if b is a _MOD, returns a as a b _MOD
gen chkmod(const gen& a, const gen & b);
// make a _MOD a%b
gen makemod(const gen & a, const gen & b);
// same without evaluating %
gen makemodquoted(const gen & a, const gen & b);

// from a sum in x returns a list of [coeff monomial]
// e.g. 5+2x+3*x*y -> [ [5 1] [2 x] [ 3 x*y] ]
vecteur symbolique2liste(const gen & x, GIAC_CONTEXT);
// v should be sorted and shrinked
gen liste2symbolique(const vecteur & v);

bool is_atomic(const gen & e);
gen _FRAC2_SYMB(const fraction & f);
gen _FRAC2_SYMB(const gen & e);
gen _FRAC2_SYMB(const gen & n, const gen & d);
gen string2gen(const std::string & ss, bool remove_ss_quotes = true);
// by default ss is assumed to be delimited by " and "
std::complex<double> gen2complex_d(const gen & e);
gen eval_VECT(const vecteur & v, int subtype, int level, const context * context_ptr);
// functional equivalent of gen methods
inline gen eval(const gen & e, int level, const context * contextptr){ return e.eval(level, contextptr); };
inline gen eval(const gen & e, const context * contextptr){ return e.eval(eval_level(contextptr), contextptr); };
gen no_context_evalf(const gen & e);
gen evalf(const gen & e, int level, const context * contextptr);
gen evalf2bcd_nock(const gen & g0, int level, const context * contextptr);
gen evalf2bcd(const gen & g0, int level, const context * contextptr);
inline gen evalf_double(const gen & e, int level, const context * contextptr){ return e.evalf_double(level, contextptr); };
// return true if g can be converted to a double or real or complex
bool has_evalf(const gen & g, gen & res, int level, const context * contextptr);
inline std::string print(const gen & e, context * contextptr){ return e.print(contextptr); }
inline bool is_real(const gen & g, GIAC_CONTEXT){ return g.is_real(contextptr); }
inline  bool is_cinteger(const gen & g){ return g.is_cinteger();}  ;
inline  bool is_integer(const gen & g){ return g.is_integer(); }  ;
double int2double(int i);
inline  bool is_constant(const gen & g){ return g.is_constant(); } ;
inline bool is_approx(const gen & g){ return g.is_approx(); };
gen aplatir_fois_plus(const gen & g);
gen collect(const gen & g, GIAC_CONTEXT);

class gen_user {
public:
	virtual gen_user * memory_alloc() const { gen_user * ptr = new gen_user(*this); return ptr; }
	virtual ~gen_user() {};
	// redefine operations if it makes sense.
	// You can redefine gen_user + gen_user for speed
	virtual gen operator + (const gen &) const { return gensizeerr(gettext("+ not redefined")); }
	virtual gen operator + (const gen_user & a) const { return (*this) + gen(a); }
	virtual gen operator - (const gen &) const { return gensizeerr(gettext("Binary - not redefined")); }
	virtual gen operator - (const gen_user & a) const { return (*this) - gen(a); }
	virtual gen operator - () const { return gensizeerr(gettext("Unary - not redefined")); }
	virtual gen operator * (const gen &) const { return gensizeerr(gettext("Binary * not redefined")); }
	virtual gen operator * (const gen_user & a) const { return (*this) * gen(a); }
	virtual gen operator / (const gen_user & a) const { return (*this) * a.inv(); }
	virtual gen operator / (const gen & a) const { return gensizeerr(gettext("Binary / not redefined")); }
	virtual bool is_zero() const {
#ifndef NO_STDEXCEPT
		setsizeerr(gettext("==0 not redefined"));
#endif
		return false;
	}
	virtual bool is_one() const {
#ifndef NO_STDEXCEPT
		setsizeerr(gettext("==1 not redefined"));
#endif
		return false;
	}
	virtual bool is_minus_one() const {
#ifndef NO_STDEXCEPT
		setsizeerr(gettext("==-1 not redefined"));
#endif
		return false;
	}
	virtual gen inv() const { return gensizeerr(gettext("Inv not redefined")); }
	virtual gen conj(GIAC_CONTEXT) const { return gensizeerr(gettext("Conj not redefined"));}
	virtual gen re(GIAC_CONTEXT) const { return gensizeerr(gettext("Real part not redefined"));}
	virtual gen im(GIAC_CONTEXT) const { return gensizeerr(gettext("Imaginary part not redefined")); }
	virtual gen abs(GIAC_CONTEXT) const { return gensizeerr(gettext("Abs not redefined"));}
	virtual gen arg(GIAC_CONTEXT) const { return gensizeerr(gettext("Arg not redefined")); }
	virtual gen sqrt(GIAC_CONTEXT) const { return gensizeerr(gettext("Sqrt not redefined")); }
	virtual gen operator()(const gen &, GIAC_CONTEXT) const { return gensizeerr(gettext("() not redefined")); }
	virtual gen operator [](const gen &) { return gensizeerr(gettext("[] not redefined")); }
	virtual bool operator == (const gen &) const {
#ifndef NO_STDEXCEPT
		setsizeerr(gettext("== not redefined"));
#endif
		return false;
	}
	virtual bool operator == (const gen_user & a) const { return (*this) == gen(a); }
	// must redefine > AND <= since we do not have symetrical type arguments
	virtual gen operator > (const gen &) const { return gensizeerr(gettext("> not redefined")); }
	virtual gen operator > (const gen_user & a) const { return superieur_strict(*this, gen(a), 0); }
	virtual gen operator <= (const gen &) const { return gensizeerr(gettext("<= not redefined")); }
	virtual gen operator <= (const gen_user & a) const { return inferieur_egal(*this, gen(a), 0); }
	virtual gen polygcd(const polynome &, const polynome &, polynome &) const { return gensizeerr(gettext("Polynomial gcd not redefined")); }
	virtual gen polyfactor(const polynome & p,
			factorization & f) const {
		return gensizeerr(gettext("Polynomial gcd not redefined"));
	}
	virtual gen gcd(const gen &) const { return gensizeerr(gettext("gcd not redefined")); }
	virtual gen gcd(const gen_user & a) const { return gcd(gen(a)); }
	virtual std::string print(GIAC_CONTEXT) const { return  "Nothing_to_print";}
	void dbgprint() const { CERR << this->print(0) << std::endl;}
	virtual std::string texprint(GIAC_CONTEXT) const { return "Nothing_to_print_tex"; }
	virtual gen eval(int level, const context * contextptr) const {return *this;};
	virtual gen evalf(int level, const context * contextptr) const {return *this;};
	virtual gen makegen(int i) const { return string2gen("makegen not redefined"); } ;
	virtual gen rand(GIAC_CONTEXT) const { return string2gen("rand not redefined"); };
};
struct ref_gen_user {
	volatile ref_count_t ref_count;
	gen_user * u;
	ref_gen_user(const gen_user & U): ref_count(1), u(U.memory_alloc()) {}
	~ref_gen_user() {delete u;}
};

std::string print_the_type(int val, GIAC_CONTEXT);

// I/O
#ifdef NSPIRE
template<class T> nio::ios_base<T> & operator<<(nio::ios_base<T> & os, const gen & a) {
	return os << a.print(context0);
}
template<class T> nio::ios_base<T> & operator>>(nio::ios_base<T> & is, gen & a);
#else
std::ostream & operator << (std::ostream & os, const gen & a);
std::istream & operator >> (std::istream & is, gen & a);
#endif

#if defined(GIAC_GENERIC_CONSTANTS) // || (defined(VISUALC) && !defined(RTOS_THREADX)) || defined(__x86_64__)
extern const gen zero;
#else
extern const gen & zero;
#endif

struct monome {
	gen coeff;
	gen exponent;
	monome(): coeff(0), exponent(0) {};
	monome(const gen & mycoeff) : coeff(mycoeff), exponent(zero) {};
	monome(const gen &mycoeff, const gen &myexponent) : coeff(mycoeff), exponent(myexponent) {};
	std::string print() const ;
	void dbgprint() const ;
};
#ifdef NSPIRE
template<class T> nio::ios_base<T> & operator<<(nio::ios_base<T> & os, const monome & m){    return os << m.print() ;}
#else
std::ostream & operator << (std::ostream & os, const monome & m);
#endif
inline bool operator == (const monome & a, const monome & b){ return a.coeff == b.coeff && a.exponent == b.exponent; }
inline bool operator != (const monome & a, const monome & b){ return a.coeff != b.coeff || a.exponent != b.exponent; }
polynome apply(const polynome & p, const context * contextptr, gen(* f)(const gen &, const context *));

const char * printi(GIAC_CONTEXT);
std::string hexa_print_ZINT(const mpz_t & a);
std::string octal_print_ZINT(const mpz_t & a);
std::string binary_print_ZINT(const mpz_t & a);
std::string print_ZINT(const mpz_t & a);
std::string printinner_VECT(const vecteur & v, int subtype, GIAC_CONTEXT);
std::string & add_printinner_VECT(std::string & s, const vecteur &v, int subtype, GIAC_CONTEXT);
std::string begin_VECT_string(int subtype, bool tex, GIAC_CONTEXT);
std::string end_VECT_string(int subtype, bool tex, GIAC_CONTEXT);
std::string print_VECT(const vecteur & v, int subtype, GIAC_CONTEXT); // subtype was 0 by default
std::string print_SPOL1(const sparse_poly1 & p, GIAC_CONTEXT);
std::string print_STRNG(const std::string & s);
std::string printint32(int val, int subtype, GIAC_CONTEXT);
std::string print_FLOAT_(const giac_float & f, GIAC_CONTEXT);
// find closing or opening () [] {}
bool matchpos(const std::string & s, int & pos);
std::string cut_string(const std::string & chaine, int nchar, std::vector<int> & ligne_end) ;
std::string calc_endlines_positions(const vecteur & history_in, const vecteur & history_out, int nchar, std::vector<int> & endlines, std::vector<int> & positions);
bool is_operator_char(char c);
void increase_selection(const std::string & s, int & pos1, int& pos2);
void decrease_selection(const std::string & s, int & pos1, int& pos2);
void move_selection_right(const std::string & s, int & pos1, int & pos2);
void move_selection_left(const std::string & s, int & pos1, int & pos2);
std::string remove_extension(const std::string & chaine);


// This type collects global variables to enable threading
struct environment {
	gen modulo; // characteristic
	bool moduloon; // Set to false if non modular arithmetic required
	bool complexe; // true if working on Z/pZ[i]
	gen pn; // cardinal of the field, 0 means equal to modulo
	gen coeff; // exemple of coeff, so that we can call coeff.makegen
	environment() {
		modulo = 13;
		moduloon = false;
		complexe = false;
		coeff = pn = 0;
	}
};

struct ref_sparse_poly1 {
	volatile ref_count_t ref_count;
	sparse_poly1 s;
	ref_sparse_poly1(const sparse_poly1 & S): ref_count(1), s(S) {}
};


// extern environment * env;

struct attributs {
	int fontsize;
	int background;
	int text_color;
	attributs(int f, int b, int t): fontsize(f), background(b), text_color(t) {};
	attributs(): fontsize(0), background(0), text_color(0) {};
};

// Terminal data for EQW display
struct eqwdata {
	gen g;
	attributs eqw_attributs;
	int x, y, dx, dy;
	bool selected;
	bool active;
	bool hasbaseline;
	bool modifiable;
	int baseline;
	eqwdata(int dxx, int dyy, int xx, int yy, const attributs & a, const gen& gg): g(gg), eqw_attributs(a), x(xx), y(yy), dx(dxx), dy(dyy), selected(false), active(false), hasbaseline(false), modifiable(true), baseline(0) {};
	eqwdata(int dxx, int dyy, int xx, int yy, const attributs & a, const gen& gg, int mybaseline): g(gg), eqw_attributs(a), x(xx), y(yy), dx(dxx), dy(dyy), selected(false), active(false), hasbaseline(true), modifiable(true), baseline(mybaseline) {};
	void dbgprint(){ CERR << g << ":" << dx << "," << dy << "+" << x << "," << y << "," << baseline << "," << eqw_attributs.fontsize << "," << eqw_attributs.background << "," << eqw_attributs.text_color << std::endl; }
};
struct ref_eqwdata {
	volatile ref_count_t ref_count;
	eqwdata e;
	ref_eqwdata(const eqwdata & E): ref_count(1), e(E) {}
};


class identificateur {
public:
	int * ref_count;
	gen * value;
	// std::string * name;
	const char * id_name;
	vecteur * localvalue;
	// value / localvalue might be an assumption if it's a vecteur
	// of subtype _ASSUME__VECT
	// The first gen of an assumption vecteur is the type (_FRAC for rational)
	// If the type is _REAL, the vecteur has 2 other elements
	// * an interval or a _SET_VECT of intervals
	//   where interval=vecteur of length 2 of subtype _LINE__VECT
	// * a list of excluded particular values
	// If the type is _DOUBLE_ the variable will be evalf-ed but not eval-ed
	// This is useful in geometry to make figures and get exact results
	// If the type is _INT_ it
	short int * quoted;
	identificateur();
	explicit identificateur(const std::string & s);
	explicit identificateur(const char * s);
#ifdef GIAC_HAS_STO_38
	explicit identificateur(const char * s, bool StringIsNowYours); // creates identifier. if StringIsNowYours, then the string will be freed when id is destroyed...
#endif
	explicit identificateur(const char * s, const gen & e);
	identificateur(const std::string & s, const gen & e);
	identificateur(const identificateur & s);
	~identificateur();
	identificateur & operator =(const identificateur & s);
	gen eval(int level, const gen & orig, const context * context_ptr) ;
	bool in_eval(int level, const gen & orig, gen & evaled, const context * context_ptr, bool No38Lookup = false); // if No38Lookup, does not check if HP38 knows about this name...
	const char * print(const context * context_ptr) const ;
	std::string name() const { return id_name; }
	void dbgprint() const { COUT << this->print(context0); }
	void unassign() ;
	void push(int protection, const gen & e);
	bool operator ==(const identificateur & i);
	bool operator ==(const gen & i);
	inline bool operator !=(const identificateur & i){ return !(*this == i); }
	inline bool operator !=(const gen & i){ return !(*this == i);}
	void MakeCopyOfNameIfNotLocal(); ///< if the name is not dynamicaly allocated, create a copy for that id.
};
struct ref_identificateur {
	volatile ref_count_t ref_count;
	identificateur i;
	ref_identificateur(const char * s): ref_count(1), i(s) {}
	ref_identificateur(const std::string & s): ref_count(1), i(s) {}
	ref_identificateur(const identificateur & s): ref_count(1), i(s) {}
};
struct alias_ref_identificateur {
	ref_count_t i;
	int * ref_count;
	gen * value;
	const char * id_name;
	vecteur * localvalue;
	short int * quoted;
};

struct ref_unary_function_ptr {
	volatile ref_count_t ref_count;
	unary_function_ptr u;
	ref_unary_function_ptr(const unary_function_ptr & U): ref_count(1), u(U) {}
	ref_unary_function_ptr(const unary_function_ptr * U): ref_count(1), u(*U) {}
};
struct symbolic {
	unary_function_ptr sommet;
	gen feuille;
	symbolic(const unary_function_ptr & o, const gen & e): sommet(o), feuille(e) {chk();};
	symbolic(const unary_function_ptr & o, const gen & e1, const gen &e2): sommet(o), feuille(makevecteur(e1, e2)) {chk();};
	symbolic(const unary_function_ptr & o, const gen & e1, const gen &e2, const gen & e3): sommet(o), feuille(makevecteur(e1, e2, e3)) {chk();};
	symbolic(const unary_function_ptr & o, const gen & e1, const gen &e2, const gen & e3, const gen & e4): sommet(o), feuille(makevecteur(e1, e2, e3, e4)) {chk();};
	symbolic(const unary_function_ptr * o, const gen & e): sommet(*o), feuille(e) {chk();};
	symbolic(const unary_function_ptr * o, const gen & e1, const gen &e2): sommet(*o), feuille(makevecteur(e1, e2)) {chk();};
	symbolic(const unary_function_ptr * o, const gen & e1, const gen &e2, const gen & e3): sommet(*o), feuille(makevecteur(e1, e2, e3)) {chk();};
	symbolic(const unary_function_ptr * o, const gen & e1, const gen &e2, const gen & e3, const gen & e4): sommet(*o), feuille(makevecteur(e1, e2, e3, e4)) {chk();};
	symbolic(const symbolic & mys) : sommet(mys.sommet), feuille(mys.feuille) {chk();};
	symbolic(const symbolic & mys, const gen & e);
	symbolic(const gen & a, const unary_function_ptr & o, const gen & b);
	symbolic(const gen & a, const unary_function_ptr * o, const gen & b);
	std::string print(GIAC_CONTEXT) const;
	void dbgprint() const{ COUT << this->print(context0) << std::endl; }
	gen eval(int level, const context * context_ptr) const;
	gen evalf(int level, const context * context_ptr) const;
	int size() const;
	void chk() const;
};

struct ref_symbolic {
	volatile ref_count_t ref_count;
	symbolic s;
	ref_symbolic(const symbolic & S): ref_count(1), s(S) {}
};

struct alias_ref_symbolic {
	ref_count_t ref_count;
	unary_function_eval * sommet;
	unsigned char type;  // see dispatch.h
	signed char subtype;
	unsigned short reserved; // not used
#ifdef DOUBLEVAL
	longlong value;
#else
	long value ;
#endif
};



inline mpz_t * gen::ref_ZINTptr() const { return &__ZINTptr->z; }
inline real_object * gen::ref_REALptr() const { return &__REALptr->r; }
inline gen * gen::ref_CPLXptr() const { return &__CPLXptr->re; }
inline gen * gen::ref_MODptr() const { return &__MODptr->n; }
inline gen * gen::ref_EXTptr() const { return &__EXTptr->P; }
inline vecteur * gen::ref_VECTptr() const { return &__VECTptr->v; }
inline sparse_poly1 * gen::ref_SPOL1ptr() const { return &__SPOL1ptr->s; }
inline std::string * gen::ref_STRNGptr() const { return &__STRNGptr->s; }
inline gen_user * gen::ref_USERptr() const { return __USERptr->u; }
inline gen_map * gen::ref_MAPptr() const { return &__MAPptr->m; }
inline eqwdata * gen::ref_EQWptr() const { return &__EQWptr->e; }
inline grob * gen::ref_GROBptr() const { return &__GROBptr->g; }
inline void * gen::ref_POINTER_val() const { return __POINTERptr->p; }
inline Tfraction<gen> * gen::ref_FRACptr() const { return &__FRACptr->f; }
inline polynome * gen::ref_POLYptr() const { return &__POLYptr->t; }
inline identificateur * gen::ref_IDNTptr() const {return &__IDNTptr->i; }
inline symbolic * gen::ref_SYMBptr() const { return &__SYMBptr->s; }
// inline unary_function_ptr * gen::ref_FUNCptr() const { return &__FUNCptr->u; }
inline unary_function_ptr * gen::ref_FUNCptr() const { return (unary_function_ptr*) &_FUNC_; }


#ifndef DOUBLEVAL
#define _DOUBLE_val DOUBLE_val()
#define _FLOAT_val FLOAT_val()
#endif

#define  _ZINTptr ref_ZINTptr()
#define	 _REALptr ref_REALptr()
#define  _CPLXptr ref_CPLXptr()
#define  _IDNTptr ref_IDNTptr()
#define  _SYMBptr ref_SYMBptr()
#define  _MODptr ref_MODptr()
#define  _FRACptr ref_FRACptr()
#define  _EXTptr ref_EXTptr()
#define  _POLYptr ref_POLYptr ()
#define  _VECTptr  ref_VECTptr()
#define  _SPOL1ptr ref_SPOL1ptr()
#define  _STRNGptr ref_STRNGptr()
#define  _FUNCptr ref_FUNCptr()
#define  _ROOTptr ref_ROOTptr()
#define  _USERptr ref_USERptr()
#define  _MAPptr ref_MAPptr()
#define  _EQWptr ref_EQWptr()
#define  _GROBptr ref_GROBptr()
#define  _POINTER_val ref_POINTER_val()

// function that are indexed
extern const alias_type alias_at_plus;
extern const alias_type alias_at_neg;
extern const alias_type alias_at_binary_minus;
extern const alias_type alias_at_prod;
extern const alias_type alias_at_division;
extern const alias_type alias_at_inv;
extern const alias_type alias_at_pow;
extern const alias_type alias_at_exp;
extern const alias_type alias_at_ln;
extern const alias_type alias_at_abs;
extern const alias_type alias_at_arg;
extern const alias_type alias_at_pnt;
extern const alias_type alias_at_point;
extern const alias_type alias_at_segment;
extern const alias_type alias_at_sto;
extern const alias_type alias_at_sin;
extern const alias_type alias_at_cos;
extern const alias_type alias_at_tan;
extern const alias_type alias_at_asin;
extern const alias_type alias_at_acos;
extern const alias_type alias_at_atan;
extern const alias_type alias_at_sinh;
extern const alias_type alias_at_cosh;
extern const alias_type alias_at_tanh;
extern const alias_type alias_at_asinh;
extern const alias_type alias_at_acosh;
extern const alias_type alias_at_atanh;
extern const alias_type alias_at_interval;
extern const alias_type alias_at_union;
extern const alias_type alias_at_minus;
extern const alias_type alias_at_intersect;
extern const alias_type alias_at_not;
extern const alias_type alias_at_and;
extern const alias_type alias_at_ou;
extern const alias_type alias_at_inferieur_strict;
extern const alias_type alias_at_inferieur_egal;
extern const alias_type alias_at_superieur_strict;
extern const alias_type alias_at_superieur_egal;
extern const alias_type alias_at_different;
extern const alias_type alias_at_equal;
extern const alias_type alias_at_equal2;
extern const alias_type alias_at_rpn_prog;
extern const alias_type alias_at_local;
extern const alias_type alias_at_return;
extern const alias_type alias_at_Dialog;
extern const alias_type alias_at_double_deux_points;
extern const alias_type alias_at_pointprod;
extern const alias_type alias_at_pointdivision;
extern const alias_type alias_at_pointpow;
extern const alias_type alias_at_hash;
extern const alias_type alias_at_pourcent;
extern const alias_type alias_at_tilocal;
extern const alias_type alias_at_break;
extern const alias_type alias_at_continue;
extern const alias_type alias_at_ampersand_times;
extern const alias_type alias_at_maple_lib;
extern const alias_type alias_at_unit;
extern const alias_type alias_at_plot_style;
extern const alias_type alias_at_xor;
extern const alias_type alias_at_check_type;
extern const alias_type alias_at_quote_pow;
extern const alias_type alias_at_case;
extern const alias_type alias_at_dollar;
extern const alias_type alias_at_IFTE;
extern const alias_type alias_at_RPN_CASE;
extern const alias_type alias_at_RPN_LOCAL;
extern const alias_type alias_at_RPN_FOR;
extern const alias_type alias_at_RPN_WHILE;
extern const alias_type alias_at_NOP;
extern const alias_type alias_at_unit;
extern const alias_type alias_at_ifte;
extern const alias_type alias_at_for;
extern const alias_type alias_at_bloc;
extern const alias_type alias_at_program;
extern const alias_type alias_at_same;
extern const alias_type alias_at_increment;
extern const alias_type alias_at_decrement;
extern const alias_type alias_at_multcrement;
extern const alias_type alias_at_divcrement;
extern const alias_type alias_at_sq;
extern const alias_type alias_at_display;
extern const alias_type alias_at_of;
extern const alias_type alias_at_at;
extern const alias_type alias_at_normalmod;
extern const alias_type alias_at_pointplus;
extern const alias_type alias_at_pointminus;

#ifdef BCD
inline bool ck_gentobcd(const gen & g, accurate_bcd_float * bcdptr) {
	if (g.type != _FLOAT_)
		return false;
	fExpand(g._FLOAT_val.f, bcdptr);
	return true;
}
inline accurate_bcd_float * gentobcd(const gen & g, accurate_bcd_float * bcdptr) {
	return fExpand(g._FLOAT_val.f, bcdptr);
}
#endif

// should be in input_lexer.h
// return true/false to tell if s is recognized. return the appropriate gen if true
bool CasIsBuildInFunction(char const *s, gen &g);

void sprintfdouble(char *, const char *, double d);

const char * caseval(const char *);


}


#endif // _GIAC_GEN_H
