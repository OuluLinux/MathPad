#ifndef _Script_Script_h_
#define _Script_Script_h_


#include <Core/Core.h>
using namespace Upp;

enum {
	UNKNOWN,
	ID,
	OP,
	VARIABLE,
	FUNCREF,
	FUNC,
	CLASS,
	CALL,
	INTLITERAL,
	DOUBLELITEREAL,
	STRINGLITERAL,
	BLOCK,
	SWITCHBLOCK,
	SWITCH,
	IF,
	SCOPE,
	WHILE,
	FOR,
	RETURN,
	BREAK,
	USING,
	NAMESPACE,
	TRY,
};

enum {
	OP_UNOT,
	OP_INC,
	OP_DEC,
	OP_TYPE,
	OP_EXP,
	OP_MUL,
	OP_DIV,
	OP_MOD,
	OP_ADD,
	OP_SUB,
	OP_LSHIFT,
	OP_RSHIFT,
	OP_EQUAL,
	OP_NEQUAL,
	OP_LEQUAL,
	OP_GEQUAL,
	OP_LESS,
	OP_GREATER,
	OP_AND,
	OP_OR,
	OP_XOR,
	OP_ANDAND,
	OP_OROR,
	OP_PLUSEQUAL,
	OP_MINUSEQUAL,
	OP_ASSIGN,
	OP_INLINECOND,
	OP_METHOD,
};

inline String GetOpString(int op) {
	switch (op) {
		case OP_UNOT: return "!";
		case OP_INC: return "++";
		case OP_DEC: return "--";
		case OP_TYPE: return "M type";
		case OP_EXP: return "M pow";
		case OP_MUL: return "*";
		case OP_DIV: return "/";
		case OP_MOD: return "%";
		case OP_ADD: return "+";
		case OP_SUB: return "-";
		case OP_LSHIFT: return "<<";
		case OP_RSHIFT: return ">>";
		case OP_EQUAL: return "==";
		case OP_NEQUAL: return "!=";
		case OP_LEQUAL: return "<=";
		case OP_GEQUAL: return ">=";
		case OP_LESS: return "<";
		case OP_GREATER: return ">";
		case OP_AND: return "&";
		case OP_OR: return "|";
		case OP_XOR: return "^";
		case OP_ANDAND: return "&&";
		case OP_OROR: return "||";
		case OP_PLUSEQUAL: return "+=";
		case OP_MINUSEQUAL: return "-=";
		case OP_ASSIGN: return "=";
		case OP_INLINECOND: return "?:";
		case OP_METHOD: return ".";
	}
	return "Unknown";
}
inline String GetNodeHeaderString(int kind, const int& int_data, const double& dbl_data, const String& str_data) {
	switch (kind) {
		case UNKNOWN: return "Unknown";
		case ID: return "Ident (" + (str_data) + ")";
		case OP: return "Op (" + GetOpString(int_data) + ")";
		case VARIABLE: return "Variable (" + (str_data) + ")";
		case FUNCREF: return "Function reference (" + (str_data) + ")";
		case FUNC: return "Function (" + (str_data) + ")";
		case CLASS: return "Class (" + (str_data) + ")";
		case CALL: return "Call (" + (str_data) + ")";
		case INTLITERAL: return "Int literal (" + IntStr(int_data) + ")";
		case DOUBLELITEREAL: return "Double literal (" + DblStr(dbl_data) + ")";
		case STRINGLITERAL: return "String literal (" + (str_data) + ")";
		case BLOCK: return "Block";
		case SWITCHBLOCK: return "Switch block";
		case SWITCH: return "Switch";
		case IF: return "If";
		case SCOPE: return "Scope";
		case WHILE: return "While";
		case FOR: return "For";
		case RETURN: return "Return";
		case BREAK: return "Break";
		case USING: return "Using (" + (str_data) + ")";
		case NAMESPACE: return "Namespace (" + (str_data) + ")";
		case TRY: return "Try";
		case OP_METHOD: return "Method (" + (str_data) + ")";
		default: return "<no kind string set>";
	}
	return "Unknown";
}

class Node {

protected:
	friend class Parser;
	
	Node* portal = NULL;
	Array<Node> nodes;
	String name;
	String str_data;
	double dbl_data = 0.0;
	int int_data = 0;
	int kind = UNKNOWN;
	int line = -1, col = -1;
	
	
public:
	typedef Node CLASSNAME;
	Node() {}
	Node(int kind, String str) : kind(kind), str_data(str) {}
	Node(int kind, double d) : kind(kind), dbl_data(d) {}
	Node(int kind, int i) : kind(kind), int_data(i) {}
	Node(int kind) : kind(kind) {}
	Node(int kind, String name, String str) : kind(kind), name(name), str_data(str) {}
	Node(int kind, String name, double d) : kind(kind), name(name), dbl_data(d) {}
	Node(int kind, String name, int i) : kind(kind), name(name), int_data(i) {}
	
	Node& Add() {Node& n=nodes.Add(); n.portal=this; return n;}
	
	bool IsUnknown() const {return kind == UNKNOWN;}
	int GetKind() const {return kind;}
	String GetKey() const {return name;}
	
	void Swap(Node& n);
	
	void NodeAsString(String& out, String name, Node* node, int indent=0, Vector<Node*>* stack=0);
	String AsString(String name="", int indent=0, Vector<Node*>* stack=0);
};


class Code {
	
protected:
	friend class Parser;
	Node root;
	
	
public:
	typedef Code CLASSNAME;
	Code() {}
	
	
	void Dump() {LOG(root.AsString());}
};

inline Code& GetCode() {return Single<Code>();}


class Parser {
	CParser p;
	Vector<Node*> scopes;
	int is_mathmode = false;
	
public:
	typedef Parser CLASSNAME;
	Parser() {}
	
	void PushScope(Node& n) {scopes.Add(&n);}
	void PopScope() {scopes.Pop();}
	Node& TopScope() {return *scopes.Top();}
	Node& AddScope() {PushScope(TopScope().Add()); Node& n = TopScope(); GetLocation(n); return n;}
	
	void GetLocation(Node& n);
	void Path(String id);
	
	inline Node BlockNode(bool math) {Node n(BLOCK, (int)math); GetLocation(n); return n;}
	inline Node SwitchBlockNode() {Node n(SWITCHBLOCK); GetLocation(n); return n;}
	inline Node SwitchNode() {Node n(SWITCH); GetLocation(n); return n;}
	inline Node IfNode() {Node n(IF); GetLocation(n); return n;}
	inline Node ScopeNode() {Node n(SCOPE); GetLocation(n); return n;}
	inline Node WhileNode() {Node n(WHILE); GetLocation(n); return n;}
	inline Node ForNode() {Node n(FOR); GetLocation(n); return n;}
	inline Node ReturnNode() {Node n(RETURN); GetLocation(n); return n;}
	inline Node BreakNode() {Node n(BREAK); GetLocation(n); return n;}
	inline Node UsingNode() {Node n(USING); GetLocation(n); return n;}
	inline Node NamespaceNode() {Node n(NAMESPACE); GetLocation(n); return n;}
	inline Node TryNode() {Node n(TRY); GetLocation(n); return n;}
	inline Node IntLiteral(int i) {Node n(INTLITERAL, i); GetLocation(n); return n;}
	inline Node DoubleLiteral(double d) {Node n(DOUBLELITEREAL, d); GetLocation(n); return n;}
	inline Node StringLiteral(String s) {Node n(STRINGLITERAL, s); GetLocation(n); return n;}
	inline Node Ident(String id) {Node n(ID, id); GetLocation(n); return n;}
	inline Node FunctionRefNode(String id) {Node n(FUNCREF, id); GetLocation(n); return n;}
	inline Node FunctionNode(String id) {Node n(FUNC, id); GetLocation(n); return n;}
	inline Node CallNode(String id) {Node n(CALL, id); GetLocation(n); return n;}
	inline Node ClassNode(String id) {Node n(CLASS, id); GetLocation(n); return n;}
	inline Node VariableNode(String type, String name) {Node n(VARIABLE, name, type); GetLocation(n); return n;}
	
	void Set(const String& s) {p.Set(s);}
	void Factor();
	void Unary();
	void Type();
	void Expo();
	void Term();
	void Expr();
	void Shift();
	void Condition();
	void Logic();
	void Ternary();
	void Base();
	void CompilationUnit();
	void Block(bool math=false);
	void SwitchBlock();
	void Statement();
	void CheckLegalBreak();
	void Op(int i);
	void ParseFunctionDefinition(String ret_type, String func_name);
	void ParseTryDefinition();
	void ParseClassDefinition();
	void FunctionCall(String func_name);
	void ParseFunctionArguments();
	
	
	
	Callback1<String> WhenError;
	
};

#endif
