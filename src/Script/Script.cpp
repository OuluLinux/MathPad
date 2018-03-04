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
	out << name << ": " << GetNodeHeaderString(kind, int_data, dbl_data, str_data);
	for(int i = 0; i < nodes.GetCount(); i++)
		NodeAsString(out, nodes[i].GetKey(), &nodes[i], indent, stack);
	stack->Remove(stack->GetCount()-1);
	if (delete_stack) {
		delete stack;
	}
	return out;
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
			while (p.IsChar(':')) {
				p.PassChar2(':', ':');
				id << "::" << p.ReadId();
			}
			if(p.IsChar('('))
				Path(id);
			else
				TopScope() = FunctionRefNode(id);
		}
		else p.ThrowError("Unexpected character");
	}
	else if(p.Char('(')) {
		Ternary();
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
	if (p.IsChar('!')) {
		for(;;) {
			if (p.Char('!')) {Op(OP_UNOT);		Factor();		PopScope();}
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
	if (is_mathmode) {
		for(;;) {
			if (p.Char('\'')) {Op(OP_TYPE);		Unary();		PopScope();}
			else return;
		}
	}
}

void Parser::Expo() {
	Node& a = TopScope();
	Type();
	if (is_mathmode) {
		for(;;) {
			if (p.Char2('*', '*')) {Op(OP_EXP);		Type();		PopScope();}
			else return;
		}
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
		if      (p.Char('+')) {Op(OP_ADD);		Term();		PopScope();}
		else if (p.Char('-')) {Op(OP_SUB);		Term();		PopScope();}
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
		if      (p.Char('&'))       {Op(OP_AND);		Condition();	PopScope();}
		else if (p.Char('|'))       {Op(OP_OR);			Condition();	PopScope();}
		else if (p.Char('^'))       {Op(OP_XOR);		Condition();	PopScope();}
		else if (p.Char2('&', '&')) {Op(OP_ANDAND);		Condition();	PopScope();}
		else if (p.Char2('|', '|')) {Op(OP_OROR);		Condition();	PopScope();}
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
	PushScope(GetCode().root);
	
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
			AddScope();
			Term();
			PopScope();
			
			p.PassChar(':');
		}
		else if (p.Id("default")) {
			p.PassChar(':');
			
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
	
	if (p.IsChar2('M','{')) {
		p.Char('M');
		is_mathmode++;
		Block(true);
		is_mathmode--;
	}
	
	// Class, Function
	else if (first_char >= 'A' && first_char <= 'Z') {
		
		String id = p.ReadId();
		
		while (p.IsChar(':')) {
			p.PassChar2(':', ':');
			id << "::" << p.ReadId();
		}
		
		// Call
		if(p.IsChar('(')) {
			Path(id);
		}
		// Function declaring
		else if (p.IsId()) {
			String id2 = p.ReadId();
			
			char first_char2 = id2[0];
			
			// Declare variable
			if (first_char2 >= 'a' && first_char2 <= 'z') {
				for(;;) {
					TopScope() = VariableNode(id, id2);
					if (p.Char('=')) {
						AddScope();
						Base();
						PopScope();
					}
					if (!p.Char(',')) break;
					PopScope();
					AddScope();
					id2 = p.ReadId();
				}
				p.Char(';');
			}
			// Declare Function
			else if (first_char2 >= 'A' && first_char2 <= 'Z') {
				ParseFunctionDefinition(id, id2);
			}
			
			else p.ThrowError("Unexpected input");
		}
		// Class, Function reference
		else {
			TopScope() = FunctionRefNode(id);
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
	else if (p.IsId("class")) {
		ParseClassDefinition();
	}
	else if (p.IsId("try")) {
		ParseTryDefinition();
	}
	else if (p.Id("break")) {
		TopScope() = BreakNode();
		
		p.PassChar(';');
		
        CheckLegalBreak();
	}
	else if (p.Id("continue")) {
		p.PassChar(';');
		
        CheckLegalBreak();
	}
	else if (p.Id("using")) {
		TopScope() = UsingNode();
		AddScope();
		Base();
		PopScope();
	}
	else if (p.Id("namespace")) {
		TopScope() = NamespaceNode();
		AddScope();
		Base();
		PopScope();
		AddScope();
		Block();
		PopScope();
		
	}
	else if (p.IsId() || p.IsInt() || p.IsDouble() || p.IsString() || p.IsChar('-')) {
		Base();
		p.PassChar(';');
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

        if (kind == FUNC || kind == CLASS)
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

void Parser::ParseFunctionDefinition(String ret_type, String func_name) {
	TopScope() = FunctionNode(func_name);
	
	ParseFunctionArguments();
	
	AddScope();
	Block();
	PopScope();
	
}

void Parser::ParseTryDefinition() {
	p.PassId("try");
	TopScope() = TryNode();
	
	AddScope();
	Statement();
	PopScope();
	
	if (p.Id("catch")) {
		AddScope();
		ParseFunctionArguments();
		AddScope();
		Statement();
		PopScope();
		PopScope();
	}
}

void Parser::ParseClassDefinition() {
	p.PassId("class");
	String cls_name = p.ReadId();
	
	TopScope() = ClassNode(cls_name);
	
	AddScope();
	Block();
	PopScope();
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

void Parser::ParseFunctionArguments() {
	p.PassChar('(');
	
	while (!p.IsChar(')')) {
		AddScope();
		String type = p.ReadId();
		String id = p.ReadId();
		TopScope() = VariableNode(type, id);
		if (!p.IsChar(')'))
			p.PassChar(',');
		PopScope();
	}
	
	p.PassChar(')');
}


CONSOLE_APP_MAIN {
	
	String script = LoadFile(GetDataFile("test.oct"));
	
	Parser p;
	p.Set(script);
	
	try {
		p.CompilationUnit();
		GetCode().Dump();
	}
	catch (Exc e) {
		GetCode().Dump();
		LOG("Exception: " << e);
	}
	
	/*CParser p(script);
	try {
		String id;
		if(p.IsId()) {
			CParser::Pos pos = p.GetPos();
			id = p.ReadId();
			if(p.Char('=')) {
				double x = Exp(p);
				Cout() << id << " <- " << x << '\n';
				var.GetAdd(id) = x;
				goto done;
			}
			p.SetPos(pos);
		}
		Cout() << Exp(p) << '\n';
	done:;
	}
	catch(CParser::Error e) {
		Cout() << "ERROR: " << e << '\n';
	}*/
}