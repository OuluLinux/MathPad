#include "SubMathCtrl.h"

namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;	

#define OP_BINDER(op) Node& join = node->GetOpJoinNode(op, !IsRightHandFocus());\
	join = Op(op).Add(join).Add(Void());\
	RefreshNode(&join[1]);\
	return true;



bool SubMathCtrl::Key(dword key, int count) {
	
	if (key == K_UP) {
		Node* parent = node ? node->GetInner() : 0;
		if (parent && parent->IsDivide() && node->IsEqual((*parent)[1])) {
			Cursor cur;
			cur.SetPtr(&(*parent)[0]);
			WhenNeedsAbsoluteCursor(cur);
			return true;
		}
		return false;
	}
	if (key == K_DOWN) {
		Node* parent = node ? node->GetInner() : 0;
		if (parent && parent->IsDivide() && node->IsEqual((*parent)[0])) {
			Cursor cur;
			cur.SetPtr(&(*parent)[1]);
			WhenNeedsAbsoluteCursor(cur);
			return true;
		}
		return false;
	}
	if (key == K_HOME && node) {
		Node* n = node;
		Node* par = n->GetInner();
		while (par) {
			n = par;
			par = n->GetInner();
		}
		Cursor cur;
		cur.SetPtr(n);
		WhenNeedsAbsoluteCursor(cur);
		return true;
	}
	if (key == K_END && node) {
		Node* n = node;
		Node* par = n->GetInner();
		while (par) {
			n = par;
			par = n->GetInner();
		}
		int count = n->GetCount();
		while (count) {
			n = &(*n)[count-1];
			count = n->GetCount();
		}
		Cursor cur;
		cur.SetPtr(n);
		cur.SetTextPos(1000);
		WhenNeedsAbsoluteCursor(cur);
		return true;
	}
	else if (key == (K_J|K_CTRL)) {
		Navigator nav;
		nav.SetDefault();
		nav.SetSubMathCtrl(this);
		nav.Run();
		return true;
	}
	else if (key == (K_CTRL|K_SPACE)) {
		if (GetNavigatorCount()) return true;
		
		Rect r = GetScreenRect();
		Rect r3 = GetRect();
		int pop_x = r.left;
		int pop_y = r.top + r3.GetSize().cy;
		
		Navigator& nav = NewNavigator();
		nav.SetAssist();
		nav.SetSubMathCtrl(this);
		nav.SetRect(pop_x,pop_y,640,200);
		nav.PopUp(GetTopWindow());
		return true;
	}
	
	
	int type = node ? node->GetType() : -1;
	
	
	bool alpha = false;
	bool digit = false;
		
	int chr = 0;
	bool shift = key & K_SHIFT;
	bool up = key & K_KEYUP;
	bool ctrl = key & K_CTRL;
	bool alt = key & K_ALT;
	bool skip = key & (K_CTRL | K_ALT | K_KEYUP);
	key &= ~(K_CTRL | K_ALT | K_SHIFT | K_KEYUP);
	
	
	
	if (!skip) {
		if (key >= K_A && key <= K_Z) {
			chr = 'a' + key - K_A;
			if (shift) chr += 'A' - 'a';
			alpha = true;
		}
		else if (key >= 'a' && key <= 'z') {
			chr = key;
			if (shift) chr += 'A' - 'a';
			alpha = true;
		}
		else if (key >= 'A' && key <= 'Z') {
			chr = key;
			alpha = true;
		}
		else if (key >= K_0 && key <= K_9) {
			chr = '0' + key - K_0;
			digit = true;
		}
		else if (key >= '0' && key <= '9') {
			chr = key;
			digit = true;
		}
		else if (key == K_BACKSPACE) {
			chr = -1;	
		}
		else if (key == K_DELETE) {
			chr = -2;
		}
		else if ((key == K_MINUS || key == '-') && cursor == 0) {
			chr = '-';
		}
		else if (key == '_' && type == TYPE_ID) {
			chr = '_';
			alpha = true;
		}
	}
	

	if (key == K_BACKSPACE && cursor == 0 && !up) {
		Node* par = node->GetInner();
		if (par && par->GetCount()) {
			*par = par[0];
			WhenNeedsRedraw();
			Cursor cur;
			cur.SetPtr(par);
			WhenNeedsAbsoluteCursor(cur);
			return true;
		}
	}
	
	if (!skip && (digit || alpha)  && type == TYPE_NULL) {
		String value;
		value.Cat(chr);
		
		if (alpha) {
			*node = MathCore::id(value);
		} 
		else if (digit) {
			*node = MathCore::Int(StrInt(value));
		}
		
		cursor++;
		Cursor cur;
		cur.SetPtr(node);
		cur.SetTextPos(1);
		WhenNeedsRedraw();
		WhenNeedsAbsoluteCursor(cur);
		return true;
	}
	else if (!skip && chr=='-' && type==TYPE_NULL) {
		*node = MathCore::Negate(Void());
		
		Cursor cur;
		cur.SetPtr(&node[0]);
		cur.SetTextPos(0);
		WhenNeedsRedraw();
		WhenNeedsAbsoluteCursor(cur);
		return true;
	}
	else if (!skip && HasStringCursor()) {
		String value = node->GetString();
		
		if (key == K_LEFT) {
			if (cursor <= cursor_begin || type == TYPE_FUNCTION)
				WhenCursorLeft();
			else {
				cursor--;
				WhenNeedsFullRefresh();
			}
			previous_key = key;
			return true;
		}
		else if (key == K_RIGHT) {
			if (cursor >= cursor_end || type == TYPE_FUNCTION) 
				WhenCursorRight();
			else {
				cursor++;
				WhenNeedsFullRefresh();
			}
			previous_key = key;
			return true;
		}
		else if (chr && (alpha || chr < 0) && node->IsTypeUsingString()) {
			String value = node->GetString();
			String new_value;
			if (chr > 0) {
			 	new_value = value.Left(cursor);
				new_value.Cat(chr);
				new_value += value.Mid(cursor);
			}
			else if (chr < 0) {
				if (chr == -1) {
					new_value = value.Left(cursor-1);
					new_value += value.Mid(cursor);
					cursor -= 2;
				}
				else if (chr == -2) {
					new_value = value.Left(cursor);
					new_value += value.Mid(cursor+1);
					cursor -= 1;
				}
			}
			if (new_value.GetCount()) {
				bool set_fn = false;
				if    (new_value == "abs") {*node = MathCore::Absolute(Void()); set_fn = true;}
				else if (new_value == "derive") {*node = MathCore::FunctionDerive(Void()); set_fn = true;}
				else if (new_value == "integrate") {*node = MathCore::FunctionIntegrate(Void(), Void()); set_fn = true;}
				else if (new_value == "sin") {*node = MathCore::Sin(Void()); set_fn = true;}
				else if (new_value == "cos") {*node = MathCore::Cos(Void()); set_fn = true;}
				else if (new_value == "tan") {*node = MathCore::Tan(Void()); set_fn = true;}
				else if (new_value == "asin") {*node = MathCore::ArcSin(Void()); set_fn = true;}
				else if (new_value == "acos") {*node = MathCore::ArcCos(Void()); set_fn = true;}
				else if (new_value == "atan") {*node = MathCore::ArcTan(Void()); set_fn = true;}
				else if (new_value == "sqrt") {*node = MathCore::Sqrt(Void()); set_fn = true;}
				else if (new_value == "pow") {*node = MathCore::Pow(Void(), Void()); set_fn = true;}
				//else if (new_value == "log") {node = MathCore::Log(new_value).Add(Void()); set_fn = true;}
				else if (new_value == "sum") {*node = MathCore::FunctionSum(Void(), Void(), Void(), Void()); set_fn = true;}
				else if (new_value == "limit") {*node = MathCore::FunctionLimit(Void(), Void(), Void()); set_fn = true;}
				//else if (new_value == "plusmin") {node = MathCore::Function(new_value).Add(Void()); set_fn = true;}
				else if (new_value == "cross") {*node = MathCore::CrossProduct(Void(), Void()); set_fn = true;}
				if (set_fn) {
					RefreshNode(&(*node)[0]);
					return true;
				}
				node->SetDataString(new_value);
			}
			else *node = MathCore::Void();
			cursor++;
			Cursor cur;
			cur.SetPtr(node);
			cur.SetTextPos(cursor);
			cur.Add(-1);
			WhenNeedsRedraw();
			WhenNeedsAbsoluteCursor(cur);
			if (chr == '_') {
				Cursor cur2;
				cur2.Add(-1);
				cur2.Add(2);
				WhenNeedsLogicalCursor(cur2);
			}
			return true;
		}
		else if (type == TYPE_INT || type == TYPE_DOUBLE) {
			if (chr && (digit || chr < 0)) {
				String value = node->GetString();
				String new_value;
				if (chr > 0) {
					new_value = value.Left(cursor);
					new_value.Cat(chr);
					new_value += value.Mid(cursor);
				}
				else if (chr < 0) {
					if (chr == -1) {
						new_value = value.Left(cursor-1);
						new_value += value.Mid(cursor);
						cursor -= 2;
					}
					else if (chr == -2) {
						new_value = value.Left(cursor);
						new_value += value.Mid(cursor+1);
						cursor -= 1;
					}
				}
				if (new_value.GetCount()) {
					if (type == TYPE_INT)
						node->SetInt(StrInt(new_value));
					else {
						node->SetNull();
						node->SetDouble(StrDbl(new_value));
					}
				}
				else *node = MathCore::Void();
				cursor++;
				Cursor cur;
				cur.SetPtr(node);
				cur.SetTextPos(cursor);
				cur.Add(-1);
				WhenNeedsRedraw();
				WhenNeedsLogicalCursor(cur);
				return true;
			}
			// 2x --> 2*x
			else if (chr >= 0 && alpha) {
				String value;
				value.Cat(chr);
				Node join = node->GetOpJoinNode(OP_MUL, !IsRightHandFocus());
				join = Op(OP_MUL).Add(join).Add(MathCore::id(value));
				RefreshNode(&join[1]);
				return true;
			}
		}
	} 
	
	if (!skip) {
		
		if (key == K_LEFT) {
			WhenCursorLeft();
			previous_key = key;
			return true;
		}
		else if (key == K_RIGHT) {
			WhenCursorRight();
			previous_key = key;
			return true;
		}
		
		
		key &= ~K_DELTA;
		
		if (!shift) {
				
			if (key == K_PLUS || key == '+') {
				OP_BINDER(OP_ADD);
			}
			else if (key == K_MINUS || key == '-') {
				OP_BINDER(OP_SUB);
			}
			else if (key == K_MULTIPLY || key == '*') {
				OP_BINDER(OP_MUL);
			}
			else if (key == K_DIVIDE || key == '/') {
				OP_BINDER(OP_DIV);
			}
			else if (key == '^' || key == 39) {//TODO: fix this key
				OP_BINDER(OP_POW);
			}
			else if (key == '=') {
				Node* par = node ? node->GetInner() : 0;
				int par_type = par ? par->GetType() : 0;
				int par_op = par_type == TYPE_OP ? par->GetInt() : 0;
				if (par_type == TYPE_OP && par_op == OP_LESS) {
					par->SetOp(OP_LSEQ);
					WhenNeedsRedraw();
					return true;
				} else if (par_type == TYPE_OP && par_op == OP_GRT) {
					par->SetOp(OP_GREQ);
					WhenNeedsRedraw();
					return true;
				} else {
					OP_BINDER(OP_EQ);
				}
			}
			else if (key == '!') {
				OP_BINDER(OP_INEQ);
			}
			else if (key == '<') {
				OP_BINDER(OP_LESS);
			}
			else if (key == '>') {
				OP_BINDER(OP_GRT);
			}
			else if (key == ':') {
				OP_BINDER(OP_ASSIGN);
			}
			#undef OP_BINDER
		} else {
			key &= ~K_DELTA;
			
			#define OP_BINDER(op) Node* join = node;\
				*join = Op(op).Add(*join).Add(Void());\
				RefreshNode(&((*join)[1]));\
				return true;
				
			if (key == K_PLUS || key == '+') {
				OP_BINDER(OP_ADD);
			}
			else if (key == K_MINUS || key == '-') {
				OP_BINDER(OP_SUB);
			}
			else if (key == K_MULTIPLY || key == '*') {
				OP_BINDER(OP_MUL);
			}
			else if (key == K_DIVIDE || key == '/') {
				OP_BINDER(OP_DIV);
			}
			else if (key == '^') {
				OP_BINDER(OP_POW);
			}
			
			#undef OP_BINDER
			
		}
		
		if (key == '(') {
			if (node->GetType() == TYPE_ID && node->GetString().GetCount()) {
				SubMathCtrl voidnode;
				*node = MathCore::Function(node->GetString()).Add(Void());
				RefreshNode(&(*node)[0]);
			} else {
				*node = Parenthesis().Add(*node);
				RefreshNode(&(*node)[0]);
			}
			return true;
		}
		else if (key == '[') {
			*node = MathCore::Array().Add(*node);
			RefreshNode(&(*node)[0]);
			return true;
		}
		else if (key == '{') {
			*node = MathCore::Set().Add(*node);
			RefreshNode(&(*node)[0]);
			return true;
		}
		else if (key == ',') {
			Node* par = node ? node->GetInner() : 0;
			if (par && par->GetOpLevel() == 4) {
				int pos = -1;
				for(int i = 0; i < par->GetCount(); i++) {
					Node cur = par[i];
					if (cur.IsEqual(*node)) {
						pos = i+1;
						break;
					}
				}
				if (pos == -1) pos = par->GetCount();
				par->Insert(pos, Void());
				Node* fn_node = &(*node)[pos];
				Cursor cur;
				cur.SetPtr(fn_node);
				WhenNeedsRedraw();
				WhenNeedsAbsoluteCursor(cur);
				return true;
			}
		}
	}
	
	return false;
}

void SubMathCtrl::RefreshNode(Node* cursor_dest) {
	
	Cursor cur;
	cur.SetPtr(cursor_dest);
	
	SetDefaultWantFocus(false);
	cur.SetTextPos(0);
	WhenNeedsRedraw();
	WhenNeedsAbsoluteCursor(cur);
}



SubMathCtrl::Cursor SubMathCtrl::GetCursor() const {
	SubMathCtrl::Cursor out;
	GetCursorLoop(out);
	return out;
}

void SubMathCtrl::GetCursorLoop(SubMathCtrl::Cursor& cursor) const {
	bool has_cursor = HasFocusDeep();
	if (!has_cursor)
		return;
	for(int i = 0; i < nodes.GetCount(); i++) {
		const SubMathCtrl& n = nodes[i];
		bool node_has_cursor = n.HasFocusDeep();
		if (node_has_cursor) {
			cursor.Add(i);
			n.GetCursorLoop(cursor);
			return;
		}
	}
	cursor.SetTextPos(this->cursor);
	cursor.SetPtr(node);
	cursor.SetSubMathCtrl((SubMathCtrl*)this);
}

void SubMathCtrl::SetCursor(const SubMathCtrl::Cursor& cursor) {
	SetCursor(cursor, 0);
}

void SubMathCtrl::SetCursor(const SubMathCtrl::Cursor& cursor, int pos) {
	if (pos >= cursor.GetCount()) {
		Node* ptr = cursor.GetPtr();
		if (ptr && !ptr->IsEqual(*node)) {
			IteratorDeep it = BeginDeep();
			for (;!it.IsEnd(); it++) {
				SubMathCtrl& mc = *it;
				if (mc.node->IsEqual(*ptr)) {
					Cursor cur;
					cur.SetTextPos(cursor.GetTextPos());
					mc.SetCursor(cur);
					return;
				}
			}
		}
		
		if (!node || !IsDefaultWantFocus()) {
			for(int i = 0; i < nodes.GetCount(); i++) {
				SubMathCtrl& n = nodes[i];
				if (n.nodes.GetCount() || (n.node && n.IsDefaultWantFocus())) {
					Cursor cur;
					cur.SetTextPos(cursor.GetTextPos());
					n.SetCursor(cur);
					return;
				}
			}
			return;
		}
		
		this->cursor = cursor.GetTextPos();
		if (this->cursor > cursor_end)
			this->cursor = cursor_end;
		if (this->cursor < cursor_begin)
			this->cursor = cursor_begin;
		
		SetFocus();
	} else {
		int i = cursor[pos];
		if (i >= 0 && i < nodes.GetCount()) {
			nodes[i].SetCursor(cursor, pos+1);
		}
	}
}

bool SubMathCtrl::HasStringCursor() {
	if (!node) return false;
	int type;
	return node->IsTypeUsingString() || (type=node->GetType()) == TYPE_INT || type == TYPE_DOUBLE;
}

void SubMathCtrl::EnableSubFocus(bool b) {
	if (b == false) {
		WantFocus(false);
	} else {
		WantFocus(default_wantfocus);
	}
	for(int i = 0; i < nodes.GetCount(); i++) {
		nodes[i].EnableSubFocus(b);
	}
}

void SubMathCtrl::FocusSub(int i) {
	EnableSubFocus(true);
	
	if (i >= 0 && i < nodes.GetCount()) {
		SubMathCtrl& mnode = nodes[i];
		if (!mnode.IsDefaultWantFocus()) {
			if (i < mnode.nodes.GetCount() && mnode.nodes[i].IsDefaultWantFocus()) {
				mnode.nodes[i].SetFocus();
			}
			return;
		}
		mnode.SetFocus();
	}
}

void SubMathCtrl::GotFocus() {
	if (previous_key) {
		if (previous_key == K_LEFT && HasStringCursor()) {
			String value = node->GetString();
			cursor = cursor_end;
		}
		else if (previous_key == K_RIGHT) {
			cursor = cursor_begin;
		}
		previous_key = 0;
	}
	Refresh();
}



}
