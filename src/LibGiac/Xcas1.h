#ifndef _XCAS_H
#define _XCAS_H

#include "config.h"
#include "first.h"

#include <string>
#include <iostream>
#include "History.h"
#include "Input.h"
#include "giac.h"
#include "input_lexer.h"


namespace xcas {

bool fltk_view(const giac::gen & g, giac::gen & ge, const std::string & filename, std::string & figure_filename, int file_type, const giac::context *);
void icas_eval(giac::gen & g, giac::gen & gg, int & reading_file, std::string &filename, giac::context * contextptr);
int read_file(const giac::gen & g);

}

#endif
