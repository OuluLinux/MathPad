#include "LibGiac.h"
#include "Equation.h"

#ifdef flagGUI

namespace Xcas {

using namespace giac;
using namespace std;
using Upp::min;
using Upp::max;

typedef Font Fl_Font;
typedef Ctrl Fl_Widget;

struct Fl_Color {
	Fl_Color() {}
	Fl_Color(int i) {}
};

Fl_Font FL_HELVETICA = SansSerif(15);
Fl_Font FL_TIMES_BOLD_ITALIC = Serif(15).Bold().Italic();
Fl_Font FL_TIMES_ITALIC = Serif(15).Italic();
int FL_BLUE;
//int undef;
//int clip_x, clip_y, clip_w, clip_h;
int _PRINT__VECT = 123123;

int fnt_sz;
Font fnt;
Color clr;
Draw* draw;

void fl_font(Fl_Font i, int size) {
	fnt_sz = size;
	fnt = i;
	fnt.Height(fnt_sz);
}

void fl_color(Fl_Color c) {
	clr = Black();
}

void fl_draw(const char* c, int x, int y) {
	int y2 = y - fnt_sz;
	draw->DrawText(x, y2, c, fnt, clr);
}

void fl_point(int x, int y) {
	draw->DrawRect(x, y, 1, 1, clr);
}

void fl_line(int x0, int y0, int x1, int y1) {
	draw->DrawLine(x0, y0-2, x1, y1-2, 1, clr);
}

void fl_rect(int x, int y, int w, int h) {
	draw->DrawRect(x, y, w, h, clr);
}

void fl_rectf(int x, int y, int w, int h) {
	draw->DrawRect(x, y, w, h, clr);
}

int fl_width(const char* c) {
	return GetTextSize(c, fnt).cx;
}

void fl_arc(int x, int y, int w, int h, int begin, int end) {
	//draw->DrawArc(RectC(x, y, w, h),
}

namespace Fl {
	Equation* focus() {return NULL;}
};

const giac::context * get_context(Equation* eq) {return eq->ctx;}
void Equation_select(iterateur, bool) {}
void Equation_select(gen, bool) {}
void Equation_select(gen, bool, int) {}
bool Equation_is_selected(gen, bool) {return false;}

// fwd declare
gen Equation_compute_size(const gen & g, const attributs & a, int windowhsize, GIAC_CONTEXT);
gen Equation_nullstring(const attributs & a, int windowhsize, GIAC_CONTEXT);
gen Equation_nullstring();
gen Equation_copy(const gen & g);
gen * Equation_selected(gen & g, attributs & attr, int windowhsize, vecteur & position, bool active_search, GIAC_CONTEXT);
int in_multistring(const gen & data);

Fl_Font cst_greek_translate(string & s0) {
	string s1(s0);
	s0 = "";
	int n = s1.size();
	for (int i = 0;i < n;++i) {
		if (!isalpha(s1[i])) {
			s0 += s1[i];
			continue;
		}
		string s;
		for (;i < n;++i) {
			if (!isalpha(s1[i])) {
				--i;
				break;
			}
			s += s1[i];
		}
		bool done = false;
		switch (s.size()) {
		case 2:
			if (s == "mu") {
				s0 += "μ";
				done = true;
			}
			if (s == "nu") {
				s0 += "ν";
				done = true;
			}
			if (s == "pi") {
				s0 += "π";
				done = true;
			}
			if (s == "xi") {
				s0 += "ξ";
				done = true;
			}
			if (s == "Xi") {
				s0 += "Ξ";
				done = true;
			}
			break;
		case 3:
			if (s == "chi") {
				s0 += "χ";
				done = true;
			}
			if (s == "phi") {
				s0 += "φ";
				done = true;
			}
			if (s == "Phi") {
				s0 += "Φ";
				done = true;
			}
			if (s == "eta") {
				s0 += "η";
				done = true;
			}
			if (s == "rho") {
				s0 += "ρ";
				done = true;
			}
			if (s == "tau") {
				s0 += "τ";
				done = true;
			}
			if (s == "psi") {
				s0 += "ψ";
				done = true;
			}
			if (s == "Psi") {
				s0 += "Ψ";
				done = true;
			}
			break;
		case 4:
			if (s == "beta") {
				s0 += "β";
				done = true;
			}
			if (s == "Beta") {
				s0 += "β";
				done = true;
			}
			if (s == "zeta") {
				s0 += "ζ";
				done = true;
			}
			if (s == "Zeta") {
				s0 += "ζ";
				done = true;
			}
			if (s == "alef") {
				s0 += "ℵ";
				done = true;
			}
			break;
		case 5:
			if (s == "alpha") {
				s0 += "α";
				done = true;
			}
			if (s == "delta") {
				s0 += "δ";
				done = true;
			}
			if (s == "Delta") {
				s0 += "Δ";
				done = true;
			}
			if (s == "gamma") {
				s0 += "γ";
				done = true;
			}
			if (s == "Gamma") {
				s0 += "Γ";
				done = true;
			}
			if (s == "kappa") {
				s0 += "κ";
				done = true;
			}
			if (s == "theta") {
				s0 += "θ";
				done = true;
			}
			if (s == "Theta") {
				s0 += "Θ";
				done = true;
			}
			if (s == "sigma") {
				s0 += "σ";
				done = true;
			}
			if (s == "Sigma") {
				s0 += "Σ";
				done = true;
			}
			if (s == "Omega") {
				s0 += "Ω";
				done = true;
			}
			if (s == "omega") {
				s0 += "ω";
				done = true;
			}
			break;
		case 6:
			if (s == "lambda") {
				s0 += "λ";
				done = true;
			}
			if (s == "Lambda") {
				s0 += "λ";
				done = true;
			}
			break;
		case 7:
			if (s == "epsilon") {
				s0 += "ε";
				done = true;
			}
			if (s == "product") {
				s0 = "Π";
				done = true;
			}
			break;
		case 8:
			if (s == "infinity") {
				s0 += "∞";
				done = true;
			}
			break;
		case 11:
			if (s == "euler_gamma") {
				s0 += "ϒ";
				done = true;
			}
			break;
		} // end switch
		if (!done)
			s0 += s;
	} // end for (int i=0;i<n;i++)
	return FL_HELVETICA;
}


void check_fl_draw(const char * ch, int i0, int j0, int imin, int jmin, int di, int dj, int delta_i, int delta_j) {
	if (strlen(ch) > 2000)
		fl_draw("String too long for display", i0 + delta_i, j0 + delta_j);
	else
		fl_draw(ch, i0 + delta_i, j0 + delta_j);
}

void check_fl_point(int i0, int j0, int imin, int jmin, int di, int dj, int delta_i, int delta_j) {
	fl_point(i0 + delta_i, j0 + delta_j);
}

void check_fl_rect(int i0, int j0, int i1, int j1, int imin, int jmin, int di, int dj, int delta_i, int delta_j) {
	fl_rect(i0 + delta_i, j0 + delta_j, i1, j1);
}

void check_fl_rectf(int x, int y, int w, int h, int imin, int jmin, int di, int dj, int delta_i, int delta_j) {
	fl_rectf(x + delta_i, y + delta_j, w, h);
}

// Calls fl_line, checking with bounds
void check_fl_line(int i0, int j0, int i1, int j1, int imin, int jmin, int di, int dj, int delta_i, int delta_j) {
	fl_line(i0 + delta_i, j0 + delta_j, i1 + delta_i, j1 + delta_j);
}


int Equation_binary_search_pos(const eqwdata & e, int x, int y) {
	int ss = e.g._STRNGptr->size();
	int debut = 0, fin = ss, pos, l = 0;
	fl_font(FL_HELVETICA, e.eqw_attributs.fontsize);
	for (;debut + 1 < fin;) {
		pos = (debut + fin) / 2;
		l = int(fl_width(e.g._STRNGptr->substr(0, pos).c_str()));
		if (e.x + l < x)
			debut = pos;
		else
			fin = pos;
	}
	int l2 = int(fl_width(e.g._STRNGptr->substr(0, fin).c_str()));
	x = x - e.x;
	if (x - l > l2 - x)
		return fin;
	else
		return debut;
}

bool Equation_multistring_selection(const_iterateur & it, const_iterateur & itend, bool search_active) {
	for (;it != itend;++it) { // find first selection
		if (it->type == _EQW) {
			if (search_active) {
				if (it->_EQWptr->active)
					break;
			}
			else {
				if (it->_EQWptr->selected)
					break;
			}
		}
	}
	if (it == itend)
		return false; // nothing selected
	const_iterateur it_end_sel = it + 1;
	for (;it_end_sel != itend;++it_end_sel) { // find last selection
		if (it_end_sel->type == _EQW) {
			if (search_active) {
				if (!it_end_sel->_EQWptr->active)
					break;
			}
			else {
				if (!it_end_sel->_EQWptr->selected)
					break;
			}
		}
	}
	itend = it_end_sel - 1;
	return true;
}

eqwdata Equation_total_size(const gen & g) {
	if (g.type == _EQW)
		return *g._EQWptr;
	if (g.type != _VECT || g._VECTptr->empty())
		return eqwdata(0, 0, 0, 0, attributs(0, 0, 0), undef);
	return Equation_total_size(g._VECTptr->back());
}

// find smallest value of y and height
void Equation_y_dy(const gen & g, int & y, int & dy) {
	y = 0; dy = 0;
	if (g.type == _EQW) {
		y = g._EQWptr->y;
		dy = g._EQWptr->dy;
	}
	if (g.type == _VECT) {
		iterateur it = g._VECTptr->begin(), itend = g._VECTptr->end();
		for (;it != itend;++it) {
			int Y, dY;
			Equation_y_dy(*it, Y, dY);
			// Y, Y+dY and y,y+dy
			int ymax = giacmax(y + dy, Y + dY);
			if (Y < y)
				y = Y;
			dy = ymax - y;
		}
	}
}

void Equation_translate(gen & g, int deltax, int deltay) {
	if (g.type == _EQW) {
		g._EQWptr->x += deltax;
		g._EQWptr->y += deltay;
		g._EQWptr->baseline += deltay;
		return ;
	}
	if (g.type != _VECT)
		setsizeerr();
	vecteur & v = *g._VECTptr;
	iterateur it = v.begin(), itend = v.end();
	for (;it != itend;++it)
		Equation_translate(*it, deltax, deltay);
}

gen Equation_change_attributs(const gen & g, const attributs & newa) {
	if (g.type == _EQW) {
		gen res(*g._EQWptr);
		res._EQWptr->eqw_attributs = newa;
		return res;
	}
	if (g.type != _VECT)
		setsizeerr();
	vecteur v = *g._VECTptr;
	iterateur it = v.begin(), itend = v.end();
	for (;it != itend;++it)
		*it = Equation_change_attributs(*it, newa);
	return gen(v, g.subtype);
}

vecteur Equation_subsizes(const gen & arg, const attributs & a, int windowhsize, GIAC_CONTEXT) {
	vecteur v;
	if ((arg.type == _VECT) && ((arg.subtype == _SEQ__VECT)
			// || (!ckmatrix(arg))
							   )) {
		const_iterateur it = arg._VECTptr->begin(), itend = arg._VECTptr->end();
		for (;it != itend;++it)
			v.push_back(Equation_compute_size(*it, a, windowhsize, contextptr));
	}
	else {
		v.push_back(Equation_compute_size(arg, a, windowhsize, contextptr));
	}
	return v;
}

// vertical merge with same baseline
// for vertical merge of hp,yp at top (like ^) add fontsize to yp
// at bottom (like lower bound of int) substract fontsize from yp
void Equation_vertical_adjust(int hp, int yp, int & h, int & y) {
	int yf = Upp::min(y, yp);
	h = Upp::max(y + h, yp + hp) - yf;
	y = yf;
}

gen Equation_compute_symb_size(const gen & g, const attributs & a, int windowhsize, GIAC_CONTEXT) {
	if (g.type != _SYMB)
		return Equation_compute_size(g, a, windowhsize, contextptr);
	unary_function_ptr & u = g._SYMBptr->sommet;
	gen arg = g._SYMBptr->feuille, rootof_value;
	if (u == at_rootof && arg.type == _VECT && arg._VECTptr->size() == 2 && arg._VECTptr->front().type == _VECT && has_rootof_value(arg._VECTptr->back(), rootof_value, contextptr)) {
		return Equation_compute_symb_size(horner_rootof(*arg._VECTptr->front()._VECTptr, rootof_value, contextptr), a, windowhsize, contextptr);
	}
	if (u == at_multistring) {
		gen tmp = _multistring(arg, contextptr);
		tmp.subtype = 1;
		return Equation_compute_size(tmp, a, windowhsize, contextptr);
	}
	if (u == at_makevector) {
		vecteur v(1, arg);
		if (arg.type == _VECT)
			v = *arg._VECTptr;
		iterateur it = v.begin(), itend = v.end();
		for (;it != itend;++it) {
			if ((it->type == _SYMB) && (it->_SYMBptr->sommet == at_makevector))
				*it = _makevector(it->_SYMBptr->feuille, contextptr);
		}
		return Equation_compute_size(v, a, windowhsize, contextptr);
	}
	if (u == at_makesuite) {
		if (arg.type == _VECT)
			return Equation_compute_size(gen(*arg._VECTptr, _SEQ__VECT), a, windowhsize, contextptr);
		else
			return Equation_compute_size(arg, a, windowhsize, contextptr);
	}
	if (u == at_sqrt)
		return Equation_compute_size(symbolic(at_pow, gen(makevecteur(arg, plus_one_half), _SEQ__VECT)), a, windowhsize, contextptr);
	if (u == at_division) {
		if (arg.type != _VECT || arg._VECTptr->size() != 2)
			return Equation_compute_size(arg, a, windowhsize, contextptr);
		gen tmp;
#ifdef SMARTPTR64
		* ((ulonglong *) &tmp) = ulonglong(new ref_fraction(Tfraction<gen>(arg._VECTptr->front(), arg._VECTptr->back()))) << 16;
#else
		tmp.__FRACptr = new ref_fraction(Tfraction<gen>(arg._VECTptr->front(), arg._VECTptr->back()));
#endif
		tmp.type = _FRAC;
		return Equation_compute_size(tmp, a, windowhsize, contextptr);
	}
	if (u == at_prod) {
		gen n, d;
		if (rewrite_prod_inv(arg, n, d)) {
			if (n.is_symb_of_sommet(at_neg))
				return Equation_compute_size(symbolic(at_neg, Tfraction<gen>(-n, d)), a, windowhsize, contextptr);
			return Equation_compute_size(Tfraction<gen>(n, d), a, windowhsize, contextptr);
		}
	}
	if (u == at_inv) {
		if ((is_integer(arg) && is_positive(-arg, contextptr))
			|| (arg.is_symb_of_sommet(at_neg)))
			return Equation_compute_size(symbolic(at_neg, Tfraction<gen>(plus_one, -arg)), a, windowhsize, contextptr);
		return Equation_compute_size(Tfraction<gen>(plus_one, arg), a, windowhsize, contextptr);
	}
	if (u == at_expr && arg.type == _VECT && arg.subtype == _SEQ__VECT && arg._VECTptr->size() == 2 && arg._VECTptr->back().type == _INT_) {
		gen varg1 = Equation_compute_size(arg._VECTptr->front(), a, windowhsize, contextptr);
		eqwdata vv(Equation_total_size(varg1));
		gen varg2 = eqwdata(0, 0, 0, 0, a, arg._VECTptr->back());
		vecteur v12(makevecteur(varg1, varg2));
		v12.push_back(eqwdata(vv.dx, vv.dy, 0, vv.y, a, at_expr, 0));
		return gen(v12, _SEQ__VECT);
	}
	int llp = int(fl_width("("));
	int lrp = int(fl_width(")"));
	int lc = int(fl_width(","));
	string us = u.ptr()->s;
	fl_font(cst_greek_translate(us), a.fontsize);
	int ls = int(fl_width(us.c_str()));
	fl_font(FL_HELVETICA, a.fontsize);
	if (isalpha(u.ptr()->s[0]))
		ls += 2;
	if (u == at_abs)
		ls = 2;
	// special cases first int, sigma, /, ^
	// and if printed as printsommetasoperator
	// otherwise print with usual functional notation
	int x = 0;
	int h = a.fontsize;
	int y = 0;
	if ((u == at_integrate) || (u == at_sum)) { // Int
		int s = 1;
		if (arg.type == _VECT)
			s = arg._VECTptr->size();
		else
			arg = vecteur(1, arg);
		// s==1 -> general case
		if ((s == 1) || (s == 2)) { // int f(x) dx and sum f(n) n
			vecteur v(Equation_subsizes(gen(*arg._VECTptr, _SEQ__VECT), a, windowhsize, contextptr));
			eqwdata vv(Equation_total_size(v[0]));
			if (s == 1) {
				x = a.fontsize;
				Equation_translate(v[0], x, 0);
				x += int(fl_width(" dx"));
			}
			if (s == 2) {
				if (u == at_integrate) {
					x = a.fontsize;
					Equation_translate(v[0], x, 0);
					x += vv.dx + int(fl_width(" d"));
					Equation_vertical_adjust(vv.dy, vv.y, h, y);
					vv = Equation_total_size(v[1]);
					Equation_translate(v[1], x, 0);
					Equation_vertical_adjust(vv.dy, vv.y, h, y);
				}
				else {
					Equation_vertical_adjust(vv.dy, vv.y, h, y);
					eqwdata v1 = Equation_total_size(v[1]);
					x = max<int>(a.fontsize, v1.dx) + a.fontsize / 3; // var name size
					Equation_translate(v[1], 0, -v1.dy - v1.y);
					Equation_vertical_adjust(v1.dy, -v1.dy, h, y);
					Equation_translate(v[0], x, 0);
					x += vv.dx; // add function size
				}
			}
			if (u == at_integrate) {
				x += vv.dx;
				if (h == a.fontsize)
					h += 2 * a.fontsize / 3;
				if (y == 0) {
					y = -2 * a.fontsize / 3;
					h += 2 * a.fontsize / 3;
				}
			}
			v.push_back(eqwdata(x, h, 0, y, a, u, 0));
			return gen(v, _SEQ__VECT);
		}
		if (s >= 3) { // int _a^b f(x) dx
			vecteur & intarg = *arg._VECTptr;
			gen tmp_l, tmp_u, tmp_f, tmp_x;
			attributs aa(a);
			if (a.fontsize >= 10)
				aa.fontsize -= 2;
			tmp_f = Equation_compute_size(intarg[0], a, windowhsize, contextptr);
			tmp_x = Equation_compute_size(intarg[1], a, windowhsize, contextptr);
			tmp_l = Equation_compute_size(intarg[2], aa, windowhsize, contextptr);
			if (s == 4)
				tmp_u = Equation_compute_size(intarg[3], aa, windowhsize, contextptr);
			x = a.fontsize;
			eqwdata vv(Equation_total_size(tmp_l));
			Equation_translate(tmp_l, x - 2, -vv.y - vv.dy);
			vv = Equation_total_size(tmp_l);
			Equation_vertical_adjust(vv.dy, vv.y, h, y);
			int lx = vv.dx;
			if (s == 4) {
				vv = Equation_total_size(tmp_u);
				Equation_translate(tmp_u, x, a.fontsize - 3 - vv.y);
				vv = Equation_total_size(tmp_u);
				Equation_vertical_adjust(vv.dy, vv.y, h, y);
			}
			x += max(lx, vv.dx);
			Equation_translate(tmp_f, x, 0);
			vv = Equation_total_size(tmp_f);
			Equation_vertical_adjust(vv.dy, vv.y, h, y);
			if (u == at_integrate) {
				x += vv.dx + int(fl_width(" d"));
				Equation_translate(tmp_x, x, 0);
				vv = Equation_total_size(tmp_x);
				Equation_vertical_adjust(vv.dy, vv.y, h, y);
				x += vv.dx;
			}
			else {
				x += vv.dx;
				Equation_vertical_adjust(vv.dy, vv.y, h, y);
				vv = Equation_total_size(tmp_x);
				x = max(x, vv.dx) + a.fontsize / 3;
				Equation_translate(tmp_x, 0, -vv.dy - vv.y);
				Equation_vertical_adjust(vv.dy, -vv.dy, h, y);
			}
			vecteur res(makevecteur(tmp_f, tmp_x, tmp_l));
			if (s == 4)
				res.push_back(tmp_u);
			res.push_back(eqwdata(x, h, 0, y, a, u, 0));
			return gen(res, _SEQ__VECT);
		}
	}
	if (u == at_limit && arg.type == _VECT) { // limit
		vecteur & limarg = *arg._VECTptr;
		int s = limarg.size();
		if (s >= 3) {
			gen tmp_l, tmp_f, tmp_x, tmp_dir;
			attributs aa(a);
			if (a.fontsize >= 10)
				aa.fontsize -= 2;
			tmp_f = Equation_compute_size(limarg[0], a, windowhsize, contextptr);
			tmp_x = Equation_compute_size(limarg[1], aa, windowhsize, contextptr);
			tmp_l = Equation_compute_size(limarg[2], aa, windowhsize, contextptr);
			if (s == 4)
				tmp_dir = Equation_compute_size(limarg[3], aa, windowhsize, contextptr);
			eqwdata vf(Equation_total_size(tmp_f));
			eqwdata vx(Equation_total_size(tmp_x));
			eqwdata vl(Equation_total_size(tmp_l));
			eqwdata vdir(Equation_total_size(tmp_dir));
			int sous = max(vx.dy, vl.dy);
			if (s == 4)
				Equation_translate(tmp_f, vx.dx + vl.dx + vdir.dx + a.fontsize + 4, 0);
			else
				Equation_translate(tmp_f, vx.dx + vl.dx + a.fontsize + 2, 0);
			Equation_translate(tmp_x, 0, -sous - vl.y);
			Equation_translate(tmp_l, vx.dx + a.fontsize + 2, -sous - vl.y);
			if (s == 4)
				Equation_translate(tmp_dir, vx.dx + vl.dx + a.fontsize + 4, -sous - vl.y);
			h = vf.dy;
			y = vf.y;
			vl = Equation_total_size(tmp_l);
			Equation_vertical_adjust(vl.dy, vl.y, h, y);
			vecteur res(makevecteur(tmp_f, tmp_x, tmp_l));
			if (s == 4) {
				res.push_back(tmp_dir);
				res.push_back(eqwdata(vf.dx + vx.dx + a.fontsize + 4 + vl.dx + vdir.dx, h, 0, y, a, u, 0));
			}
			else
				res.push_back(eqwdata(vf.dx + vx.dx + a.fontsize + 2 + vl.dx, h, 0, y, a, u, 0));
			return gen(res, _SEQ__VECT);
		}
	}
	if ((u == at_of || u == at_at) && arg.type == _VECT && arg._VECTptr->size() == 2) {
		// user function, function in 1st arg, arguments in 2nd arg
		gen varg1 = Equation_compute_size(arg._VECTptr->front(), a, windowhsize, contextptr);
		eqwdata vv = Equation_total_size(varg1);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		gen arg2 = arg._VECTptr->back();
		if (u == at_at && xcas_mode(contextptr) != 0) {
			if (arg2.type == _VECT)
				arg2 = gen(addvecteur(*arg2._VECTptr, vecteur(arg2._VECTptr->size(), plus_one)), _SEQ__VECT);
			else
				arg2 = arg2 + plus_one;
		}
		gen varg2 = Equation_compute_size(arg2, a, windowhsize, contextptr);
		Equation_translate(varg2, vv.dx + llp, 0);
		vv = Equation_total_size(varg2);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		vecteur res(makevecteur(varg1, varg2));
		res.push_back(eqwdata(vv.dx + vv.x + lrp, h, 0, y, a, u, 0));
		return gen(res, _SEQ__VECT);
	}
	if (u == at_pow) {
		// first arg not translated
		gen varg = Equation_compute_size(arg._VECTptr->front(), a, windowhsize, contextptr);
		eqwdata vv = Equation_total_size(varg);
		// 1/2 ->sqrt, otherwise as exponent
		if (arg._VECTptr->back() == plus_one_half) {
			Equation_translate(varg, a.fontsize, 0);
			vecteur res(1, varg);
			res.push_back(eqwdata(vv.dx + a.fontsize, vv.dy + 4, vv.x, vv.y, a, at_sqrt, 0));
			return gen(res, _SEQ__VECT);
		}
		if (vv.g.type == _FUNC || vv.g.is_symb_of_sommet(at_pow))
			x = llp;
		Equation_translate(varg, x, 0);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		vecteur res(1, varg);
		// 2nd arg translated
		if (vv.g.type == _FUNC || vv.g.is_symb_of_sommet(at_pow))
			x += vv.dx + lrp;
		else
			x += vv.dx + 1;
		int arg1dy = vv.dy, arg1y = vv.y;
		if (a.fontsize >= 10) {
			attributs aa(a);
			aa.fontsize -= 2;
			varg = Equation_compute_size(arg._VECTptr->back(), aa, windowhsize, contextptr);
		}
		else
			varg = Equation_compute_size(arg._VECTptr->back(), a, windowhsize, contextptr);
		vv = Equation_total_size(varg);
		Equation_translate(varg, x, arg1y + (3*arg1dy) / 4 - vv.y);
		res.push_back(varg);
		vv = Equation_total_size(varg);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		x += vv.dx;
		res.push_back(eqwdata(x, h, 0, y, a, u, 0));
		return gen(res, _SEQ__VECT);
	}
	if (u == at_factorial) {
		vecteur v;
		gen varg = Equation_compute_size(arg, a, windowhsize, contextptr);
		eqwdata vv = Equation_total_size(varg);
		bool paren = need_parenthesis(vv.g) || vv.g == at_prod || vv.g == at_division || vv.g == at_pow;
		if (paren)
			x += llp;
		Equation_translate(varg, x, 0);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		v.push_back(varg);
		x += vv.dx;
		if (paren)
			x += lrp;
		varg = eqwdata(x + 4, h, 0, y, a, u, 0);
		v.push_back(varg);
		return gen(v, _SEQ__VECT);
	}
	if (u == at_sto) { // A:=B, *it -> B
		gen varg = Equation_compute_size(arg._VECTptr->back(), a, windowhsize, contextptr);
		eqwdata vv = Equation_total_size(varg);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		Equation_translate(varg, x, 0);
		vecteur v(2);
		v[1] = varg;
		x += vv.dx;
		x += ls + 3;
		// first arg not translated
		varg = Equation_compute_size(arg._VECTptr->front(), a, windowhsize, contextptr);
		vv = Equation_total_size(varg);
		if (need_parenthesis(vv.g))
			x += llp;
		Equation_translate(varg, x, 0);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		v[0] = varg;
		x += vv.dx;
		if (need_parenthesis(vv.g))
			x += lrp;
		v.push_back(eqwdata(x, h, 0, y, a, u, 0));
		return gen(v, _SEQ__VECT);
	}
	if (u == at_program && arg._VECTptr->back().type != _VECT && !arg._VECTptr->back().is_symb_of_sommet(at_local)) {
		gen varg = Equation_compute_size(arg._VECTptr->front(), a, windowhsize, contextptr);
		eqwdata vv = Equation_total_size(varg);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		Equation_translate(varg, x, 0);
		vecteur v(2);
		v[0] = varg;
		x += vv.dx;
		x += int(fl_width("->")) + 3;
		varg = Equation_compute_size(arg._VECTptr->back(), a, windowhsize, contextptr);
		vv = Equation_total_size(varg);
		Equation_translate(varg, x, 0);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		v[1] = varg;
		x += vv.dx;
		v.push_back(eqwdata(x, h, 0, y, a, u, 0));
		return gen(v, _SEQ__VECT);
	}
	bool binaryop = (u.ptr()->printsommet == &printsommetasoperator) || equalposcomp(binary_op_tab(), u);
	if (u != at_sto && u.ptr()->printsommet != NULL && !binaryop) {
		gen tmp = string2gen(g.print(contextptr), false);
		return Equation_compute_size(symbolic(at_expr, gen(makevecteur(tmp, xcas_mode(contextptr)), _SEQ__VECT)), a, windowhsize, contextptr);
	}
	vecteur v;
	if (!binaryop || arg.type != _VECT)
		v = Equation_subsizes(arg, a, windowhsize, contextptr);
	else
		v = Equation_subsizes(gen(*arg._VECTptr, _SEQ__VECT), a, windowhsize, contextptr);
	iterateur it = v.begin(), itend = v.end();
	if (it == itend || (itend - it == 1)) {
		gen gtmp;
		if (it == itend)
			gtmp = Equation_compute_size(gen(vecteur(0), _SEQ__VECT), a, windowhsize, contextptr);
		else
			gtmp = *it;
		// unary op, shift arg position horizontally
		eqwdata vv = Equation_total_size(gtmp);
		bool paren = u != at_neg || (vv.g != at_prod && need_parenthesis(vv.g)) ;
		x = ls + (paren ? llp : 0);
		gen tmp = gtmp; Equation_translate(tmp, x, 0);
		x = x + vv.dx + (paren ? lrp : 0);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		return gen(makevecteur(tmp, eqwdata(x, h, 0, y, a, u, 0)), _EQW__VECT);
	}
	if (binaryop) { // op (default with par)
		int currenth = h, largeur = 0;
		iterateur itprec = v.begin();
		h = 0;
		if (u == at_plus) { // op without parenthesis
			llp = 0;
			lrp = 0;
		}
		for (;;) {
			eqwdata vv = Equation_total_size(*it);
			if (need_parenthesis(vv.g))
				x += llp;
			if (u == at_plus && it != v.begin() &&
				(
					(it->type == _VECT && it->_VECTptr->back().type == _EQW && it->_VECTptr->back()._EQWptr->g == at_neg)
					||
					(it->type == _EQW && (is_integer(it->_EQWptr->g) || it->_EQWptr->g.type == _DOUBLE_) && is_strictly_positive(-it->_EQWptr->g, contextptr))
				)
			   )
				x -= ls;
			if (x > windowhsize - vv.dx && x > windowhsize / 2 && (itend - it)*vv.dx > windowhsize / 2) {
				largeur = max(x, largeur);
				x = 0;
				if (need_parenthesis(vv.g))
					x += llp;
				h += currenth;
				Equation_translate(*it, x, 0);
				for (iterateur kt = v.begin();kt != itprec;++kt)
					Equation_translate(*kt, 0, currenth);
				if (y) {
					for (iterateur kt = itprec;kt != it;++kt)
						Equation_translate(*kt, 0, -y);
				}
				itprec = it;
				currenth = vv.dy;
				y = vv.y;
			}
			else {
				Equation_translate(*it, x, 0);
				vv = Equation_total_size(*it);
				Equation_vertical_adjust(vv.dy, vv.y, currenth, y);
			}
			x += vv.dx;
			if (need_parenthesis(vv.g))
				x += lrp;
			++it;
			if (it == itend) {
				for (iterateur kt = v.begin();kt != itprec;++kt)
					Equation_translate(*kt, 0, currenth + y);
				h += currenth;
				v.push_back(eqwdata(max(x, largeur), h, 0, y, a, u, 0));
				// cerr << v << endl;
				return gen(v, _SEQ__VECT);
			}
			x += ls + 3;
		}
	}
	// normal printing
	x = ls + llp;
	for (;;) {
		eqwdata vv = Equation_total_size(*it);
		Equation_translate(*it, x, 0);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		x += vv.dx;
		++it;
		if (it == itend) {
			x += lrp;
			v.push_back(eqwdata(x, h, 0, y, a, u, 0));
			return gen(v, _SEQ__VECT);
		}
		x += lc;
	}
}

// windowhsize is used for g of type HIST__VECT (history) right justify answers
// Returns either a eqwdata type object (terminal) or a vector
// (of subtype _EQW__VECT or _HIST__VECT)
gen Equation_compute_size(const gen & g, const attributs & a, int windowhsize, GIAC_CONTEXT) {
	fl_font(FL_HELVETICA, a.fontsize);
	/**************
	 * FL_WIDGET  *
	 **************/
	/*if (g.type == _POINTER_) {
		if (g.subtype == _FL_WIDGET_POINTER) {
			Fl_Widget * wg = (Fl_Widget *) g._POINTER_val;
			return eqwdata(wg->w(), wg->h(), 0, 0, a, g);
		}
#ifndef __APPLE__
		if (g.subtype == _FL_IMAGE_POINTER) {
			Fl_Image * ptr = (Fl_Image *) g._POINTER_val;
			return eqwdata(ptr->w(), ptr->h(), 0, 0, a, g);
		}
#endif
	}*/
	
	/***************
	 *   STRINGS   *
	 ***************/
	if (g.type == _STRNG) {
		string s;
		s = *g._STRNGptr;
		string cs;
		int ss = s.size();
		/* if (!ss)
		return eqwdata(10,6,0,0,a,g); */
		int hsize = 0, vsize = 0;
		bool newline = false;
		vecteur res;
		gen tmps;
		for (int pos = 0;pos < ss;++pos) {
			char ch = s[pos];
			if (ch == '\n') {
				newline = true;
				hsize = max(hsize, int(fl_width((' ' + cs).c_str())));
				tmps = string2gen(cs, false);
				vsize += a.fontsize;
				res.push_back(eqwdata(hsize, a.fontsize, 0, -vsize, a, tmps));
				cs = "";
			}
			else
				cs += ch;
		}
		hsize = max(hsize, int(fl_width((' ' + cs).c_str())));
		vsize += a.fontsize;
		tmps = string2gen(cs, false);
		if (!newline) {
			tmps.subtype = g.subtype;
			return eqwdata(hsize, a.fontsize, 0, 0, a, tmps);
		}
		gen tmp = eqwdata(hsize, a.fontsize, 0, -vsize, a, tmps);
		res.push_back(tmp);
		res.push_back(eqwdata(hsize, vsize, 0, -vsize, a, at_multistring));
		tmp = gen(res, _EQW__VECT);
		Equation_translate(tmp, 0, vsize);
		return tmp;
	}
	/* SERIES */
	/*if (g.type == _SPOL1) {
		int sf = series_flags(contextptr);
		if (sf & (1 << 5) && !(sf & (1 << 4))) {
			identificateur tt(string(1, series_variable_name(contextptr)));
			gen remains, gg = sparse_poly12gen(*g._SPOL1ptr, tt, remains, !(sf & (1 << 6)));
			if ((sf & (1 << 6)) && !is_zero(remains))
				gg += symb_of(gen("O", contextptr), remains);
			return Equation_compute_size(gg, a, windowhsize, contextptr);
		}
	}*/
	/*****************
	 *   FRACTIONS   *
	 *****************/
	if (g.type == _FRAC) {
		if (is_integer(g._FRACptr->num) && is_positive(-g._FRACptr->num, contextptr))
			return Equation_compute_size(symbolic(at_neg, fraction(-g._FRACptr->num, g._FRACptr->den)), a, windowhsize, contextptr);
		gen v1 = Equation_compute_size(g._FRACptr->num, a, windowhsize, contextptr);
		eqwdata vv1 = Equation_total_size(v1);
		gen v2 = Equation_compute_size(g._FRACptr->den, a, windowhsize, contextptr);
		eqwdata vv2 = Equation_total_size(v2);
		// Center the fraction
		int w1 = vv1.dx, w2 = vv2.dx;
		int w = max(w1, w2) + 6;
		vecteur v(3);
		v[0] = v1; Equation_translate(v[0], (w - w1) / 2, 11 - vv1.y);
		v[1] = v2; Equation_translate(v[1], (w - w2) / 2, 7 - vv2.dy - vv2.y);
		v[2] = eqwdata(w, 4 + vv1.dy + vv2.dy, 0, 7 - vv2.dy, a, at_division, 0);
		return gen(v, _SEQ__VECT);
	}
	/***************
	 *   VECTORS   *
	 ***************/
	if ((g.type == _VECT) && !g._VECTptr->empty()) {
		if (g.subtype == _SPREAD__VECT)
			return Equation_compute_size(string2gen("spreadsheet", false), a, windowhsize, contextptr);
		vecteur v;
		const_iterateur it = g._VECTptr->begin(), itend = g._VECTptr->end();
		int x = 0, y = 0, h = a.fontsize;
		/***************
		 *   HISTORY   *
		 ***************/
#if 0
		if (g.subtype == _HIST__VECT) {
			int l = windowhsize;
			vecteur vplot;
			for (int i = 0;it != itend;++it, ++i) {
				gen tmpg(*it);
				if (!rpn_mode(contextptr) && it->type == _VECT && !it->_VECTptr->empty()) {
					if (it->_VECTptr->front().type == _STRNG)
						tmpg = makevecteur(it->_VECTptr->front(), string2gen("", false));
					gen tmpback = it->_VECTptr->back();
					if (tmpback.type == _POINTER_ && tmpback.subtype == _FL_WIDGET_POINTER && fl_widget_updatepict_function)
						tmpback = fl_widget_updatepict_function(tmpback);
					if (tmpback.is_symb_of_sommet(at_pnt) || (tmpback.type == _VECT && !tmpback._VECTptr->empty() && tmpback._VECTptr->back().is_symb_of_sommet(at_pnt)))
						vplot.push_back(tmpback);
					if (tmpback.is_symb_of_sommet(at_erase))
						vplot.clear();
					gen itfront = it->_VECTptr->front();
					if (itfront.is_symb_of_sommet(at_expr)) {
						itfront = itfront._SYMBptr->feuille;
						int mode = xcas_mode(contextptr);
						if (itfront.type == _VECT && !itfront._VECTptr->empty()) {
							mode = itfront._VECTptr->back().val;
							itfront = itfront._VECTptr->front();
						}
						if (itfront.type == _STRNG) {
							int save_maple_mode = xcas_mode(contextptr);
							xcas_mode(contextptr) = mode;
							try {
								itfront = gen(*itfront._STRNGptr, contextptr);
							}
							catch (std::runtime_error & e) {
							}
							xcas_mode(contextptr) = save_maple_mode;
						}
					}
				}
				tmpg.subtype = _SEQ__VECT;
				vecteur tmp(Equation_subsizes(tmpg, a, windowhsize, contextptr));
				iterateur jt = tmp.begin(); // this is the question
				// compute the size of writing the history level i
				eqwdata w(Equation_total_size(*jt));
				if (rpn_mode(contextptr)) // ignore question
					v.push_back(eqwdata(1, 1, x, -y, w.eqw_attributs, string2gen("", false)));
				else {
					y += w.dy + 2;
					x = int(fl_width((print_INT_(i) + ": ").c_str()));
					if (l < w.dx + x)
						l = w.dx + x;
					Equation_translate(*jt, x, -y - w.y); v.push_back(*jt);
				}
				jt = tmp.end() - 1; // this is the answer
				*jt = Equation_change_attributs(*jt, attributs(a.fontsize, a.background, a.text_color + 4));
				w = Equation_total_size(*jt);
				y += w.dy + 2;
				x = int(fl_width("    "));
				l = max(l, w.dx + x);
				int xshift = x;
				if (w.dx + 4 < windowhsize - a.fontsize) {
					if (center_history)
						xshift = (windowhsize - w.dx - 4) / 2;
					else
						xshift = windowhsize - w.dx - 4 - a.fontsize;
				}
				Equation_translate(*jt, xshift, -y - w.y); v.push_back(*jt);
			}
			v.push_back(eqwdata(l, y, 0, -y, a, at_makevector, 0));
			// cerr << v << endl;
			gen res = gen(v, _HIST__VECT); Equation_translate(res, 0, y); return res;
		} // END HISTORY
#else
		if (g.subtype == _HIST__VECT) {
			vecteur v;
			v.reserve(g._VECTptr->size());
			// vertical gluing
			int W = 0, H = 0;
			for (int i = 0;it != itend;++it, ++i) {
				gen g = *it;
				if (g.type == _VECT && g.subtype == _SEQ__VECT)
					g.subtype = _PRINT__VECT;
				gen cur_size = Equation_compute_size(g, a, windowhsize, contextptr);
				eqwdata tmp = Equation_total_size(cur_size);
#if 0
				int Y, dY;
				Equation_y_dy(cur_size, Y, dY);
				Equation_translate(cur_size, 0, Y - y - H);
				H += dY + 2;
#else
		if (ckmatrix(*it))
			Equation_translate(cur_size, 0, tmp.y - y - H + a.fontsize / 2);
		else
			Equation_translate(cur_size, 0, tmp.y - y - H - a.fontsize / 2);
		H += tmp.dy + 2;
#endif
				v.push_back(cur_size);
				W = giacmax(W, tmp.dx);
			}
			gen mkvect(at_makevector);
			mkvect.subtype = _PRINT__VECT;
			v.push_back(eqwdata(W, H, 0, -y - H, a, mkvect, 0));
			return gen(v, _EQW__VECT);
		} // END HISTORY
#endif
		/***************
		 *   MATRICE   *
		 ***************/
		bool gmat = ckmatrix(g);
		vector<int> V; int p = 0;
		if (!gmat && is_mod_vecteur(*g._VECTptr, V, p) && p != 0) {
			gen gm = makemodquoted(unmod(g), p);
			return Equation_compute_size(gm, a, windowhsize, contextptr);
		}
		vector< vector<int> > M;
		if (gmat && is_mod_matrice(*g._VECTptr, M, p) && p != 0) {
			gen gm = makemodquoted(unmod(g), p);
			return Equation_compute_size(gm, a, windowhsize, contextptr);
		}
		if (gmat && g.subtype != _SEQ__VECT && g.subtype != _SET__VECT && g.subtype != _POLY1__VECT && g._VECTptr->front().subtype != _SEQ__VECT) {
			gen mkvect(at_makevector);
			mkvect.subtype = _SEQ__VECT;
			gen mkmat(at_makevector);
			mkmat.subtype = _MATRIX__VECT;
			int nrows, ncols;
			mdims(*g._VECTptr, nrows, ncols);
			if (ncols) {
				vecteur all_sizes;
				all_sizes.reserve(nrows);
				vector<int> row_heights(nrows), row_bases(nrows), col_widths(ncols);
				// vertical gluing
				for (int i = 0;it != itend;++it, ++i) {
					gen tmpg = *it;
					tmpg.subtype = _SEQ__VECT;
					vecteur tmp(Equation_subsizes(tmpg, a, max(windowhsize / ncols - a.fontsize, 230), contextptr));
					int h = a.fontsize, y = 0;
					const_iterateur jt = tmp.begin(), jtend = tmp.end();
					for (int j = 0;jt != jtend;++jt, ++j) {
						eqwdata w(Equation_total_size(*jt));
						Equation_vertical_adjust(w.dy, w.y, h, y);
						col_widths[j] = max(col_widths[j], w.dx);
					}
					if (i)
						row_heights[i] = row_heights[i-1] + h + a.fontsize;
					else
						row_heights[i] = h;
					row_bases[i] = y;
					all_sizes.push_back(tmp);
				}
				// accumulate col widths
				col_widths.front() += (3 * a.fontsize) / 2;
				vector<int>::iterator iit = col_widths.begin() + 1, iitend = col_widths.end();
				for (;iit != iitend;++iit)
					*iit += *(iit - 1) + a.fontsize;
				// translate each cell
				it = all_sizes.begin();
				itend = all_sizes.end();
				int h, y, prev_h = 0;
				for (int i = 0;it != itend;++it, ++i) {
					h = row_heights[i];
					y = row_bases[i];
					iterateur jt = it->_VECTptr->begin(), jtend = it->_VECTptr->end();
					for (int j = 0;jt != jtend;++jt, ++j) {
						eqwdata w(Equation_total_size(*jt));
						if (j)
							Equation_translate(*jt, col_widths[j-1] - w.x, -h - y);
						else
							Equation_translate(*jt, -w.x + a.fontsize / 2, -h - y);
					}
					it->_VECTptr->push_back(eqwdata(col_widths.back(), h - prev_h, 0, -h, a, mkvect, 0));
					prev_h = h;
				}
				all_sizes.push_back(eqwdata(col_widths.back(), row_heights.back(), 0, -row_heights.back(), a, mkmat, -row_heights.back() / 2));
				gen all_sizesg = all_sizes; Equation_translate(all_sizesg, 0, row_heights.back() / 2); return all_sizesg;
			}
		} // end matrices
		/*************************
		 *   SEQUENCES/VECTORS   *
		 *************************/
		// horizontal gluing
		if (g.subtype != _PRINT__VECT) x += a.fontsize / 2;
		int ncols = itend - it;
		//ncols=min(ncols,5);
		for (;it != itend;++it) {
			gen cur_size = Equation_compute_size(*it, a,
						   max(windowhsize / ncols - a.fontsize,
#ifdef IPAQ
							   200
#else
							   480
#endif
							  ), contextptr);
			eqwdata tmp = Equation_total_size(cur_size);
			Equation_translate(cur_size, x - tmp.x, 0); v.push_back(cur_size);
			x = x + tmp.dx + ((g.subtype == _PRINT__VECT) ? 2 : a.fontsize);
			Equation_vertical_adjust(tmp.dy, tmp.y, h, y);
		}
		gen mkvect(at_makevector);
		if (g.subtype == _SEQ__VECT)
			mkvect = at_makesuite;
		else
			mkvect.subtype = g.subtype;
		v.push_back(eqwdata(x, h, 0, y, a, mkvect, 0));
		return gen(v, _EQW__VECT);
	} // end sequences
	if (g.type == _MOD) {
		int x = 0;
		int h = a.fontsize;
		int y = 0;
		int modsize = int(fl_width("%")) + 4;
		bool paren = is_positive(-*g._MODptr, contextptr);
		int llp = int(fl_width("("));
		int lrp = int(fl_width(")"));
		gen varg1 = Equation_compute_size(*g._MODptr, a, windowhsize, contextptr);
		if (paren) Equation_translate(varg1, llp, 0);
		eqwdata vv = Equation_total_size(varg1);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		gen arg2 = *(g._MODptr + 1);
		gen varg2 = Equation_compute_size(arg2, a, windowhsize, contextptr);
		if (paren)
			Equation_translate(varg2, vv.dx + modsize + lrp, 0);
		else
			Equation_translate(varg2, vv.dx + modsize, 0);
		vv = Equation_total_size(varg2);
		Equation_vertical_adjust(vv.dy, vv.y, h, y);
		vecteur res(makevecteur(varg1, varg2));
		res.push_back(eqwdata(vv.dx + vv.x, h, 0, y, a, at_normalmod, 0));
		return gen(res, _SEQ__VECT);
	}
	if (g.type == _USER) {
		if (dynamic_cast<giac::galois_field *>(g._USERptr)) {
			gen g1(g.print(contextptr), contextptr);
			return Equation_compute_size(g1, a, windowhsize, contextptr);
		}
	}
	if (g.type != _SYMB) {
		string s = g.print(contextptr);
		if (s.size() > 2000)
			s = s.substr(0, 2000) + "...";
		fl_font(cst_greek_translate(s), a.fontsize);
		int i = int(fl_width(s.c_str()));
		gen tmp = eqwdata(i, a.fontsize, 0, 0, a, g);
		return tmp;
	}
	/**********************
	 *  SYMBOLIC HANDLING *
	 **********************/
	return Equation_compute_symb_size(g, a, windowhsize, contextptr);
	// return Equation_compute_symb_size(aplatir_fois_plus(g),a,windowhsize,contextptr);
	// aplatir_fois_plus is a problem for Equation_replace_selection
	// because it will modify the structure of the data
}

string Equation_extract_string(const string & cs, int begin_sel, int end_sel) {
	int css = cs.size();
	if (!css)
		return cs;
	int sel0 = min(max(0, begin_sel), css - 1), sel1 = css;
	if (end_sel >= 0)
		sel1 = min(end_sel, css);
	if (sel0 > sel1)
		giac::swapint(sel0, sel1);
	return cs.substr(sel0, sel1 - sel0);
}

void Equation_draw(const eqwdata & e, int x, int y, int rightx, int lowery, Equation * eq, int begin_sel, int end_sel) {
	//if ((e.dx + e.x < x) || (e.x > rightx) || (e.y > y) || e.y + e.dy < lowery)
	//	return; // nothing to draw, out of window
	const giac::context * contextptr = get_context(eq);
	gen gg = e.g;
	bool selected = e.selected && Fl::focus() == eq;
	int fontsize = e.eqw_attributs.fontsize;
	Fl_Color text_color = Fl_Color(e.eqw_attributs.text_color);
	Fl_Color background = Fl_Color(e.eqw_attributs.background);
	fl_font(FL_HELVETICA, fontsize);
	fl_color(selected ? background : text_color);
	if (gg.type == _POINTER_) {
		// wg->resize(e.x-x,y-e.y-e.dy,e.dx,e.dy);
		// wg->draw(); // automatically done if it belongs to the group
		return;
	}
	if (gg.type == _STRNG) {
		string s;
		if (e.active) {
			// draw s and the cursor
			s = *gg._STRNGptr;
			int ss = s.size();
			int pos = max(min(eq->active_pos, ss), 0);
			if (eq->need_active_parse)
				s = s.substr(0, pos) + "|" + s.substr(pos, ss - pos);
			else
				s = '"' + s.substr(0, pos) + "|" + s.substr(pos, ss - pos);
		}
		else {
			if (gg.subtype)
				s = '"' + *gg._STRNGptr;
			else
				s = ' ' + *gg._STRNGptr;
		}
		string cs;
		int ss = s.size();
		int vsize = fontsize - 2, pos;
		for (pos = 0;pos < ss;++pos) {
			char ch = s[pos];
			if (ch == '\n') {
				check_fl_draw(cs.c_str(), eq->x() + e.x - x, eq->y() + y - e.y + vsize - e.dy, eq->clip_x, eq->clip_y, eq->clip_w, eq->clip_h, 0, 0);
				cs = "";
				vsize += fontsize;
			}
			else
				cs += ch;
		}
		check_fl_draw(cs.c_str(), eq->x() + e.x - x, eq->y() + y - e.y + vsize - e.dy, eq->clip_x, eq->clip_y, eq->clip_w, eq->clip_h, 0, 0);
		// If selected take care of begin/end selection
		int css = cs.size(); // must be >=1 since selected hence not active
		if (selected && css) {
			int sel0 = min(begin_sel + 1, css - 1), sel1 = css;
			if (end_sel >= 0)
				sel1 = min(end_sel + 1, css);
			if (sel0 > sel1)
				giac::swapint(sel0, sel1);
			int deltax = int(fl_width(cs.substr(0, sel0).c_str()));
			cs = cs.substr(sel0, sel1 - sel0);
			int dx = int(fl_width(cs.c_str()));
			fl_color(text_color);
			check_fl_rectf(eq->x() + e.x - x + deltax, eq->y() + y - e.y - e.dy + 1, dx, e.dy + 3, eq->clip_x, eq->clip_y, eq->clip_w, eq->clip_h, 0, 0);
			fl_color(background);
			check_fl_draw(cs.c_str(), eq->x() + e.x - x + deltax, eq->y() + y - e.y + vsize - e.dy, eq->clip_x, eq->clip_y, eq->clip_w, eq->clip_h, 0, 0);
		}
		return;
	}
	if (selected) {
		fl_color(text_color);
		check_fl_rectf(eq->x() + e.x - x, eq->y() + y - e.y - e.dy + 1, e.dx, e.dy + 3, eq->clip_x, eq->clip_y, eq->clip_w, eq->clip_h, 0, 0);
		fl_color(background);
	}
	string s = gg.print(contextptr);
	if (gg.type == _IDNT && !s.empty() && s[0] == '_')
		s = s.substr(1, s.size() - 1);
	if (s.size() > 2000)
		s = s.substr(0, 2000) + "...";
	Fl_Font font = cst_greek_translate(s);
	if (gg.type == _IDNT && font == FL_HELVETICA
#if !defined(WIN32) && !defined(__APPLE__)
		&& gg != cst_pi
#endif
	   )
		font = FL_TIMES_BOLD_ITALIC; // FL_HELVETICA_BOLD_ITALIC;
	fl_font(font, fontsize);
	// cerr << s.size() << endl;
	check_fl_draw(s.c_str(), eq->x() + e.x - x, eq->y() + y - e.y, eq->clip_x, eq->clip_y, eq->clip_w, eq->clip_h, 0, 0);
	return;
}

void Equation_draw(const gen & g, int x, int y, int rightx, int lowery, Equation * equat) {
	const giac::context * contextptr = get_context(equat);
	int eqx = equat->x(), eqy = equat->y();
	if (g.type == _EQW) { // terminal
		eqwdata & e = *g._EQWptr;
		Equation_draw(e, x, y, rightx, lowery, equat, equat->begin_sel, equat->end_sel);
	}
	if (g.type != _VECT)
		return;
	vecteur & v = *g._VECTptr;
	if (v.empty())
		return;
	gen tmp = v.back();
	if (tmp.type != _EQW) {
		cerr << "EQW error:" << v << endl;
		return;
	}
	eqwdata & w = *tmp._EQWptr;
	
	//if ((w.dx + w.x - x < 0) || (w.x > rightx) || (w.y > y) || (w.y + w.dy < lowery))
	//	return; // nothing to draw, out of window
	
	/*******************
	 * draw the vector *
	 *******************/
	// v is the vector, w the master operator eqwdata
	gen oper = w.g;
	bool selected = w.selected && Fl::focus() == equat;
	int fontsize = 15;//zero crash with w.eqw_attributs.fontsize;
	int background = w.eqw_attributs.background;
	int text_color = w.eqw_attributs.text_color;
	int x0 = w.x;
	int y0 = w.y; // lower coordinate of the master vector
	int y1 = y0 + w.dy; // upper coordinate of the master vector
	if (selected) {
		fl_color(text_color);
		check_fl_rectf(eqx + w.x - x, eqy + y - w.y - w.dy + 1, w.dx, w.dy + 3, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
	}
	else {
		fl_color(background);
		// fl_rectf(w.x-x,y-w.y-w.dy,w.dx,w.dy-2);
	}
	// draw arguments of v
	const_iterateur it = v.begin(), itend = v.end() - 1;
	if (oper == at_multistring) {
		const_iterateur it_beg = it, it_end = itend;
		if (Equation_multistring_selection(it_beg, it_end, false)) {
			// begin_sel and end_sel apply respect. to it_beg and it_end sel. lines
			for (;it != it_beg;++it)
				Equation_draw(*it->_EQWptr, x, y, rightx, lowery, equat, -1, -1);
			if (it_beg == it_end) {
				Equation_draw(*it->_EQWptr, x, y, rightx, lowery, equat, equat->begin_sel, equat->end_sel);
				++it;
			}
			else {
				Equation_draw(*it->_EQWptr, x, y, rightx, lowery, equat, equat->begin_sel, -1);
				++it;
				for (;it != it_end;++it)
					Equation_draw(*it->_EQWptr, x, y, rightx, lowery, equat, -1, -1);
				Equation_draw(*it->_EQWptr, x, y, rightx, lowery, equat, -1, equat->end_sel);
				++it;
			}
			for (;it != itend;++it)
				Equation_draw(*it->_EQWptr, x, y, rightx, lowery, equat, -1, -1);
			return;
		}
	}
	if (oper == at_expr && v.size() == 3) {
		Equation_draw(*it, x, y, rightx, lowery, equat);
		return;
	}
	for (;it != itend;++it)
		Equation_draw(*it, x, y, rightx, lowery, equat);
	if (oper == at_multistring)
		return;
	fl_font(FL_HELVETICA, fontsize);
	fl_color(selected ? background : text_color);
	string s;
	if (g.subtype == _HIST__VECT) { // For history, we must write history levels
		it = v.begin();
		int nlevels = (itend - it) / 2 - 1, wlevel;
		int skip = 2; // int skip=2-rpn_mode(contextptr);
		for (int i = 0;it != itend;it += skip, ++i) {
			eqwdata tmp = Equation_total_size(*it);
			fl_font(FL_HELVETICA, tmp.eqw_attributs.fontsize);
			fl_color(FL_BLUE);
			// cerr << tmp << endl;
			int yy;
			// uncommented, seemed previously to be problematic with strings
			if (tmp.hasbaseline)
				yy = y - tmp.baseline;
			else
				yy = y - tmp.y - (tmp.dy - tmp.eqw_attributs.fontsize) / 2;
			if (yy < 0 || yy > y - lowery)
				continue;
			if (rpn_mode(contextptr))
				wlevel = nlevels - i;
			else
				wlevel = i;
			if (wlevel || !rpn_mode(contextptr))
				check_fl_draw((print_INT_(wlevel) + ": ").c_str(), eqx - x, eqy + yy, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
		}
		return; // nothing else to do
	}
	if (oper.type == _FUNC) {
		// catch here special cases user function, vect/matr, ^, int, sqrt, etc.
		unary_function_ptr & u = *oper._FUNCptr;
		if (u == at_at) { // draw brackets around 2nd arg
			gen arg2 = v[1]; // 2nd arg of at_of, i.e. what's inside the parenth.
			eqwdata varg2 = Equation_total_size(arg2);
			x0 = varg2.x;
			y0 = varg2.y;
			y1 = y0 + varg2.dy;
			fontsize = varg2.eqw_attributs.fontsize;
			fl_font(FL_HELVETICA, fontsize);
			if (x0 < rightx)
				check_fl_draw("[", eqx + x0 - x - int(fl_width("[")), eqy + y - varg2.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			x0 += varg2.dx ;
			if (x0 < rightx)
				check_fl_draw("]", eqx + x0 - x, eqy + y - varg2.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			return;
		}
		if (u == at_of) { // do we need to draw some parenthesis?
			gen arg2 = v[1]; // 2nd arg of at_of, i.e. what's inside the parenth.
			if (arg2.type != _VECT || arg2._VECTptr->back().type != _EQW || arg2._VECTptr->back()._EQWptr->g != at_makesuite) { // Yes (if not _EQW it's a sequence with parent)
				eqwdata varg2 = Equation_total_size(arg2);
				x0 = varg2.x;
				y0 = varg2.y;
				y1 = y0 + varg2.dy;
				fontsize = varg2.eqw_attributs.fontsize;
				int pfontsize = max(fontsize, (fontsize + (varg2.baseline - varg2.y)) / 2);
				fl_font(FL_HELVETICA, pfontsize); // was fontsize
				if (x0 < rightx)
					check_fl_draw("(", eqx + x0 - x - int(fl_width("(")), eqy + y - varg2.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				x0 += varg2.dx ;
				if (x0 < rightx)
					check_fl_draw(")", eqx + x0 - x, eqy + y - varg2.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			}
			return;
		}
		if (u == at_makesuite) {
			bool paren = v.size() != 2; // Sequences with 1 arg don't show parenthesis
			int pfontsize = max(fontsize, (fontsize + (w.baseline - w.y)) / 2);
			fl_font(FL_HELVETICA, pfontsize);
			if (paren && x0 < rightx)
				check_fl_draw("(", eqx + x0 - x - int(fl_width("(")) / 2, eqy + y - w.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			x0 += w.dx;
			if (paren && x0 < rightx)
				check_fl_draw(")", eqx + x0 - x - int(fl_width("(")) / 2, eqy + y - w.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			// print commas between args
			it = v.begin(), itend = v.end() - 2;
			for (;it != itend;++it) {
				eqwdata varg2 = Equation_total_size(*it);
				fontsize = varg2.eqw_attributs.fontsize;
				fl_font(FL_HELVETICA, fontsize);
				if (varg2.x + varg2.dx < rightx)
					check_fl_draw(",", eqx + varg2.x + varg2.dx - x + 1, eqy + y - varg2.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			}
			return;
		}
		if (u == at_makevector) { // draw [] delimiters for vector/matrices
			if (oper.subtype != _SEQ__VECT && oper.subtype != _PRINT__VECT) {
				int decal = 1;
				switch (oper.subtype) {
				case _MATRIX__VECT: decal = 2; break;
				case _SET__VECT: decal = 4; break;
				case _POLY1__VECT: decal = 6; break;
				}
				if (x0 + 1 < rightx) {
					check_fl_line(eqx + x0 - x + 1, eqy + y - y0 + 1, eqx + x0 - x + 1, eqy + y - y1 + 1, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
					check_fl_line(eqx + x0 - x + decal, eqy + y - y0 + 1, eqx + x0 - x + decal, eqy + y - y1 + 1, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
					check_fl_line(eqx + x0 - x + 1, eqy + y - y0 + 1, eqx + x0 - x + fontsize / 4, eqy + y - y0 + 1, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
					check_fl_line(eqx + x0 - x + 1, eqy + y - y1 + 1, eqx + x0 - x + fontsize / 4, eqy + y - y1 + 1, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				}
				x0 += w.dx ;
				if (x0 - 1 < rightx) {
					check_fl_line(eqx + x0 - x - 1, eqy + y - y0 + 1, eqx + x0 - x - 1, eqy + y - y1 + 1, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
					check_fl_line(eqx + x0 - x - decal, eqy + y - y0 + 1, eqx + x0 - x - decal, eqy + y - y1 + 1, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
					check_fl_line(eqx + x0 - x - 1, eqy + y - y0 + 1, eqx + x0 - x - fontsize / 4, eqy + y - y0 + 1, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
					check_fl_line(eqx + x0 - x - 1, eqy + y - y1 + 1, eqx + x0 - x - fontsize / 4, eqy + y - y1 + 1, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				}
			} // end if oper.subtype!=SEQ__VECT
			if (oper.subtype != _MATRIX__VECT && oper.subtype != _PRINT__VECT) {
				// print commas between args
				it = v.begin(), itend = v.end() - 2;
				for (;it != itend;++it) {
					eqwdata varg2 = Equation_total_size(*it);
					fontsize = varg2.eqw_attributs.fontsize;
					fl_font(FL_HELVETICA, fontsize);
					if (varg2.x + varg2.dx < rightx)
						check_fl_draw(",", eqx + varg2.x + varg2.dx - x + 1, eqy + y - varg2.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				}
			}
			return;
		}
		int lpsize = int(fl_width("("));
		int rpsize = int(fl_width(")"));
		eqwdata tmp = Equation_total_size(v.front()); // tmp= 1st arg eqwdata
		if (u == at_sto)
			tmp = Equation_total_size(v[1]);
		x0 = w.x - x;
		y0 = y - w.baseline;
		if (u == at_pow) {
			if (!need_parenthesis(tmp.g) && tmp.g != at_pow && tmp.g != at_prod && tmp.g != at_division)
				return;
			if (tmp.g == at_pow) {
				fontsize = tmp.eqw_attributs.fontsize + 2;
				fl_font(FL_HELVETICA, fontsize);
			}
			if (tmp.x - lpsize < rightx)
				check_fl_draw("(", eqx + tmp.x - x - lpsize, eqy + y - tmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			if (tmp.x + tmp.dx < rightx)
				check_fl_draw(")", eqx + tmp.x + tmp.dx - x, eqy + y - tmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			return;
		}
		if (u == at_program) {
			if (tmp.x + tmp.dx < rightx)
				check_fl_draw("->", eqx + tmp.x + tmp.dx - x, eqy + y - tmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			return;
		}
		if (u == at_sum) {
			if (x0 < rightx) {
				check_fl_line(eqx + x0, eqy + y0, eqx + x0 + (2*fontsize) / 3, eqy + y0, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				check_fl_line(eqx + x0, eqy + y0 - fontsize, eqx + x0 + (2*fontsize) / 3, eqy + y0 - fontsize, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				check_fl_line(eqx + x0, eqy + y0, eqx + x0 + fontsize / 2, eqy + y0 - fontsize / 2, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				check_fl_line(eqx + x0 + fontsize / 2, eqy + y0 - fontsize / 2, eqx + x0, eqy + y0 - fontsize, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				if (v.size() > 2) { // draw the =
					eqwdata ptmp = Equation_total_size(v[1]);
					if (ptmp.x + ptmp.dx < rightx)
						check_fl_draw("=", eqx + ptmp.x + ptmp.dx - x - 2, eqy + y - ptmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				}
			}
			return;
		}
		if (u == at_abs) {
			y0 = 1 + y - w.y;
			int h = w.dy;
			if (x0 < rightx) {
				check_fl_line(eqx + x0 + 2, eqy + y0 - 1, eqx + x0 + 2, eqy + y0 - h + 3, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				check_fl_line(eqx + x0 + 1, eqy + y0 - 1, eqx + x0 + 1, eqy + y0 - h + 3, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				check_fl_line(eqx + x0 + w.dx - 1, eqy + y0 - 1, eqx + x0 + w.dx - 1, eqy + y0 - h + 3, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				check_fl_line(eqx + x0 + w.dx, eqy + y0 - 1, eqx + x0 + w.dx, eqy + y0 - h + 3, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			}
			return;
		}
		if (u == at_sqrt) {
			y0 = 1 + y - w.y;
			int h = w.dy;
			if (x0 < rightx) {
				check_fl_line(eqx + x0 + 2, eqy + y0 - h / 2, eqx + x0 + fontsize / 2, eqy + y0 - 1, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				check_fl_line(eqx + x0 + fontsize / 2, eqy + y0 - 1, eqx + x0 + fontsize, eqy + y0 - h + 3, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				check_fl_line(eqx + x0 + fontsize, eqy + y0 - h + 3, eqx + x0 + w.dx - 1, eqy + y0 - h + 3, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				++y0;
				check_fl_line(eqx + x0 + 2, eqy + y0 - h / 2, eqx + x0 + fontsize / 2, eqy + y0 - 1, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				check_fl_line(eqx + x0 + fontsize / 2, eqy + y0 - 1, eqx + x0 + fontsize, eqy + y0 - h + 3, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				check_fl_line(eqx + x0 + fontsize, eqy + y0 - h + 3, eqx + x0 + w.dx - 1, eqy + y0 - h + 3, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			}
			return;
		}
		if (u == at_factorial) {
			check_fl_draw("!", eqx + w.x + w.dx - 4 - x, eqy + y - w.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			if (!need_parenthesis(tmp.g)
				&& tmp.g != at_pow && tmp.g != at_prod && tmp.g != at_division
			   )
				return;
			if (tmp.x -lpsize < rightx)
				check_fl_draw("(", eqx + tmp.x - x - lpsize, eqy + y - tmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			if (tmp.x + tmp.dx < rightx)
				check_fl_draw(")", eqx + tmp.x + tmp.dx - x, eqy + y - tmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			return;
		}
		if (u == at_integrate) {
			x0 += 2;
			y0 += fontsize / 2;
			if (x0 < rightx) {
				fl_arc(eqx + x0, eqy + y0, fontsize / 3, fontsize / 3, 180, 360);
				check_fl_line(eqx + x0 + fontsize / 3, eqy + y0, eqx + x0 + fontsize / 3, eqy + y0 - 2*fontsize + 4, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				fl_arc(eqx + x0 + fontsize / 3, eqy + y0 - 2*fontsize + 3, fontsize / 3, fontsize / 3, 0, 180);
			}
			if (v.size() != 2) { // if arg has size > 1 draw the d
				eqwdata ptmp = Equation_total_size(v[1]);
				if (ptmp.x < rightx)
					check_fl_draw(" d", eqx + ptmp.x - x - int(fl_width(" d")), eqy + y - ptmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			}
			else {
				eqwdata ptmp = Equation_total_size(v[0]);
				if (ptmp.x + ptmp.dx < rightx)
					check_fl_draw(" dx", eqx + ptmp.x + ptmp.dx - x, eqy + y - ptmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			}
			return;
		}
		if (u == at_division) {
			if (x0 < rightx) {
				int yy = eqy + y0 - 6;
				check_fl_line(eqx + x0 + 2, yy, eqx + x0 + w.dx - 2, yy, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				++yy;
				check_fl_line(eqx + x0 + 2, yy, eqx + x0 + w.dx - 2, yy, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			}
			return;
		}
		if (u == at_limit && v.size() >= 4) {
			if (x0 < rightx)
				check_fl_draw("lim", eqx + w.x - x, eqy + y - w.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			gen arg2 = v[1]; // 2nd arg of limit, i.e. the variable
			if (arg2.type == _EQW) {
				eqwdata & varg2 = *arg2._EQWptr;
				if (varg2.x + varg2.dx + 2 < rightx)
					check_fl_draw("->", eqx + varg2.x + varg2.dx + 2 - x, eqy + y - varg2.y, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			}
			if (v.size() >= 5) {
				arg2 = v[2]; // 3rd arg of lim, the point, draw a comma after if dir.
				if (arg2.type == _EQW) {
					eqwdata & varg2 = *arg2._EQWptr;
					if (varg2.x + varg2.dx < rightx)
						check_fl_draw(",", eqx + varg2.x + varg2.dx - x, eqy + y - varg2.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				}
			}
			return;
		}
		bool parenthesis = true;
		string opstring(",");
		if (u.ptr()->printsommet == &printsommetasoperator || equalposcomp(binary_op_tab(), u))
			opstring = u.ptr()->s;
		else {
			if (u == at_sto)
				opstring = ":=";
			parenthesis = false;
		}
		// int yy=y0; // y0 is the lower coordinate of the whole eqwdata
		// int opsize=int(fl_width(opstring.c_str()))+3;
		it = v.begin();
		itend = v.end() - 1;
		// Reminder: here tmp is the 1st arg eqwdata, w the whole eqwdata
		if ((itend - it == 1) && ((u == at_neg)
				|| (u == at_plus) // uncommented for +infinity
								 )) {
			if (u == at_neg && need_parenthesis(tmp.g) && tmp.g != at_prod) {
				if (tmp.x - lpsize < rightx)
					check_fl_draw("(", eqx + tmp.x - x - lpsize, eqy + y - tmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				if (tmp.x + tmp.dx < rightx)
					check_fl_draw(")", eqx + tmp.x - x + tmp.dx, eqy + y - tmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			}
			if (w.x < rightx) {
#if 1
				if (u == at_neg) {
					int xx = w.x - x;
					int cx = eqx + xx + 2, cy = eqy + y - w.baseline - fontsize / 3, ch = giacmax(2, (fontsize + 5) / 6);
					if (fontsize < 16) --cx; else ++cy;
					if (fontsize < 12) --cx;
					if (fontsize > 23) ++cx;
					if (fontsize > 29) ++cx;
					if (fontsize > 37) ++cx;
					fl_line(cx - ch, cy, cx + ch - 1, cy);
					--cy;
					fl_line(cx - ch, cy, cx + ch - 1, cy);
					if (fontsize > 13) {
						--cy;
						fl_line(cx - ch, cy, cx + ch - 1, cy);
						if (fontsize > 23) {
							cy += 3;
							fl_line(cx - ch, cy, cx + ch - 1, cy);
						}
					}
				}
				else
					check_fl_draw(u.ptr()->s, eqx + w.x - x, eqy + y - w.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
#else
				fl_font(FL_TIMES_BOLD, fontsize);
				check_fl_draw(u.ptr()->s, eqx + w.x - x, eqy + y - w.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
#endif
			}
			return;
		}
		// write first open parenthesis
		if (u == at_plus)
			parenthesis = false;
		else {
			if (parenthesis && need_parenthesis(tmp.g)) {
				if (w.x < rightx) {
					int pfontsize = max(fontsize, (fontsize + (tmp.baseline - tmp.y)) / 2);
					fl_font(FL_HELVETICA, pfontsize);
					check_fl_draw("(", eqx + w.x - x, eqy + y - tmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
					fl_font(FL_HELVETICA, fontsize);
				}
			}
		}
		for (;;) {
			// write close parenthesis at end
			int xx = tmp.dx + tmp.x - x;
			if (parenthesis && need_parenthesis(tmp.g)) {
				if (xx < rightx) {
					int pfontsize = min(max(fontsize, (fontsize + (tmp.baseline - tmp.y)) / 2), fontsize * 2);
					int deltapary = (2 * (pfontsize - fontsize)) / 3;
					fl_font(FL_HELVETICA, pfontsize);
					check_fl_draw(")", eqx + xx, eqy + y - tmp.baseline + deltapary, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
					fl_font(FL_HELVETICA, fontsize);
				}
				xx += rpsize;
			}
			++it;
			if (it == itend) {
				if (u.ptr()->printsommet == &printsommetasoperator || u == at_sto || equalposcomp(binary_op_tab(), u))
					return;
				else
					break;
			}
			// write operator
			if (u == at_prod) {
#if 1
				// check_fl_draw(".",eqx+xx+3,eqy+y-tmp.baseline-fontsize/3,equat->clip_x,equat->clip_y,equat->clip_w,equat->clip_h,0,0);
				fl_font(FL_TIMES_ITALIC, fontsize);
				check_fl_draw(opstring.c_str(), eqx + xx + 1, eqy + y - tmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
#else
				int cx = eqx + xx + 5, cy = -3 + eqy + y - tmp.baseline - fontsize / 3, ch = giacmax(2, fontsize / 6);
				// fl_line(cx-ch+1,cy,cx+ch-1,cy); // horizontal (smaller to avoid confusion with -)
				if (fontsize < 16) --cx;
				if (fontsize < 12) --cx;
				fl_line(cx, cy + ch, cx, cy - ch);
				fl_line(cx - ch, cy + ch, cx + ch, cy - ch);
				fl_line(cx - ch, cy - ch, cx + ch, cy + ch);
#endif
			}
			else {
				gen tmpgen;
				if (u == at_plus && (
						(it->type == _VECT && it->_VECTptr->back().type == _EQW && it->_VECTptr->back()._EQWptr->g == at_neg)
						||
						(it->type == _EQW && (is_integer(it->_EQWptr->g) || it->_EQWptr->g.type == _DOUBLE_) && is_strictly_positive(-it->_EQWptr->g, contextptr))
					)
				   )
					;
				else {
					if (xx + 1 < rightx)
						// fl_draw(opstring.c_str(),xx+1,y-tmp.y-tmp.dy/2+fontsize/2);
						check_fl_draw(opstring.c_str(), eqx + xx + 1, eqy + y - tmp.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
				}
			}
			// write right parent, update tmp
			tmp = Equation_total_size(*it);
			if (parenthesis && (need_parenthesis(tmp.g))) {
				if (tmp.x - lpsize < rightx) {
					int pfontsize = min(max(fontsize, (fontsize + (tmp.baseline - tmp.y)) / 2), fontsize * 2);
					int deltapary = (2 * (pfontsize - fontsize)) / 3;
					fl_font(FL_HELVETICA, pfontsize);
					check_fl_draw("(", eqx + tmp.x - pfontsize*lpsize / fontsize - x, eqy + y - tmp.baseline + deltapary, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
					fl_font(FL_HELVETICA, fontsize);
				}
			}
		} // end for (;;)
		if (w.x < rightx) {
			s = u.ptr()->s;
			fl_font(cst_greek_translate(s), fontsize);
			s += '(';
			check_fl_draw(s.c_str(), eqx + w.x - x, eqy + y - w.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
			fl_font(FL_HELVETICA, fontsize);
		}
		if (w.x + w.dx - rpsize < rightx)
			check_fl_draw(")", eqx + w.x + w.dx - x - rpsize, eqy + y - w.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
		return;
	}
	s = oper.print(contextptr);
	if (w.x < rightx) {
		fl_font(cst_greek_translate(s), fontsize);
		check_fl_draw(s.c_str(), eqx + w.x - x, eqy + y - w.baseline, equat->clip_x, equat->clip_y, equat->clip_w, equat->clip_h, 0, 0);
		fl_font(FL_HELVETICA, fontsize);
	}
}

bool Equation_box_sizes(const gen & g, int & l, int & h, int & x, int & y, attributs & attr, bool & selected, bool search_active = false) {
	if (g.type == _EQW) {
		eqwdata & w = *g._EQWptr;
		x = w.x;
		y = w.y;
		l = w.dx;
		h = w.dy;
		if (search_active)
			selected = w.active;
		else
			selected = w.selected;
		attr = w.eqw_attributs;
		return true;
	}
	else {
		if (g.type != _VECT || g._VECTptr->empty()) {
			l = 0;
			h = 0;
			x = 0;
			y = 0;
			attr = attributs(0, 0, 0);
			selected = false;
			return true;
		}
		gen & g1 = g._VECTptr->back();
		Equation_box_sizes(g1, l, h, x, y, attr, selected, search_active);
		return false;
	}
}


// check if interval xmin..xmax and x1..x2 have a non empty intersection
bool interval_crossing(int xmin, int xmax, int x1, int x2) {
	if (xmax < x1 || xmin > x2)
		return false;
	return true;
}

// Select in g the subrectangle xmin,ymin,xmax,ymax
void Equation_select_rectangle(gen & g, int x1, int y1, int x2, int y2, Equation * eq) {
	int xmin = min(x1, x2), xmax = max(x1, x2), ymin = min(y1, y2), ymax = max(y1, y2);
	if (g.type == _EQW) {
		Equation_select(g, true);
		if (g._EQWptr->g.type == _STRNG) {
			// compute begin/end_sel
			if (y1 < y2) {
				giac::swapint(y1, y2);
				giac::swapint(x1, x2);
			}
			eq->begin_sel = Equation_binary_search_pos(*g._EQWptr, x1, y1);
			eq->end_sel = Equation_binary_search_pos(*g._EQWptr, x2, y2);
		}
		return;
	}
	if (g.type != _VECT)
		return;
	vecteur & v = *g._VECTptr;
	// Find the first element of v containing the whole rectangle
	iterateur it = v.begin(), itend = v.end() - 1, last_sel_it;
	for (;it != itend;++it) {
		int x, y, l, h;
		attributs attr(0, 0, 0);
		bool selected;
		Equation_box_sizes(*it, l, h, x, y, attr, selected);
		if ((x <= xmin) && (y <= ymin) && (xmax <= x + l) && (ymax <= y + h)) {
			Equation_select_rectangle(*it, x1, y1, x2, y2, eq);
			return;
		}
	}
	// None, then we select each element that crosses the rectangle
	bool selectall = true, find_first_sel = false, find_last_sel = false;
	if (itend->type == _EQW && itend->_EQWptr->g == at_multistring) {
		if (y1 < y2) {
			giac::swapint(y1, y2);
			giac::swapint(x1, x2);
		}
		find_first_sel = true;
		selectall = false;
	}
	it = v.begin();
	for (;it != itend;++it) {
		int x, y, l, h;
		attributs attr(0, 0, 0);
		bool selected;
		Equation_box_sizes(*it, l, h, x, y, attr, selected);
		bool xcross = interval_crossing(xmin, xmax, x, x + l), ycross = interval_crossing(ymin, ymax, y, y + h);
		if (xcross && ycross) {
			Equation_select(*it, true);
			last_sel_it = it;
			if (find_first_sel) {
				find_first_sel = false;
				find_last_sel = true;
				eq->begin_sel = Equation_binary_search_pos(*it->_EQWptr, x1, y1);
			}
		}
		else {
			if (ycross && g.subtype == _HIST__VECT) {
				Equation_select(*it, true);
				last_sel_it = it;
			}
			else
				selectall = false;
		}
	}
	if (find_last_sel)
		eq->end_sel = Equation_binary_search_pos(*last_sel_it->_EQWptr, x2, y2);
	if (selectall)
		Equation_select(g, true);
}

// return true if g has some selection inside
bool Equation_adjust_xy(const gen & g, int & xleft, int & ytop, int & xright, int & ybottom, bool active_search = false) {
	int x, y, w, h;
	attributs f(0, 0, 0);
	bool selected;
	Equation_box_sizes(g, w, h, x, y, f, selected, active_search);
	if ((g.type == _EQW__VECT) || selected) { // terminal or selected
		xleft = x;
		ybottom = y;
		if (selected) { // g is selected
			ytop = y + h;
			xright = x + w;
			return true;
		}
		else { // no selection
			xright = x;
			ytop = y;
			return false;
		}
	}
	if (g.type != _VECT)
		return false;
	// last not selected, recurse
	iterateur it = g._VECTptr->begin(), itend = g._VECTptr->end() - 1;
	for (;it != itend;++it) {
		if (Equation_adjust_xy(*it, xleft, ytop, xright, ybottom, active_search))
			return true;
	}
	return false;
}

// Find i position of first selected item in it..itend
bool Equation_find_multistring_pos(const_iterateur it, const_iterateur itend, int & i, int &nrows, bool active_search = false) {
	int t;
	nrows = itend - it;
	for (i = 0;it != itend;++it, ++i) { // find row
		if (it->type != _EQW || it->_EQWptr->g.type != _STRNG)
			return false;
		if (Equation_adjust_xy(*it, t, t, t, t, active_search))
			break;
	}
	if (it == itend)
		return false;
	return true;
}

// Assumes it points to the beginning of a string, selects position i
void Equation_select_multistring_pos(iterateur it, int i) {
	iterateur jt = (it + i);
	Equation_select(*jt, true);
}

// find selection position in it..itend
// Return true if at least 1 item is selected
bool Equation_find_vector_pos(const_iterateur it, const_iterateur itend, int & i, int &nrows) {
	int t;
	nrows = itend - it;
	for (i = 0;it != itend;++it, ++i) { // find row in i
		if (Equation_adjust_xy(*it, t, t, t, t))
			break;
	}
	if (it == itend) {
		--i;
		return false;
	}
	return true;
}

// Find i,j position of first selected item in it..itend
bool Equation_find_matrix_pos(const_iterateur it, const_iterateur itend, int & i, int &j, int &nrows, int & ncols, bool active_search = false) {
	// (incomplete) check for a matrix: the first element must be a vector
	if (it->type != _VECT || it->_VECTptr->empty())
		return false;
	gen & tmp = it->_VECTptr->back();
	if (tmp.type != _EQW || tmp._EQWptr->g != at_makevector)
		return false;
	// find selection position and move down
	int t;
	nrows = itend - it;
	int c = it->_VECTptr->size();
	const_iterateur it0 = it;
	for (;it0 != itend;++it0) { // more complete matrix check
		if (it0->type != _VECT || int(it0->_VECTptr->size()) != c)
			return false;
	}
	for (i = 0;it != itend;++it, ++i) { // find row
		if (Equation_adjust_xy(*it, t, t, t, t, active_search))
			break;
	}
	if (it == itend || it->type != _VECT)
		return false;
	// find column
	const_iterateur jt = it->_VECTptr->begin(), jtend = it->_VECTptr->end() - 1;
	ncols = jtend - jt;
	for (j = 0;jt != jtend;++jt, ++j) {
		if (Equation_adjust_xy(*jt, t, t, t, t, active_search))
			break;
	}
	if (jt == jtend)
		return false;
	return true;
}

// Assumes it points to the beginning of the matrix, selects position i,j
void Equation_select_matrix_pos(iterateur it, int i, int j) {
	iterateur jt = (it + i)->_VECTptr->begin() + j;
	Equation_select(*jt, true);
}

// increase selection up (like HP49 eqw Up key)
// x,y is the cursor position
void Equation_select_up(gen & g, int x, int y, int mode) {
	if (g.type == _EQW) { // terminal -> select
		g._EQWptr->selected = true;
		return;
	}
	if (g.type != _VECT || g._VECTptr->empty())
		return;
	vecteur & v = *g._VECTptr;
	iterateur it = v.begin(), itend = v.end() - 1;
	if (v.back().type != _EQW)
		setsizeerr();
	eqwdata & w = *v.back()._EQWptr;
	int i, j, nrows, ncols;
	if (mode && w.g == at_makevector && Equation_find_matrix_pos(it, itend, i, j, nrows, ncols)) {
		--i;
		if (i < 0)
			i = nrows - 1;
		Equation_select(g, false);
		Equation_select_matrix_pos(it, i, j);
		return;
	}
	if (mode && w.g == at_multistring && Equation_find_multistring_pos(it, itend, i, nrows)) {
		--i;
		if (i < 0)
			i = nrows - 1;
		Equation_select(g, false);
		Equation_select_multistring_pos(it, i);
		return;
	}
	// find a box containing the cursor
	int x0, y0, w0, h0;
	attributs attr(0, 0, 0);
	bool selected;
	for (;it != itend;++it) {
		Equation_box_sizes(*it, w0, h0, x0, y0, attr, selected);
		if ((x0 < x) && (x0 + w0 >= x) && (y0 <= y) && (y0 + h0 >= y))
			break;
	}
	if (it == itend)
		return;
	// if the box is selected select g else recurse
	if (selected)
		Equation_select(g, true);
	else
		Equation_select_up(*it, x, y, mode);
}

gen Equation_data2gen(const gen & g, GIAC_CONTEXT) {
	if (g.type == _EQW) {
		eqwdata & e = *g._EQWptr;
		if (e.g.type == _STRNG && e.active && e.g.subtype)
			return gen(*e.g._STRNGptr, contextptr);
		return e.g;
	}
	if (g.type != _VECT)
		return g;
	vecteur & v = *g._VECTptr;
	const_iterateur it = v.begin(), itend = v.end() - 1;
	vecteur res;
	if (g.subtype == _HIST__VECT) {
		vecteur tmp(2);
		for (;it != itend;++it) {
			tmp[0] = Equation_data2gen(*it, contextptr);
			++it;
			if (it == itend)
				break;
			tmp[1] = Equation_data2gen(*it, contextptr);
			res.push_back(tmp);
		}
		return gen(res, _HIST__VECT);
	}
	res.reserve(itend - it);
	for (;it != itend;++it) {
		res.push_back(Equation_data2gen(*it, contextptr));
	}
	gen f = Equation_data2gen(*it, contextptr);
	if (f == at_makesuite)
		return gen(res, _SEQ__VECT);
	if (f == at_makevector) {
		if (f.subtype == _MATRIX__VECT && ckmatrix(res)) {
			iterateur it = res.begin();itend = res.end();
			for (;it != itend;++it)
				it->subtype = 0;
			return res;
		}
		return gen(res, f.subtype);
	}
	if (f == at_program && res.size() == 2)
		res.insert(res.begin() + 1, res.front()*zero);
	if (f == at_at && xcas_mode(contextptr) != 0) {
		if (res.size() == 2) {
			if (res.back().type == _VECT) {
				vecteur & resv = *res.back()._VECTptr;
				res.back() = gen(subvecteur(resv, vecteur(resv.size(), plus_one)), _SEQ__VECT);
			}
			else
				res.back() = res.back() - plus_one;
		}
	}
	gen arg = gen(res, g.subtype);
	if (f.type == _FUNC) {
		if (res.size() == 1)
			return symbolic(*f._FUNCptr, res.front());
		else {
			if (f == at_multistring) {
				gen tmp = _multistring(res, contextptr);
				return tmp;
			}
			return symbolic(*f._FUNCptr, arg);
		}
	}
	else
		return f(arg, contextptr);
}

gen Equation_selected2gen(Equation * eq, const gen & g, bool & selected, bool search_active = false) {
	const giac::context * contextptr = get_context(eq);
	if (g.type == _EQW) {
		eqwdata & e = *g._EQWptr;
		if (search_active)
			selected = e.active;
		else
			selected = e.selected;
		if (!selected)
			return Equation_nullstring();
		if (e.g.type != _STRNG)
			return e.g;
		if (e.active && e.g.subtype)
			return gen(*e.g._STRNGptr, contextptr); // parse
		// begin_sel/end_sel
		string res = *e.g._STRNGptr;
		if (!e.active)
			res = Equation_extract_string(*e.g._STRNGptr, eq->begin_sel, eq->end_sel);
		return string2gen(res, false);
	}
	selected = false;
	if (g.type != _VECT)
		return g;
	vecteur & v = *g._VECTptr;
	const_iterateur it = v.begin(), itend = v.end() - 1;
	if (itend->type == _EQW && itend->_EQWptr->g == at_multistring) {
		string res;
		if (!Equation_multistring_selection(it, itend, search_active))
			return string2gen(res, false);
		selected = true;
		if (it == itend) // selection inside one string
			return Equation_selected2gen(eq, *it, selected, search_active);
		res = Equation_extract_string(*it->_EQWptr->g._STRNGptr, eq->begin_sel, -1) + '\n';
		++it;
		for (;it != itend;++it)
			res = res + *it->_EQWptr->g._STRNGptr + '\n';
		res = res + Equation_extract_string(*itend->_EQWptr->g._STRNGptr, -1, eq->end_sel);
		return string2gen(res, false);
	}
	int w0, h0, x0, y0;
	attributs attr(0, 0, 0);
	Equation_box_sizes(*itend, w0, h0, x0, y0, attr, selected, search_active);
	if (selected)
		return Equation_data2gen(g, contextptr);
	vecteur res;
	res.reserve(itend - it);
	for (;it != itend;++it) {
		gen tmp = Equation_selected2gen(eq, *it, selected, search_active);
		if (selected)
			res.push_back(tmp);
	}
	selected = !res.empty();
	if (!selected)
		return zero;
	if (res.size() == 1)
		return res.front();
	gen f = Equation_data2gen(*it, contextptr);
	if (f == at_makesuite)
		return gen(res, _SEQ__VECT);
	if (f == at_makevector) {
		if (f.subtype == _MATRIX__VECT && ckmatrix(res)) {
			iterateur it = res.begin();itend = res.end();
			for (;it != itend;++it)
				it->subtype = 0;
			return res;
		}
		return gen(res, f.subtype);
	}
	if (f == at_program && res.size() == 2)
		res.insert(res.begin() + 1, res.front()*zero);
	if (f == at_at && xcas_mode(contextptr) != 0) {
		if (res.size() == 2) {
			if (res.back().type == _VECT) {
				vecteur & resv = *res.back()._VECTptr;
				res.back() = gen(subvecteur(resv, vecteur(resv.size(), plus_one)), _SEQ__VECT);
			}
			else
				res.back() = res.back() - plus_one;
		}
	}
	gen arg = gen(res, g.subtype);
	if (f.type == _FUNC)
		return symbolic(*f._FUNCptr, arg);
	else
		return f(arg, contextptr);
}

// return a pointer to the selected object
// it will modify g if for example a subsum of a sum is selected
gen * Equation_selected(gen & g, attributs & attr, int windowhsize, vecteur & position, bool active_search, GIAC_CONTEXT) {
	// FIXME begin_sel/end_sel
	int x0, y0, w0, h0, tmp;
	bool selected;
	if (!Equation_adjust_xy(g, tmp, tmp, tmp, tmp, active_search))
		return 0;
	Equation_box_sizes(g, w0, h0, x0, y0, attr, selected, active_search);
	if (selected)
		return &g;
	if (g.type != _VECT)
		return 0;
	vecteur & v = *g._VECTptr;
	iterateur it = v.begin(), itend = v.end() - 1;
	for (;it != itend;++it) { // search first selected item
		Equation_box_sizes(*it, w0, h0, x0, y0, attr, selected, active_search);
		if (selected)
			break;
	}
	if (it == itend) {
		it = v.begin();
		for (;it != itend;++it) {
			if (Equation_adjust_xy(*it, tmp, tmp, tmp, tmp, active_search)) {
				position.push_back(int(it - v.begin()));
				return Equation_selected(*it, attr, windowhsize, position, active_search, contextptr);
			}
		}
		return 0;
	}
	iterateur itb = it;
	position.push_back(int(it - v.begin()));
	// find the end of the selection
	++it;
	for (;it != itend;++it) {
		Equation_box_sizes(*it, w0, h0, x0, y0, attr, selected, active_search);
		if (!selected)
			break;
	}
	--it;
	Equation_box_sizes(*it, w0, h0, x0, y0, attr, selected, active_search);
	if (it == itb)
		return &*it;
	// arrange v
	if (itb == v.begin() && it + 1 == itend) {
		position.pop_back();
		return &g;
	}
	vecteur subop(itb, it + 1);
	subop.push_back(v.back());
	gen temp = Equation_data2gen(subop, contextptr);
	eqwdata e = Equation_total_size(*it);
	*it = Equation_compute_size(temp, attr, windowhsize, contextptr);
	Equation_translate(*it, e.x, e.y);
	int pos = itb - v.begin();
	v.erase(itb, it);
	return &v[pos];
}

// make a free copy of g
gen Equation_copy(const gen & g) {
	if (g.type == _EQW)
		return *g._EQWptr;
	if (g.type != _VECT)
		return g;
	vecteur & v = *g._VECTptr;
	const_iterateur it = v.begin(), itend = v.end();
	vecteur res;
	res.reserve(itend - it);
	for (;it != itend;++it)
		res.push_back(Equation_copy(*it));
	return gen(res, g.subtype);
}

// move selection right (like HP49 eqw Right key)
void Equation_select_rightleft(gen & g, Equation * eqptr, int windowhsize, bool right, int mode, GIAC_CONTEXT) {
	if (g.type != _VECT)  // we are on a terminal, nothing to do
		return;
	// find 1st object with something selected
	vecteur & v = *g._VECTptr;
	iterateur it = v.begin(), itend = v.end() - 1;
	eqwdata & w = *itend->_EQWptr;
	int i, j, nrows, ncols, tmp;
	if ((mode) && (w.g == at_makevector) && Equation_find_matrix_pos(it, itend, i, j, nrows, ncols)) {
		if (right)
			++j;
		else
			--j;
		if (j < 0)
			j = ncols - 1;
		if (j >= ncols)
			j = 0;
		Equation_select(g, false);
		Equation_select_matrix_pos(it, i, j);
		return;
	}
	for (;it != itend;++it) {
		if (Equation_adjust_xy(*it, tmp, tmp, tmp, tmp))
			break;
	}
	if (it == itend)
		return;
	iterateur it0 = it; // save first selected
	// find first non selected
	if (right || (mode == 2)) {
		++it;
		for (;it != itend;++it) {
			if (!Equation_adjust_xy(*it, tmp, tmp, tmp, tmp))
				break;
		}
		if (mode == 2 && it0 == v.begin() && it == itend)
			return;
		--it;
	}
	// it -> last selected if moving right or exchanging selection right/left
	int x0, y0, w0, h0;
	attributs attr(0, 0, 0);
	bool selected;
	Equation_box_sizes(*it, w0, h0, x0, y0, attr, selected);
	if (!selected) {
		Equation_select_rightleft(*it, eqptr, windowhsize, right, mode, contextptr);
		return;
	}
	iterateur it1 = it;
	if (right) {
		++it;
		if (it == itend)
			it = v.begin();
	}
	else {
		it = it0;
		if (it == v.begin())
			it = itend;
		--it;
	}
	if (mode == 2) { // exchange it0->it1 with it
		if (it1 == it0) {
			gen tmp = *it;
			*it = *it0;
			*it0 = tmp;
		}
		else {
			vecteur tmpv(it0, it1 + 1);
			tmpv.push_back(*itend);
			gen sel = Equation_data2gen(tmpv, contextptr);
			sel = Equation_compute_size(sel, attr, windowhsize, contextptr);
			Equation_select(sel, true);
			gen tmp = *it;
			*it = sel;
			*it1 = tmp;
			v.erase(it0, it1);
		}
		vecteur position;
		gen g_copy = Equation_copy(eqptr->data);
		gen * ptr = Equation_selected(eqptr->data, attr, windowhsize, position, false, contextptr);
		if (ptr) {
			if (ptr->type == _VECT && !ptr->_VECTptr->empty() && ptr->_VECTptr->back().type == _EQW && ptr->_VECTptr->back()._EQWptr->g == at_multistring) {
				eqptr->data = g_copy;
				return;
			}
			const gen & tmp = Equation_data2gen(eqptr->data, contextptr);
			eqptr->data = Equation_compute_size(tmp, attr, windowhsize, contextptr);
			const_iterateur it = position.begin(), itend = position.end();
			ptr = &eqptr->data;
			for (;it != itend;++it) {
				ptr = &(*ptr->_VECTptr)[it->val];
			}
			Equation_select(*ptr, true);
		}
		return;
	}
	else {
		// if shift-arrow, extend selection, mode==0 do not deselect
		if (mode == 1)
			Equation_select(g, false);
	}
	Equation_select(*it, true);
}

gen Equation_nullstring() {
	gen tmps = string2gen("", false);
	tmps.subtype = 1;
	return tmps;
}

gen Equation_nullhistlevel() {
	gen hh1, hh2(string2gen("", false));
	hh1 = Equation_nullstring();
	gen hh = gen(vecteur(1, makevecteur(hh1, hh2)), _HIST__VECT);
	return hh;
}

bool is_semi_expr(const gen & g) {
	if (!g.is_symb_of_sommet(at_expr))
		return false;
	gen tmp = g._SYMBptr->feuille;
	return tmp.type == _VECT && tmp.subtype == _SEQ__VECT && tmp._VECTptr->front().type == _STRNG && *tmp._VECTptr->front()._STRNGptr == ";";
}

gen Equation_nullstring(const attributs & a, int windowhsize, GIAC_CONTEXT) {
	gen g = Equation_compute_size(Equation_nullstring(), a, windowhsize, contextptr);
	Equation_select(g, true, 1);
	return g;
}

void Equation_remove_selection(gen & g, int & pos, Equation * eq) {
	const giac::context * contextptr = get_context(eq);
	if (g.type == _EQW) {
		eqwdata & e = *g._EQWptr;
		if (e.g.type == _STRNG) {
			eq->need_active_parse = false;
			string s = *e.g._STRNGptr;
			int ss = s.size();
			if (ss) {
				int begin_sel = max(0, min(eq->begin_sel, ss - 1));
				int end_sel = ss;
				if (eq->end_sel >= 0)
					end_sel = min(eq->end_sel, ss);
				if (end_sel < begin_sel)
					giac::swapint(begin_sel, end_sel);
				if (end_sel < ss)
					s = s.substr(0, begin_sel) + s.substr(end_sel, ss - end_sel);
				else
					s = s.substr(0, begin_sel);
				pos = begin_sel;
			}
			else
				pos = 0;
			e.g = string2gen(s, false);
		}
		else {
			eq->need_active_parse = true;
			e.g = string2gen(e.g.print(contextptr), false);
			e.g.subtype = 1;
			pos = e.g._STRNGptr->size();
		}
		e.active = true;
		e.selected = false;
		return;
	}
	if (g.type != _VECT)
		return;
	vecteur & v = *g._VECTptr;
	iterateur it = v.begin(), itend = v.end();
	if (Equation_is_selected(*(itend - 1), false)) {
		g = Equation_nullstring(v.back()._EQWptr->eqw_attributs, 0, contextptr);
		pos = 0;
		return;
	}
	for (;it != itend;++it) {
		if (Equation_is_selected(*it, false))
			break;
	}
	if (it != itend) {
		iterateur it0 = it;
		int pos = it0 - v.begin();
		for (;it != itend;++it) {
			if (!Equation_is_selected(*it, false))
				break;
		}
		if (v.back().type == _EQW && v.back()._EQWptr->g == at_multistring && it0->type == _EQW && it0->_EQWptr->g.type == _STRNG && eq->begin_sel >= 0 && eq->end_sel >= 0) {
			--it; // it0 first selected, it last selected
			int ss = it->_EQWptr->g._STRNGptr->size();
			int begin_sel = min(eq->begin_sel, ss), end_sel = min(eq->end_sel, ss);
			Equation_select(*it0, false);
			if (it == it0) {
				*it->_EQWptr->g._STRNGptr = it->_EQWptr->g._STRNGptr->substr(0, begin_sel) + it->_EQWptr->g._STRNGptr->substr(end_sel, ss - end_sel);
				Equation_select(*it, true, true);
				eq->active_pos = begin_sel;
				eq->need_active_parse = false;
				return ;
			}
			begin_sel = giacmin(eq->begin_sel, it0->_EQWptr->g._STRNGptr->size());
			*it0->_EQWptr->g._STRNGptr = it0->_EQWptr->g._STRNGptr->substr(0, begin_sel) + it->_EQWptr->g._STRNGptr->substr(end_sel, ss - end_sel);
			Equation_select(*it0, true, true);
			eq->active_pos = begin_sel;
			eq->need_active_parse = false;
			++it0;
			++it;
			iterateur it1 = it0;
			int deltay = 0;
			int n = it0 - v.begin();
			for (;it1 != it;++it1) {
				eqwdata e = Equation_total_size(*it1);
				deltay = deltay + e.dy;
			}
			v.erase(it0, it);
			int s = v.size();
			for (int j = n;j < s;++j) {
				Equation_translate(v[j], 0, deltay);
			}
			return;
		}
		// Check whether it0 is a string, in that case erase in the string
		if (it0 + 1 == it && it0->type == _EQW && it0->_EQWptr->g.type == _STRNG) {
			int ss = it0->_EQWptr->g._STRNGptr->size();
			int begin_sel = max(min(eq->begin_sel, ss), 0), end_sel = max(min(eq->end_sel, ss), 0);
			Equation_select(*it0, false);
			*it0->_EQWptr->g._STRNGptr = it0->_EQWptr->g._STRNGptr->substr(0, begin_sel) + it0->_EQWptr->g._STRNGptr->substr(end_sel, ss - end_sel);
			Equation_select(*it0, true, true);
			eq->active_pos = begin_sel;
			eq->need_active_parse = false;
			return ;
		}
		// delete from it0 to it
		// NOTE that for history, it must be of even size
		if (g.subtype == _HIST__VECT) {
			if ((it0 - v.begin()) % 2)
				--it0;
			if ((it - v.begin()) % 2)
				++it;
		}
		v.erase(it0, it);
		if (v.size() == 1) { // only the last item, i.e. operator
			g = Equation_nullstring(v.back()._EQWptr->eqw_attributs, 0, contextptr);
			pos = 0;
			return;
		}
		if (v.size() == 2
			// && (v.back()._EQWptr->g==at_plus || v.back()._EQWptr->g==at_prod)
		   ) {
			g = v.front();
			Equation_select(g, true);
			return;
		}
		/* it=v.begin(),itend=v.end();
		for (;it!=itend;++it)
		Equation_select(*it,true); */
		if (pos > 1 && pos < int(v.size()))
			Equation_select(v[pos-1], true);
		return;
	}
	// Remove 1st selection
	it = v.begin();
	for (;it != itend;++it) {
		if (Equation_is_selected(*it, true)) {
			Equation_remove_selection(*it, pos, eq);
			return;
		}
	}
}


/* not used // return true if conversion needed and done
bool string2vector(const string & s,vector<string> & v){
  int pos=s.find('\n'),newpos;
  int ss=s.size();
  if (pos<0 || pos >= ss)
    return false;
  v.clear();
  v.push_back(s.substr(0,pos));
  for (;pos<ss;){
    newpos=s.find('\n',pos);
    if (newpos<0 || newpos >= ss){
  v.push_back(s.substr(pos,ss-pos));
  return true;
    }
    v.push_back(s.substr(pos,newpos-pos));
    pos=newpos;
  }
}
*/

// return -1 if not in a multistring, position otherwise
int in_multistring(const gen & data, vecteur * & vptr) {
	if (data.type != _VECT)
		return -1;
	vecteur & v = *data._VECTptr;
	iterateur it = v.begin(), itend = v.end();
	if (itend == it)
		return -1;
	--itend;
	if (itend->type == _EQW && itend->_EQWptr->g == at_multistring) {
		for (;it != itend;++it) {
			if (it->type == _EQW && it->_EQWptr->active) {
				vptr = &v;
				return it -v.begin();
			}
		}
		return -1;
	}
	for (;it != itend;++it) {
		int i = in_multistring(*it, vptr);
		if (i != -1)
			return i;
	}
	return -1;
}

// return -1 if not in a multistring, position otherwise
int in_multistring(const gen & data) {
	vecteur * vptr;
	return in_multistring(data, vptr);
}






















Equation::Equation() {
	
}

void Equation::Set(giac::gen& g) {
	sz = GetSize();
	draw = NULL;
	
	value = g;
	const giac::context * contextptr = get_context(this);
	data = Equation_compute_size(g, attr, w(), contextptr);
	eqwdata e = Equation_total_size(data);
}

void Equation::Paint(Draw& w) {
	sz = GetSize();
	
	w.DrawRect(sz, White);
	
	//String s = data.print(ctx);
	//w.DrawText(0, 0, s);
	
	//EquationDraw(data, xleft, ytop, xleft + w() - vscroll->w(), ytop - h() - hscroll->h()
	//EquationDraw(data, 0, 0, sz.cx, sz.cy);
	attr.fontsize = 15;
	
	draw = &w;
	try {
		Equation_draw(data, 0, 0, sz.cx, sz.cy, this);
	}
	catch (...) {
		
	}
}


}
#endif
