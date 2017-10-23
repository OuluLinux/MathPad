#ifndef _INPUT_H
#define _INPUT_H

#include "config.h"

#include "vector.h"
#include <string>
#include "first.h"
#include "gen.h"
#include "identificateur.h"

#ifdef HAVE_LC_MESSAGES
#include <locale.h>
#endif
#include "giacintl.h"


namespace xcas {


// If use_external_browser is true you can change the browser used
// by setting the BROWSER env variable, e.g. export BROWSER=...
extern bool use_external_browser;
// alert if the command is not successfull
void system_browser(const std::string & s);

// return the last keyword of s
std::string motclef(const std::string & s);


}

#endif
