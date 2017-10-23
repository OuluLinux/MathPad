#ifndef _GIAC_MATHML_H
#define _GIAC_MATHML_H

#include "first.h"
#include <string>


namespace giac {


#ifndef GIAC_HAS_STO_38
extern const char mathml_preamble[];
extern const char mathml_end[];
class gen;
extern const unary_function_ptr * const  at_mathml;
gen _mathml(const gen & g, GIAC_CONTEXT);
gen _spread2mathml(const gen & g, GIAC_CONTEXT);
std::string spread2mathml(const matrice & m, int formule, GIAC_CONTEXT);
std::string matrix2mathml(const matrice & m, GIAC_CONTEXT);
std::string gen2mathml(const gen & e, GIAC_CONTEXT);
std::string gen2mathml(const gen & e, std::string &svg, GIAC_CONTEXT);
std::string gen2svg(const gen &e, GIAC_CONTEXT);
std::string gen2svg(const gen &e, double xmin, double xmax, double ymin, double ymax, GIAC_CONTEXT);
std::string svg_preamble(double width_cm, double height_cm, bool xml = true);
std::string svg_preamble(double svg_width_cm, double svg_height_cm, double xmin, double xmax, double ymin, double ymax, bool ortho, bool xml);
std::string svg_grid();
std::string svg_grid(double xmin, double xmax, double ymin, double ymax);
extern const char svg_end[];
std::string ingen2mathml(const gen & g, bool html5, GIAC_CONTEXT);

#endif


}


#endif
