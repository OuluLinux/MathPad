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
};

enum {
	OP_UNOT,
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
	
};

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
	
	void Swap(Node& n);
};


class Code {
	Node root;
	
	
public:
	typedef Code CLASSNAME;
	Code();
	
	
};

inline Code& GetCode() {return Single<Code>();}


class Parser {
	CParser p;
	Vector<Node*> scopes;
	bool is_mathmode = false;
	
public:
	typedef Parser CLASSNAME;
	Parser();
	
	void PushScope(Node& n) {scopes.Add(&n);}
	void PopScope() {scopes.Pop();}
	Node& TopScope() {return *scopes.Top();}
	Node& AddScope() {PushScope(TopScope().Add()); Node& n = TopScope(); GetLocation(n); return n;}
	
	void GetLocation(Node& n);
	
	inline Node BlockNode() {Node n(BLOCK); GetLocation(n); return n;}
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
	inline Node IntLiteral(int i) {Node n(INTLITERAL, i); GetLocation(n); return n;}
	inline Node DoubleLiteral(double d) {Node n(DOUBLELITEREAL, d); GetLocation(n); return n;}
	inline Node StringLiteral(String s) {Node n(STRINGLITERAL, s); GetLocation(n); return n;}
	inline Node Ident(String id) {Node n(ID, id); GetLocation(n); return n;}
	inline Node FunctionRefNode(String id) {Node n(FUNCREF, id); GetLocation(n); return n;}
	inline Node FunctionNode(String id) {Node n(FUNC, id); GetLocation(n); return n;}
	inline Node CallNode(String id) {Node n(CALL, id); GetLocation(n); return n;}
	inline Node ClassNode(String id) {Node n(CLASS, id); GetLocation(n); return n;}
	inline Node VariableNode(String type, String name) {Node n(VARIABLE, name, type); GetLocation(n); return n;}
	
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
	void Block();
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
