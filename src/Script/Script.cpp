#include "Script.h"



void Node::Swap(Node& n) {
	Upp::Swap(nodes,	n.nodes);
	Upp::Swap(name,		n.name);
	Upp::Swap(str_data,	n.str_data);
	Upp::Swap(dbl_data,	n.dbl_data);
	Upp::Swap(int_data,	n.int_data);
	Upp::Swap(kind,		n.kind);
	Upp::Swap(line,		n.line);
	Upp::Swap(col,		n.col);
}

void Node::NodeAsString(String& out, String name, Node* node, int indent, Vector<Node*>* stack) {
	for(int j = 0; j < stack->GetCount(); j++) {
		if ((*stack)[j] == node) {
			node = 0;
			break;
		}
	}
	if (!node) {
		out << "\n\t";
		for(int j = 0; j < indent; j++) out << "\t";
		out << name << " (recursive)";
		return;
	}
	else {
		out << "\n" << node->AsString(name, indent+1, stack);
	}
}

String Node::AsString(String name, int indent, Vector<Node*>* stack) {
	bool delete_stack = false;
	if (stack == 0) {
		stack = new Vector<Node*>();
		delete_stack = true;
	}
	stack->Add(this);
	String out;
	for(int i = 0; i < indent; i++) out.Cat('\t');
	out << name << ": " << GetNodeHeaderString(kind, name, int_data, dbl_data, str_data);
	for(int i = 0; i < nodes.GetCount(); i++)
		NodeAsString(out, nodes[i].GetKey(), &nodes[i], indent, stack);
	stack->Remove(stack->GetCount()-1);
	if (delete_stack) {
		delete stack;
	}
	return out;
}

String Node::ToCode(int indent) {
	String s;
	//s.Cat('\t', indent);
	
	if (kind==COMPILATIONUNIT) {
		for(int i = 0; i < nodes.GetCount(); i++) {
			s << nodes[i].ToCode(indent);
			char last = s.GetCount() ? *(s.End()-1) : 0;
			if (last != '\n' && last != ';')
				s << ";\n";
		}
	}
	else if (kind == ID) {
		s << name;
	}
	else if (kind == OP) {
		switch (int_data) {
			case OP_UNOT:
				s << "!" << nodes[1].ToCode();
				break;
			case OP_INC:
				s << nodes[0].ToCode() << "++";
				break;
			case OP_DEC:
				s << nodes[0].ToCode() << "--";
				break;
			case OP_TYPE:
				s << "(" << nodes[0].ToCode() << "'" << nodes[1].ToCode() << ")";
				break;
			case OP_POW:
				s << "(" << nodes[0].ToCode() << "**" << nodes[1].ToCode() << ")";
				break;
			case OP_MUL:
				s << "(" << nodes[0].ToCode() << "*" << nodes[1].ToCode() << ")";
				break;
			case OP_DIV:
				s << "(" << nodes[0].ToCode() << "/" << nodes[1].ToCode() << ")";
				break;
			case OP_MOD:
				s << "(" << nodes[0].ToCode() << "%" << nodes[1].ToCode() << ")";
				break;
			case OP_ADD:
				s << "(" << nodes[0].ToCode() << "+" << nodes[1].ToCode() << ")";
				break;
			case OP_SUB:
				s << "(" << nodes[0].ToCode() << "-" << nodes[1].ToCode() << ")";
				break;
			case OP_LSHIFT:
				s << "(" << nodes[0].ToCode() << "<<" << nodes[1].ToCode() << ")";
				break;
			case OP_RSHIFT:
				s << "(" << nodes[0].ToCode() << ">>" << nodes[1].ToCode() << ")";
				break;
			case OP_EQUAL:
				s << "(" << nodes[0].ToCode() << "==" << nodes[1].ToCode() << ")";
				break;
			case OP_NEQUAL:
				s << "(" << nodes[0].ToCode() << "!=" << nodes[1].ToCode() << ")";
				break;
			case OP_LEQUAL:
				s << "(" << nodes[0].ToCode() << "<=" << nodes[1].ToCode() << ")";
				break;
			case OP_GEQUAL:
				s << "(" << nodes[0].ToCode() << ">=" << nodes[1].ToCode() << ")";
				break;
			case OP_LESS:
				s << "(" << nodes[0].ToCode() << "<" << nodes[1].ToCode() << ")";
				break;
			case OP_GREATER:
				s << "(" << nodes[0].ToCode() << ">" << nodes[1].ToCode() << ")";
				break;
			case OP_AND:
				s << "(" << nodes[0].ToCode() << "&" << nodes[1].ToCode() << ")";
				break;
			case OP_OR:
				s << "(" << nodes[0].ToCode() << "|" << nodes[1].ToCode() << ")";
				break;
			case OP_XOR:
				s << "(" << nodes[0].ToCode() << "^" << nodes[1].ToCode() << ")";
				break;
			case OP_ANDAND:
				s << "(" << nodes[0].ToCode() << "&&" << nodes[1].ToCode() << ")";
				break;
			case OP_OROR:
				s << "(" << nodes[0].ToCode() << "||" << nodes[1].ToCode() << ")";
				break;
			case OP_PLUSEQUAL:
				s << nodes[0].ToCode() << "+=" << nodes[1].ToCode();
				break;
			case OP_MINUSEQUAL:
				s << nodes[0].ToCode() << "-=" << nodes[1].ToCode();
				break;
			case OP_ASSIGN:
				s << nodes[0].ToCode() << "=" << nodes[1].ToCode();
				break;
			case OP_INLINECOND:
				s << "(" << nodes[0].ToCode() << "?" << nodes[1].ToCode() << ":" << nodes[2].ToCode() << ")";
				break;
			case OP_METHOD:
				s << "(" << nodes[0].ToCode() << "." << nodes[1].ToCode() << ")";
				break;
			case OP_GETA:
				s << "&" << nodes[1].ToCode();
				break;
			case OP_GETV:
				s << "*" << nodes[1].ToCode();
				break;
		}
	}
	else if (kind == FUNC) {
		s << name << "(";
		for(int i = 0; i < nodes.GetCount()-1; i++) {
			if (i) s << ", ";
			s << nodes[i].ToCode();
		}
		s << ")\n";
		s.Cat('\t', indent);
		s << nodes.Top().ToCode(indent);
	}
	else if (kind == CALL) {
		s << name << "(";
		for(int i = 0; i < nodes.GetCount(); i++) {
			if (i) s << ", ";
			s << nodes[i].ToCode();
		}
		s << ")";
	}
	else if (kind == INTLITERAL) {
		s << int_data;
	}
	else if (kind == DOUBLELITEREAL) {
		s << dbl_data;
	}
	else if (kind == STRINGLITERAL) {
		s << "\"" << str_data << "\"";
	}
	else if (kind == BLOCK || kind == SWITCHBLOCK) {
		if (int_data)
			s << "M";
		s << "{\n";
		for(int i = 0; i < nodes.GetCount(); i++) {
			s.Cat('\t', indent + 1);
			s << nodes[i].ToCode(indent + 1);
			char last = s.GetCount() ? *(s.End()-1) : 0;
			if (last != '\n' && last != ';')
				s << ";\n";
		}
		s.Cat('\t', indent);
		s << "}\n";
	}
	else if (kind == SWITCH) {
		s << "switch (" << nodes[0].ToCode() << ")\n";
		s.Cat('\t', indent);
		s << nodes.Top().ToCode(indent);
	}
	else if (kind == IF) {
		s << "if (" << nodes[0].ToCode() << ") ";
		s << nodes[1].ToCode(indent);
		if (nodes.GetCount() > 2) {
			s.Cat('\t', indent);
			s << "else " << nodes[2].ToCode(indent);
		}
	}
	else if (kind == SCOPE) {
		s << nodes.Top().ToCode(indent);
	}
	else if (kind == WHILE) {
		s << "while (" << nodes[0].ToCode() << ") ";
		s << nodes.Top().ToCode(indent);
	}
	else if (kind == FOR) {
		s << "for (" << nodes[0].ToCode() << ";" << nodes[1].ToCode() << ";" << nodes[2].ToCode() << ") ";
		s << nodes.Top().ToCode(indent);
	}
	else if (kind == RETURN) {
		s << "return";
		if (nodes.GetCount())
			s << " " << nodes.Top().ToCode(indent);
	}
	else if (kind == BREAK) {
		s << "break";
	}
	else if (kind == CASE) {
		s << "case " << nodes[0].ToCode() << ":\n";
		if (nodes.GetCount() > 1) {
			s.Cat('\t', indent);
			s << nodes[1].ToCode(indent);
		}
	}
	else if (kind == DEFAULT) {
		s << "default:\n";
		if (nodes.GetCount() > 0) {
			s.Cat('\t', indent);
			s << nodes.Top().ToCode(indent);
		}
	}
	
	return s;
}










void Parser::GetLocation(Node& n) {
	CParser::Pos pos = p.GetPos();
	n.line = pos.line;
	n.col  = pos.GetColumn();
}

void Parser::Path(String id) {
	TopScope() = Ident(id);
	bool first = true;
	while (p.IsChar('(') || p.IsChar('.') || p.IsChar('[')) {
		
		if (p.IsChar('(')) {
			if (first)		FunctionCall(id);
			else			{Op(OP_METHOD);		FunctionCall(id);		 PopScope();}
		}
		else if (p.Char('.')) {
			Op(OP_METHOD);		Factor();				PopScope();
		}
		else if (p.Char('[')) {
			Op(OP_METHOD);
			Base();
			PopScope();
			p.Char(']');
		}
		
		first = false;
	}
}

void Parser::Factor() {
	if(p.IsId()) {
		String id = p.ReadId();
		char first_char = id[0];
		
		// Variable
		if (first_char >= 'a' && first_char <= 'z') {
			Path(id);
		}
		// Class, Function
		else if (first_char >= 'A' && first_char <= 'Z') {
			Path(id);
		}
		else p.ThrowError("Unexpected character");
	}
	else if(p.Char('(')) {
		Base();
		p.PassChar(')');
	}
	else if (p.IsDouble()) {
		double d = p.ReadDouble();
		TopScope() = DoubleLiteral(d);
	}
	else if (p.IsInt()) {
		int i = p.ReadInt();
		TopScope() = IntLiteral(i);
	}
	else if (p.IsString()) {
		String s = p.ReadString();
		TopScope() = StringLiteral(s);
	}
}


void Parser::Unary() {
	Node& a = TopScope();
	if (p.IsChar('!') || p.IsChar('*') || p.IsChar('&')) {
		for(;;) {
			if      (p.Char('!')) {Op(OP_UNOT);		Factor();		PopScope();}
			else if (p.Char('*')) {Op(OP_GETV);		Factor();		PopScope();}
			else if (p.Char('&')) {Op(OP_GETA);		Factor();		PopScope();}
			else return;
		}
	} else {
		Factor();
		
		for(;;) {
			if      (p.Char2('+', '+')) {Op(OP_INC);		PopScope();}
			else if (p.Char2('-', '-')) {Op(OP_DEC);		PopScope();}
			else return;
		}
	}
}

void Parser::Type() {
	Node& a = TopScope();
	Unary();
	for(;;) {
		if (p.Char('\'')) {Op(OP_TYPE);		Unary();		PopScope();}
		else return;
	}
}

void Parser::Expo() {
	Node& a = TopScope();
	Type();
	for(;;) {
		if (p.Char2('*', '*')) {Op(OP_POW);		Type();		PopScope();}
		else return;
	}
}

void Parser::Term() {
	Expo();
	for(;;) {
		if      (p.Char('*')) {Op(OP_MUL);		Expo();		PopScope();}
		else if (p.Char('/')) {Op(OP_DIV);		Expo();		PopScope();}
		else if (p.Char('%')) {Op(OP_MOD);		Expo();		PopScope();}
		else return;
	}
}

void Parser::Expr() {
	Term();
	for(;;) {
		if      (!p.IsChar2('+','=') && p.Char('+')) {Op(OP_ADD);		Term();		PopScope();}
		else if (!p.IsChar2('-','=') && p.Char('-')) {Op(OP_SUB);		Term();		PopScope();}
		else return;
	}
}

void Parser::Shift() {
	Expr();
	for(;;) {
		if      (p.Char2('<', '<')) {Op(OP_LSHIFT);		Expr();		PopScope();}
		else if (p.Char2('>', '>')) {Op(OP_RSHIFT);		Expr();		PopScope();}
		else return;
	}
}

void Parser::Condition() {
	Shift();
	for(;;) {
		if      (p.Char2('=', '=')) {Op(OP_EQUAL);		Shift();	PopScope();}
		else if (p.Char2('!', '=')) {Op(OP_NEQUAL);		Shift();	PopScope();}
		else if (p.Char2('<', '=')) {Op(OP_LEQUAL);		Shift();	PopScope();}
		else if (p.Char2('>', '=')) {Op(OP_GEQUAL);		Shift();	PopScope();}
		else if (p.Char('<'))       {Op(OP_LESS);		Shift();	PopScope();}
		else if (p.Char('>'))       {Op(OP_GREATER);	Shift();	PopScope();}
		else return;
	}
}

void Parser::Logic() {
	Condition();
	for(;;) {
		
		if      (p.Char2('&', '&')) {Op(OP_ANDAND);		Condition();	PopScope();}
		else if (p.Char2('|', '|')) {Op(OP_OROR);		Condition();	PopScope();}
		else if (p.Char('&'))       {Op(OP_AND);		Condition();	PopScope();}
		else if (p.Char('|'))       {Op(OP_OR);			Condition();	PopScope();}
		else if (p.Char('^'))       {Op(OP_XOR);		Condition();	PopScope();}
		else return;
	}
}

void Parser::Ternary() {
	Logic();
	
	if (p.Char('?')) {
		Op(OP_INLINECOND);
		Base();
		PopScope();
		p.Char(':');
		AddScope();
		Base();
		PopScope();
	}
}

void Parser::Base() {
	Node& lhs = TopScope().IsUnknown() ? TopScope() : TopScope().Add();
	
	Ternary();
	for(;;) {
		if      (p.Char2('+', '=')) {Op(OP_PLUSEQUAL);		Ternary();	PopScope();}
		else if (p.Char2('-', '=')) {Op(OP_MINUSEQUAL);		Ternary();	PopScope();}
		else if (p.Char('='))       {Op(OP_ASSIGN);			Ternary();	PopScope();}
		else return;
	}
}

void Parser::CompilationUnit() {
	PushScope(code.root);
	TopScope() = CompilationUnitNode();
	
	while (!p.IsEof()) {
		AddScope();
		Statement();
		PopScope();
	}
	
	PopScope();
}

void Parser::Block(bool math) {
	TopScope() = BlockNode(math);
	
	p.Char('{');
	
	while (!p.IsChar('}')) {
		AddScope();
		Statement();
		PopScope();
	}
	
	
	p.Char('}');
}

void Parser::SwitchBlock() {
	Node& a = TopScope();
	ASSERT(a.IsUnknown());
	
	a = SwitchBlockNode();
	
	p.PassChar('{');
	
	while (!p.IsChar('}')) {
		
		if (p.Id("case")) {
			AddScope() = CaseNode();
			AddScope();
			Term();
			PopScope();
			PopScope();
			
			p.PassChar(':');
		}
		else if (p.Id("default")) {
			p.PassChar(':');
			AddScope() = DefaultNode();
			PopScope();
		}
		else p.ThrowError("Invalid switch block statement");

		while (!p.IsChar('}')) {
			AddScope();
			Statement();
			PopScope();
			if (p.IsId("case") || p.IsId("default"))
				break;
		}
	}
	
	p.PassChar('}');
	
}

void Parser::Statement() {
	
	char first_char = p.PeekChar();
	
	// Class, Function
	if (first_char >= 'A' && first_char <= 'Z') {
		
		String id = p.ReadId();
		
		
		bool is_ptr = p.Char('*');
		bool is_ref = p.Char('&');
		int flag = (is_ptr << 1) | (is_ref << 0);
		
		// Call
		Path(id);
		
		if (p.IsChar('{')) {
			TopScope().SetKind(FUNC);
			AddScope();
			Block();
			PopScope();
		}
	}
	else if (p.IsChar('{')) {
		Block();
	}
	else if (p.Char(';')) {
		
	}
	else if (p.Id("if")) {
		TopScope() = IfNode();
		
		p.PassChar('(');
		AddScope();
		Base();
		PopScope();
		p.PassChar(')');
		
		AddScope() = ScopeNode();
		AddScope();
		Statement();
		PopScope();
		PopScope();
		
		if (p.Id("else")) {
			AddScope() = ScopeNode();
			AddScope();
			Statement();
			PopScope();
			PopScope();
		}
	}
	else if (p.Id("while")) {
		TopScope() = WhileNode();
		
		p.PassChar('(');
		AddScope();
		Base();
		PopScope();
		p.PassChar(')');
		
		AddScope() = ScopeNode();
		AddScope();
		Statement();
		PopScope();
		PopScope();
	}
	else if (p.Id("for")) {
		TopScope() = ForNode();
		
		p.PassChar('(');
		AddScope();
		Statement();
		PopScope();
		
		AddScope();
		Base();
		PopScope();
		
		p.PassChar(';');
		AddScope();
		Base();
		PopScope();
		
		p.PassChar(')');
		
		
		AddScope() = ScopeNode();
		AddScope();
		Statement();
		PopScope();
		PopScope();
	}
	else if (p.Id("switch")) {
		TopScope() = SwitchNode();
		
		p.PassChar('(');
		AddScope();
		Base();
		PopScope();
		p.PassChar(')');
		
		AddScope() = ScopeNode();
		AddScope();
		SwitchBlock();
		PopScope();
		PopScope();
	}
	else if (p.Id("return")) {
		TopScope() = ReturnNode();
		
		if (!p.IsChar(';')) {
			AddScope();
			Base();
			PopScope();
		}
		p.PassChar(';');
	}
	else if (p.Id("continue")) {
		p.PassChar(';');
		
        CheckLegalBreak();
	}
	else if (p.IsId() || p.IsInt() || p.IsDouble() || p.IsString() || p.IsChar('-')) {
		Base();
		p.PassChar(';');
	}
	else if(p.Char('(')) {
		Base();
		p.Char(')');
	}
	else if (!p.IsEof())
		p.ThrowError("Unexpected input");
}

void Parser::CheckLegalBreak() {
	bool legal_break = false;
    for (int i = scopes.GetCount() - 1; i >= 0; i--) {
        Node& nv = *scopes[i];
        int kind = nv.GetKind();

        if (kind == FOR || kind == WHILE || kind == SWITCH) {
            legal_break = true;
            break;
        }

        if (kind == FUNC)
            break;
    }

    if (!legal_break) {
        WhenError("Cannot 'break' in this scope.");
    }
}

void Parser::Op(int i) {
	Node& a = TopScope();
	Node tmp(OP, i);
	tmp.Swap(a);
	TopScope().Add().Swap(tmp);
	AddScope();
}

void Parser::FunctionCall(String func_name) {
	TopScope() = CallNode(func_name);
	
	p.PassChar('(');
	
	if (!p.Char(')')) {
		do {
			AddScope();
			Base();
			PopScope();
		}
		while (p.Char(','));
	}
	
	p.PassChar(')');
}

CONSOLE_APP_MAIN {
	
	
	String script = LoadFile(GetDataFile("test.mo"));
	
	Parser p1;
	Code& c1 = p1.GetCode();
	p1.Set(script);
	
	try {
		p1.CompilationUnit();
		c1.Dump();
		String code1 = c1.GetPortal().ToCode();
		LOG(code1);
		
		Parser p2;
		Code& c2 = p2.GetCode();
		p2.Set(code1);
		p2.CompilationUnit();
		String code2 = c2.GetPortal().ToCode();
		
		int count = min(code2.GetLength(), code1.GetLength());
		int line = 0, col = 0;
		for(int i = 0; i < count; i++) {
			char chr1 = code1[i];
			char chr2 = code2[i];
			
			if (chr1 != chr2) {
				LOG("ERROR AT " << line << ":" << col);
				break;
			}
			col++;
			if (chr1 == '\n') {
				line++;
				col = 0;
			}
		}
	}
	catch (Exc e) {
		GetCode().Dump();
		LOG("Exception: " << e);
	}
}