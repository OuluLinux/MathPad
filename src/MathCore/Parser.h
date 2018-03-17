#ifndef _MathCore_Parser_h_
#define _MathCore_Parser_h_

#include "Node.h"

namespace MathCore {

String TrimQuotes(const String& s);
Node ParseExpression(const String& expr);
Node ParseGiacExpression(String expr);

}

#endif
