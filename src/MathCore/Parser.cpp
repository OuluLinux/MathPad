#include "MathCore.h"
#include "Node.h"

struct SyntaxError : public Exc                 { SyntaxError() {*(String*)this = "Syntax error";} } syn_err;


namespace MathCore {

String TrimQuotes(const String& s) {
	if (s.GetCount() < 2) return s;
	if (s[0] == '\"' && s[s.GetCount()-1] == '\"')
		return s.Mid(1,s.GetCount()-2);
	else
		return s;
}

VectorMap<String, int> builtin_funcs;

void InitBuiltinFunctions() {
	if (builtin_funcs.GetCount() != 0) return;
	builtin_funcs.Add("abs", FUNC_ABS);
	builtin_funcs.Add("conj", FUNC_CONJUGATE);
	builtin_funcs.Add("D", FUNC_DERIVE);
	builtin_funcs.Add("integrate", FUNC_INTEGRATE);
	builtin_funcs.Add("sin", FUNC_SIN);
	builtin_funcs.Add("cos", FUNC_COS);
	builtin_funcs.Add("tan", FUNC_TAN);
	builtin_funcs.Add("asin", FUNC_ASIN);
	builtin_funcs.Add("acos", FUNC_ACOS);
	builtin_funcs.Add("atan", FUNC_ATAN);
	builtin_funcs.Add("sqrt", FUNC_SQRT);
	builtin_funcs.Add("pow", FUNC_POW);
	builtin_funcs.Add("log", FUNC_LOG);
	builtin_funcs.Add("sum", FUNC_SUM);
	builtin_funcs.Add("limit", FUNC_LIMIT);
	builtin_funcs.Add("plusmin", FUNC_PLUSMIN);
	builtin_funcs.Add("exp", FUNC_EXP);
}

Node Term(CParser& p);
Node Exp(CParser& p);
Node Assign(CParser& p);
Node Add(CParser& p);
Node Pow(CParser& p);

Node Pow10(CParser& p) {
	Node out;
	if (p.Char('e')) {
		if (p.Char('+')) {
			return Pow(10, Term(p));
		}
		else {
			return Pow(10, Term(p));
		}
	}
	return out;
}


Node Term(CParser& p)
{
	Node out;
	
	if (p.IsId()) {
		out.SetType(TYPE_ID);
		String value = p.ReadId();
		if (value == "infinity") {
			value = "inf";
		}
		out.SetString(value);
		// TODO: parse derivative '
		while (p.IsChar('\'')) {
			p.Char('\'');
			out.SetString(out.GetString() + "'");
		}
	}
	// Crashes if double is not first
	else if (p.IsDouble()) {
		double d = p.ReadDouble();
		String str = AsString( d );
		if (str.Find(".") == -1 && d < INT_MAX && d >= INT_MIN) {
			out.SetType(TYPE_INT);
			out.SetInt(d);
		} else {
			bool sign = d < 0;
			double d2 = (sign ? d * -1. : d);
			if (d2 >= 0.001 && d2 < 1000000.) {
				out.SetType(TYPE_DOUBLE);
				out.SetDouble(d);
			} else {
				int ten_exp = 0;
				while (d2 >= 10.0) { d2 /= 10.0; ten_exp++; }
				while (d2 < 1.0) { d2 *= 10.0; ten_exp--; }
				if (sign) d2 *= -1.0;
				out = Node(d2) * Pow10(ten_exp);
			}
		}
		char c = *p.GetPtr();
		/*if (p.IsChar('e')) {
			out = out * Pow10(p);
		}*/
		if (p.Char('_')) {
			out = Type(out, Add(p));
		}
	}
	else if (p.IsNumber()) {
		String str = AsString( p.ReadNumber() );
		out.SetType(TYPE_INT);
		// TODO: read oct & hex
		out.SetInt(StrInt(str));
		if (p.IsChar('e')) {
			out = out * Pow10(p);
		}
		if (p.Char('_')) {
			out = Type(out, Add(p));
		}
	}
	else if (p.IsInt()) {
		String str = AsString( p.ReadInt() );
		out.SetType(TYPE_INT);
		out.SetInt(StrInt(str));
		if (p.IsChar('e')) {
			out = out * Pow10(p);
		}
		if (p.Char('_')) {
			out = Type(out, Add(p));
		}
	}
	else if (p.IsString()) {
		out.SetType(TYPE_TEXT);
		out.SetString(AsString( p.ReadString() ));
	}
	// Giac notation
	else if (p.Char('*')) {
		// PLEASE GIVE EXAMPLE!!!!
	}
	/*else if(p.Char('-')) {
		Node out  = new Node();
		out.SetType(TYPE_UNARY);
		out.SetDataInt(UNARY_NEG);
		out.Add( Term(p) );
		//return Unary(p, out);
	}*/
	
	if(p.Char('(')) {
		String value = out.GetString();
		if (value.GetCount()) {
			int func = builtin_funcs.Find(value);
			if (func != -1) {
				func = builtin_funcs[func];
				out.SetFunction(func);
			} else {
				out.SetFunction(value);
			}
		}
		else {
			out.SetType(TYPE_PARENTHESIS);
			out.SetDataInt(0);
		}
		for (; !p.IsChar(')');) {
			out.Add(Assign(p));
			//LOG(out.AsString());
			//LOG(out.AsInlineString());
			int chr = *p.GetPtr();
			if(p.Char(',')) 
				;
			if(p.IsId("and")) {
				p.ReadId();
			}
		}
		p.Char(')');
	} 
	else if(p.Char('[')) {
		out.SetType(TYPE_ARRAY);
		out.SetDataInt(0);
		for (; !p.IsChar(']');) {
			out.Add(Assign(p));
			if(p.Char(','))
				;
			else if(p.IsId("and")) {
				p.ReadId();
			}
		}
		p.Char(']');
	} 
		
	return out;
}


Node Type(CParser& p, Node in) {
	Node out;
	if(p.Char('#')) {
		out.Add( in );
		out.SetType(TYPE_TYPE);
		out.Add( Term(p) );
		return Type(p, out);
	}
	else
		return in;
}

Node Type(CParser& p) {
	return Type(p, Term(p));
}





Node Pow(CParser& p, Node in) {
	Node out;
	out.Add( in );
	//for(;;)
	if(p.Char('^')) {
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_POW);
		out.Add( Type(p) );
		return Pow(p, out);
	}
	else
		return in;
}

Node Pow(CParser& p) {
	return Pow(p, Type(p));
}



/*
<Op Mult>    Math::= <Op Mult> '*' <Op Unary>
               | <Op Mult> '/' <Op Unary>
               | <Op Mult> '%' <Op Unary>
               | <Op Unary>
*/
Node Mult(CParser& p, Node in)
{
	Node out;
	out.Add( in );
	if(p.Char('*')) {
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_MUL);
		out.Add( Pow(p) );
		return Mult(p, out);
	}
	else
	if(p.Char('/')) {
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_DIV);
		out.Add( Pow(p) );
		return Mult(p, out);
	}
	else
		return in; //return out;
}


Node Mult(CParser& p) {
	return Mult(p, Pow(p));
}


Node Unary(CParser& p, Node in) {
	if (in.IsNull()) {
		if(p.Char('-')) {
			Node out;
			out.SetType(TYPE_UNARY);
			out.SetDataInt(UNARY_NEG);
			out.Add( Mult(p) );
			//return Unary(p, out);
			return out;
		}
		else if(p.Char('+')) {
			return Mult(p);
		}
	}
	return in;
}

Node Unary(CParser& p) {
	return Unary(p, Mult(p));
}


/*
<Op Add>     Math::= <Op Add> '+' <Op Mult>
               | <Op Add> '-' <Op Mult>
               | <Op Mult>
*/
Node Add(CParser& p, Node in) {
	Node out;
	int chr = *p.GetPtr();
	if(p.Char('+')) {
		out.Add(in);
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_ADD);
		out.Add( Unary(p) );
		return Add(p, out);
	}
	else if(p.Char('-')) {
		out.Add(in);
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_SUB);
		out.Add( Unary(p) );
		return Add(p, out);
	}
	else {
		//out.Add(in);
		return in;
	}
}
Node Add(CParser& p) {
	return Add(p, Unary(p));
}


/*
<Op Compare> Math::= <Op Compare> '<'  <Op Shift>
               | <Op Compare> '>'  <Op Shift>
               | <Op Compare> '<=' <Op Shift>
               | <Op Compare> '>=' <Op Shift>
               | <Op Shift>
*/
Node Compare(CParser& p, Node in) {
	Node out;
	out.Add(in);
	if (p.Char2('!', '=')) {
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_INEQ);
		out.Add( Add(p) );
		return Compare(p, out);
	}
	else if(p.Char('<')) {
		out.SetType(TYPE_OP);
		if(p.IsChar('=')) {
			p.Char('=');
			out.SetDataInt(OP_LSEQ);
		} else {
			out.SetDataInt(OP_LESS);
		}
		out.Add( Add(p) );
		return Compare(p, out);
	}
	else if(p.Char('>')) {
		out.SetType(TYPE_OP);
		if(p.IsChar('=')) {
			p.Char('=');
			out.SetDataInt(OP_GREQ);
		} else {
			out.SetDataInt(OP_GRT);
		}
		out.Add( Add(p) );
		return Compare(p, out);
	}
	else
		return in;
}
Node Compare(CParser& p) {
	return Compare(p, Add(p));
}


/*
<Op Equate>  Math::= <Op Equate> '==' <Op Compare>
               | <Op Equate> '!=' <Op Compare>
               | <Op Compare>
*/
Node Equate(CParser& p, Node in) {
	Node out;
	out.Add(in);
	
	if(p.Char('=')) {
		if (p.IsChar('=')) p.Char('=');
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_EQ);
		out.Add( Compare(p) );
		return Equate(p, out);
	}
	else
		return in;
}
Node Equate(CParser& p) {
	return Equate(p, Compare(p));
}

/*
<Op BinAND>  Math::= <Op BinAND> '&' <Op Equate>
               | <Op Equate>
*/
Node And(CParser& p, Node in) {
	Node out;
	out.Add(in);
	if(p.Char('&')) {
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_AND);
		if(p.Char('&'))
			;
		out.Add( Equate(p) );
		return And(p, out);
	}
	else
		return in;
}
Node And(CParser& p) {
	return And(p, Equate(p));
}

/*
<Op BinXOR>  Math::= <Op BinXOR> '^' <Op BinAND>
               | <Op BinAND>
*/
/*Node Xor(CParser& p, Node in) {
	Node out;
	out.Add(in);
	if(p.Char('^')) {
		out.GetKind() = TYPE_OP;
		out.op = OP_POW;
		if(p.Char('^'));
		out.Add( Mul(p) );
		return Exp(p, out);
	}
}*/

/*
<Op BinOR>   Math::= <Op BinOr> '|' <Op BinXOR>
               | <Op BinXOR>
*/
Node Or(CParser& p, Node in) {
	Node out;
	out.Add(in);
	if(p.Char('|')) {
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_OR);
		if(p.Char('|'))
			;
		out.Add( And(p) );
		return Or(p, out);
	}
	else
		return in;
}
Node Or(CParser& p) {
	return Or(p, And(p));
}


/*
<Op Assign>  Math::= <Op Or> ':='   <Op Assign>
               | <Op Or> '+='   <Op Assign>
               | <Op Or>
*/

/*
<Op Equate>  Math::= <Op Equate> '==' <Op Compare>
               | <Op Equate> '!=' <Op Compare>
               | <Op Compare>
*/
Node Assign(CParser& p, Node in) {
	Node out;
	out.Add(in);
	
	if(p.Char2(':', '=')) {
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_ASSIGN);
		out.Add( Add(p) );
		return Assign(p, out);
	}
	else if(p.Char2('+', '=')) {
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_PLUSEQ);
		out.Add( Add(p) );
		return Assign(p, out);
	}
	else
		return in;
}

Node Assign(CParser& p)
{
	return Assign(p, Or(p));
}

/*
<Expr>       Math::= <Expr> ',' <Op Assign>   
               | <Op Assign>
*/
Node Exp(CParser& p, Node in) {
	Node out;
	out.Add(in);
	if(p.Char(',')) {
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_LIST);
		out.Add( Or(p) );
		return Exp(p, out);
	}
	else if(p.IsId("and")) {
		p.ReadId();
		out.SetType(TYPE_OP);
		out.SetDataInt(OP_LIST);
		out.Add( Or(p) );
		return Exp(p, out);
	}
	else
		return out;
}


// TODO: make thread safe
const char* begin_pos;
const char* last_exp_pos;
Node Exp(CParser& p)
{
	const char* pos = p.GetPtr();
	if (last_exp_pos == pos) {
		String chr; chr.Cat(*p.GetPtr());
		LOG("Syntax error at: " << (int)(p.GetPos().ptr-begin_pos) << " '" << chr << "'");
		throw syn_err;
	}
	last_exp_pos = pos;
	return Exp(p, Assign(p));
}

Node ParseExpression(const String& expr) {
	last_exp_pos = 0;
	
	
	InitBuiltinFunctions();
	
	Node node;
	
	String l;
	WString ws = expr.ToWString();
	for(int i = 0; i < ws.GetCount(); i++) {
		int chr = ws[i];
		if (chr >= 128) l.Cat('1');
		else l.Cat(chr);
	}
	
	if(l.IsEmpty())
		return node;
	CParser p;
	p.SkipSpaces();
	p.Set(l);
	
	const char* begin = p.GetPtr();
	begin_pos = begin;
	
	//LOG("ParseExpression: " << l);
	
	try {
		Node out = Exp(p);
		//LOG(out.AsString());
		//LOG(out.AsInlineString());
		out.Trim();
		out.Normalize();
		//LOG(out.AsString());
		//LOG(out.AsInlineString());
		return out;
	}
	catch(CParser::Error e) {
		LOG("ERROR: " << e);
	}
	catch(SyntaxError e) {
		String chr;
		chr.Cat(*p.GetPtr());
		LOG("Syntax error at: " << (int)(p.GetPos().ptr-begin) << " '" << chr << "'" << e);
	}
	
	
	return node;
}


Node ParseGiacExpression(String expr) {
	expr.Replace("list[","[");
	expr.Replace(" or ", "|");
	expr.Replace(" and ", "&");
	
	Node node = ParseExpression(expr);
	//LOG(expr << " --> " << node.AsInlineString());
	
	return node;
}




}
