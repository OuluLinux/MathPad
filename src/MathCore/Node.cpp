#include "MathCore.h"
#include "XcasGiac.h"
#include "Units.h"

namespace MathCore {

Node::IteratorDeep::IteratorDeep(Node* cur) {
	this->cur.Add(cur);
	pos.Add(0);
}

Node::IteratorDeep::IteratorDeep(const Node* cur) {
	this->cur.Add((Node*)cur);
	pos.Add(0);
}

bool Node::IteratorDeep::operator == (IteratorDeep& it) {
	return GetCurrentAddr() == it.GetCurrentAddr();
}

void Node::IteratorDeep::operator ++(int i) {
	#define LASTPOS pos[pos.GetCount()-1]
	#define LASTCUR cur[cur.GetCount()-1]
	#define SECLASTCUR cur[cur.GetCount()-2]
	#define ADDPOS pos[pos.GetCount()-1]++
	#define ADDCUR LASTCUR = &SECLASTCUR->At(LASTPOS)
	#define REMLASTPOS pos.Remove(pos.GetCount()-1)
	#define REMLASTCUR cur.Remove(cur.GetCount()-1)
	//String s; for(int i = 0; i < pos.GetCount(); i++) s << " " << pos[i]; LOG("+++ " << s);
	
	if (pos.GetCount() == 1 && pos[0] < 0) {pos[0]++; return;}
	if (pos.GetCount() == 1 && pos[0] == 1) return; // at end
	
	if (LASTCUR->GetCount()) {
		pos.Add(0);
		cur.Add(&LASTCUR->At(0));
	}
	else if (pos.GetCount() == 1) {
		pos[0] = 1; // at end
	}
	else {
		
		while (1) {
			if (LASTPOS + 1 < SECLASTCUR->GetCount()) {
				ADDPOS;
				ADDCUR;
				break;
			} else {
				REMLASTPOS;
				REMLASTCUR;
				if (pos.GetCount() <= 1) {
					pos.SetCount(1);
					pos[0] = 1;
					break;
				}
			}
		}
	}
}

void Node::IteratorDeep::IncNotDeep() {
	if (LASTCUR->GetCount()) {
		while (1) {
			if (cur.GetCount() >= 2 && LASTPOS + 1 < SECLASTCUR->GetCount()) {
				ADDPOS;
				ADDCUR;
				break;
			} else {
				REMLASTPOS;
				REMLASTCUR;
				if (pos.GetCount() <= 1) {
					pos.SetCount(1);
					pos[0] = 1;
					break;
				}
			}
		}
	} 
	else operator++(1);
}

void Node::IteratorDeep::DecToInner() {
	pos.Remove(pos.GetCount()-1);
	cur.Remove(cur.GetCount()-1);
}

Node* Node::IteratorDeep::operator->() {
	if (pos.GetCount() && pos[0] == 1) return 0;
	return &*LASTCUR;
}

Node& Node::IteratorDeep::operator*() {
	if (pos.GetCount() && pos[0] == 1)
		throw Exc("Iterator at end");
	return *LASTCUR;
}

Node* Node::IteratorDeep::Inner() {
	if (cur.GetCount() <= 1) return 0;
	return &*cur[cur.GetCount()-2];
}


Node::IteratorDeepest::IteratorDeepest(IteratorDeep deep) {
	// Basically just gets all nodes and sorts them by depth
	int i = 0;
	for (;!deep.IsEnd(); deep++) {
		OneNode& o = nodes.Add();
		o.depth = deep.GetDepth();
		o.ptr = &*deep;
		o.pos = i;
		//LOG(i << " " << (int64)o.ptr << " " << o.depth << " " << deep->AsInlineString());
		i++;
	}
	Sort(nodes, OneNodeSorter());
	it = nodes.Begin();
}

void Node::IteratorDeepest::operator ++(int i) {
	it++;
}

Node* Node::IteratorDeepest::operator->() {
	return &*it->ptr;
}

Node& Node::IteratorDeepest::operator*() {
	return *it->ptr;
}

Node::IteratorDeepest Node::BeginDeepest() {
	return IteratorDeepest(BeginDeep());
}

Node::IteratorDeep Node::BeginDeep() {
	return IteratorDeep(this);
}

const Node::IteratorDeep Node::BeginDeep() const {
	return IteratorDeep(this);
}

Node::Iterator Node::Begin() {
	return nodes.Begin();
}

Node::Iterator Node::End() {
	return nodes.End();
}



		
uint32 Node::GetHashValue() const {
	CombineHash ch;
	ch << int_data << 1 << ::GetHashValue(dbl_data) << 1 << str_data.GetHashValue();
	for(int i = 0; i < nodes.GetCount(); i++) {
		const Node& m = nodes[i];
		ch << m.GetHashValue() << 1;
	}
	return ch;
}

void Node::operator=(const Node& n) {
	SetNull();
	int_data = n.int_data;
	dbl_data = n.dbl_data;
	str_data = n.str_data;
	type = n.type;
	
	nodes.Clear();
	for(int i = 0; i < n.nodes.GetCount(); i++) {
		nodes.Add(n.nodes[i]);
	}
}

bool Node::IsEqual(const Node& n) {
	if (n.nodes.GetCount() != nodes.GetCount())
		return false;
	if (int_data != n.int_data ||
		dbl_data != n.dbl_data ||
		str_data != n.str_data ||
		type != n.type)
		return false;
	for(int i = 0; i < nodes.GetCount(); i++) {
		if (!(nodes[i].IsEqual(n.AtConst(i))))
			return false;
	}
	return true;
}

/*Node* Node::GetInner() {
	return inner;
}*/

Node& Node::operator[](int i) {
	if (i < 0 || i >= nodes.GetCount())
		throw Exc("Node[] error");
	return nodes[i];
}

Node& Node::At(int i) {
	if (i < 0 || i >= nodes.GetCount())
		throw Exc("Node::At(i) error");
	return nodes[i];
}

const Node& Node::AtConst(int i) const {
	if (i < 0 || i >= nodes.GetCount())
		throw Exc("Node::At(i) error");
	return nodes[i];
}

Node Node::CopyWithoutNodes() {
	Node n;
	n.int_data = int_data;
	n.dbl_data = dbl_data;
	n.str_data = str_data;
	n.type = type;
	return n;
}
/*
Node Node::DeepCopy() {
	Node n;
	n.int_data = int_data;
	n.dbl_data = dbl_data;
	n.str_data = str_data;
	n.type = type;
	for(int i = 0; i < nodes.GetCount(); i++) {
		n.nodes.Add(nodes[i].DeepCopy());
	}
	return n;
}
*/
Node& Node::Add() {
	nodes.Add();
	return *this;
}

Node& Node::Add(const Node& m) {
	nodes.Add(m);
	return *this;
}

Node& Node::Insert(int pos, Node n) {
	nodes.Insert(pos, n);
	return *this;
}

/*Node Node::Print(String title) {
	
}

Node Node::Print(const char* title) {
	
}

Node Node::Print(bool lower) {
	
}

Node Node::Print(String title, bool lower) {
	
}*/

Node Node::Floating() {
	if (IsMultiple()) {
		for(int i = 0; i < nodes.GetCount(); i++)
			nodes[i].Floating();
	}
	else *this = MathCore::Floating(*this);
	return *this;
}

Node Node::Solve() {
	if (IsMultiple()) {
		for(int i = 0; i < nodes.GetCount(); i++)
			nodes[i].Solve();
	}
	else *this = MathCore::Solve(*this);
	return *this;
}

Node Node::Solve(Node key, bool keep_key_equality) {
	if (IsMultiple()) {
		for(int i = 0; i < nodes.GetCount(); i++)
			nodes[i].Solve(key, false);
	}
	else *this = MathCore::SolveVar(*this, key);
	if (keep_key_equality) {
		*this = key == *this;
	}
	return *this;
}

Node Node::SolveComplex() {
	if (IsMultiple()) {
		for(int i = 0; i < nodes.GetCount(); i++)
			nodes[i].SolveComplex();
	}
	else *this = MathCore::SolveComplex(*this);
	return *this;
}

Node Node::SolveComplex(Node key, bool keep_key_equality) {
	if (IsMultiple()) {
		for(int i = 0; i < nodes.GetCount(); i++)
			nodes[i].SolveComplex(key, false);
	}
	else *this = MathCore::SolveComplexVar(*this, key);
	if (keep_key_equality) {
		*this = key == *this;
	}
	return *this;
}

Node Node::FSolve() {
	if (IsMultiple()) {
		for(int i = 0; i < nodes.GetCount(); i++)
			nodes[i].FSolve();
	}
	else *this = MathCore::FSolve(*this);
	return *this;
}

Node Node::FSolve(Node key, bool keep_key_equality) {
	if (IsMultiple()) {
		for(int i = 0; i < nodes.GetCount(); i++)
			nodes[i].FSolve(key, false);
	}
	else *this = MathCore::FSolveVar(*this, key);
	if (keep_key_equality) {
		*this = key == *this;
	}
	return *this;
}

Node Node::Simplify() {
	if (IsOp() && GetInt() == OP_EQ && nodes.GetCount() == 2) {
		if (nodes[0].IsFuncSqrt() && nodes[1].IsFuncSqrt()) {
			if (nodes[0].GetCount() == 1 && nodes[1].GetCount() == 1) {
				nodes[0] = nodes[0].At(0);
				nodes[1] = nodes[1].At(0);
				return Simplify();
			}
		}
		Node tmp = Op(OP_EQ);
		tmp.Add(Subtract(nodes[0], nodes[1]));
		tmp.Add(Int(0));
		tmp.At(0).Simplify();
		*this = tmp;
	}
	if (IsMultiple()) {
		for(int i = 0; i < nodes.GetCount(); i++) {
			nodes[i].Simplify();
		}
	} else {
		*this = MathCore::Factor(MathCore::Simplify(*this));
	}
	return *this;
}

Node Node::Calculate() {
	for(int i = 0; i < nodes.GetCount(); i++) {
		nodes[i].Calculate();
	}
	
	if (type == TYPE_UNARY && nodes.GetCount() == 1) {
		Node n0 = nodes[0];
		if (int_data == UNARY_NEG) {
			if (n0.IsInt()) {
				int i = n0.GetInt();
				i *= -1;
				*this = Int(i);
			}
			else if (n0.IsDouble()) {
				double d = n0.GetDouble();
				d *= -1;
				*this = Double(d);
			}
			else {
				*this = Multiply(Int(-1), n0);
				return Calculate();
			}
		}
	}
	else if (type == TYPE_OP) {
		if (nodes.GetCount() == 1) {
			nodes.Insert(0) = Void();
		}
		Node n0 = nodes[0];
		Node n1 = nodes[1];
		if (int_data == OP_ADD) {
			// 1+1 --> 2
			if (n0.IsInt() && n1.IsInt())
				*this = Int(n0.GetInt() + n1.GetInt());
			// [1,1] + [2,2] --> [3,3]
			else if (n0.IsArray() && n1.IsArray() && n0.GetCount() == n1.GetCount() ) {
				Node n = Array();
				for(int i = 0; i < n1.GetCount(); i++)
					n.Add(MathCore::Add(n0.At(i), n1.At(i)));
				n.Calculate();
				*this = n;
			}
			else if (n0.IsNull())
				*this = n1;
			else if (n1.IsNull())
				*this = n0;
			// a + (-1)*b --> a - b
			else if (n1.IsMultiply() && n1.At(0).IsInt() && n1.At(0).GetInt() == -1) {
				int_data = OP_SUB;
				n1 = n1.At(1);
			}
			// a + b*(-1) --> a - b
			else if (n1.IsMultiply() && n1.At(1).IsInt() && n1.At(1).GetInt() == -1) {
				int_data = OP_SUB;
				n1 = n1.At(0);
			}
			// a + (-2)*b --> a - 2*b
			else if (n1.IsMultiply() && n1.At(0).IsInt() && n1.At(0).GetInt() < 0) {
				int_data = OP_SUB;
				n1.At(0) = Int(-1 * n1.At(0).GetInt());
			}
			// a + b*(-2) --> a - b*2
			else if (n1.IsMultiply() && n1.At(1).IsInt() && n1.At(1).GetInt() < 0) {
				int_data = OP_SUB;
				n1.At(1) = Int(-1 * n1.At(1).GetInt());
			}
			// 0 + x --> x
			else if (n0.IsInt() && n0.GetInt() == 0)
				*this = n1;
			// x + 0 --> x
			else if (n1.IsInt() && n1.GetInt() == 0)
				*this = n0;
		}
		else if (int_data == OP_SUB) {
			// 2 - 1 --> 1
			if (n0.IsInt() && n1.IsInt())
				*this = Int(n0.GetInt() - n1.GetInt());
			// [2,2]-[1,1] = [1,1]
			else if (n0.IsArray() && n1.IsArray() && n0.GetCount() == n1.GetCount() ) {
				Node n = Array();
				for(int i = 0; i < n1.GetCount(); i++)
					n.Add(Subtract(n0.At(i), n1.At(i)));
				n.Calculate();
				*this = n;
			}
			else if (n0.IsNull())
				n0 = Int(0);
			else if (n1.IsNull())
				*this = n0;
			// x - 0 --> x
			else if (n1.IsInt() && n1.GetInt() == 0)
				*this = n0;
		}
		else if (int_data == OP_MUL) {
			// 2 * 1
			if (n0.IsInt() && n1.IsInt())
				*this = Int(n0.GetInt() * n1.GetInt());
			else if (n0.IsInt() && n1.IsArray()) {
				Node n = Array();
				for(int i = 0; i < n1.GetCount(); i++)
					n.Add(Multiply(n0, n1.At(i)));
				n.Calculate();
				*this = n;
			}
			// 1/2 * a --> a / 2
			else if (n0.IsDivide() && n0.At(0).IsInt() && n0.At(0).GetInt() == 1) {
				*this = Divide(n1, n0.At(1));
			}
			// a * 1/2 --> a / 2
			else if (n1.IsDivide() && n1.At(0).IsInt() && n1.At(0).GetInt() == 1) {
				*this = Divide(n0, n1.At(1));
			}
			// 0 * a --> 0
			else if (n0.IsInt() && n0.GetInt() == 0)
				*this = Int(0);
			// a * 0 --> 0
			else if (n1.IsInt() && n1.GetInt() == 0)
				*this = Int(0);
			
			// ...not same as 0. Something is just erased.
			else if (n0.IsNull())
				*this = n1;
			else if (n1.IsNull())
				*this = n0;
			
		}
		else if (int_data == OP_DIV) {
			// 0/2 --> 0
			if (n0.IsInt() && n0.GetInt() == 0)
				*this = Int(0);
			// 4/2 --> 2 but NOT  1/2 --> 0.5
			else if (n0.IsInt() && n1.IsInt()) {
				int a = n0.GetInt();
				int b = n1.GetInt();
				if (b != 0 && a % b == 0)
					*this = Int(a / b);
			}
			
			// ...not same as 0. Something is just erased.
			else if (n0.IsNull())
				*this = Int(1);
			else if (n0.IsFunction() && n1.IsFunction()) {
				if (n0.GetInt() == FUNC_ASIN && n1.GetInt() == FUNC_ACOS) {
					*this = Function(FUNC_ATAN).Add(Divide(n1.At(0), n0.At(0)));
					Calculate();
				}
				else if (n0.GetInt() == FUNC_ACOS && n1.GetInt() == FUNC_ASIN) {
					*this = Function(FUNC_ATAN).Add(Divide(n0.At(0), n1.At(0)));
					Calculate();
				}
			}
			else if (n0.GetInt() == OP_DIV && n1.GetInt() == OP_DIV) {
				*this = Divide(n0.At(0) * n1.At(1), n1.At(0) * n0.At(1));
				Calculate();
			}
		}
		else if (int_data == OP_POW) {
			// 0^2 --> 0
			if (n0.IsInt() && n0.GetInt() == 0)
				*this = Int(0);
			// 3^2 --> 9
			else if (n0.IsInt() && n1.IsInt())
				*this = Int(pow((double)n0.GetInt(), n1.GetInt()));
			
			// ...not same as 0. Something is just erased.
			else if (n0.IsNull() || n1.IsNull())
				*this = Void();
			// x^0 --> 1
			else if (n1.IsInt() && n1.GetInt() == 0)
				*this = Int(1);
			
		}
		
		else if (int_data == OP_EQ) {
			// sqrt(2) == sqrt(x) --> 2 = x
			if (n0.IsFunction() && n1.IsFunction()) {
				if (n0.int_data == FUNC_SQRT && n1.int_data == FUNC_SQRT) {
					n0 = n0.At(0);
					n1 = n1.At(0);
				}
				else if (IsExprTrigFuncSinCos(n0) && n0.int_data == n1.int_data && HasVar(n0) && HasVar(n1) ) {
					n0 = Function(FUNC_PLUSMIN).Add(n0.At(0)) + Int(2) * MathCore::id("n") * Pi();
					n1 = n1.At(0);
				}
			}
		}
	}
	else if (type == TYPE_FUNCTION) {
		// abs(-2) --> 2
		if (int_data == FUNC_ABS) {
			if (nodes[0].IsInt())
				*this = Int(abs(nodes[0].GetInt()));
		}
		// pow(3,2) --> 9
		else if (int_data == FUNC_POW) {
			if (nodes[0].IsInt() && nodes[1].IsInt())
				*this = Int(pow((double)nodes[0].GetInt(), nodes[1].GetInt()));
			else if (nodes[1].IsInt() && nodes[1].GetInt() == 0)
				*this = Int(1);
		}
		else if (int_data == FUNC_SQRT) {
			if (nodes[0].IsInt()) {
				int i = nodes[0].GetInt();
				if (i >= 0) {
					int ans = sqrt((double)i);
					if (ans * ans == i)
						*this = Int(ans);
				}
			}
		}
		else if (int_data == FUNC_LOG) {
			if (nodes.GetCount() == 2) {
				*this = Function(FUNC_LOG).Add(nodes[0]) / Function(FUNC_LOG).Add(nodes[1]);
			}
		}
		else if (int_data == FUNC_SIN) {
			Node n0 = nodes[0];
			if (n0.IsFunction() && n0.GetInt() == FUNC_ASIN)
				*this = n0.At(0);
		}
		else if (int_data == FUNC_COS) {
			Node n0 = nodes[0];
			if (n0.IsFunction() && n0.GetInt() == FUNC_ACOS)
				*this = n0.At(0);
		}
		
	}
	else if (type == TYPE_ARRAY) {
		if (GetCount() == 1) 
			*this = At(0);
	}
	return *this;
}

Node Node::Factor() {
	*this = MathCore::Factor(*this);
	return *this;
}

Node Node::TypeSimplify() {
	if (IsOp() && GetInt() == OP_EQ && nodes.GetCount() == 2) {
		if (nodes[0].IsFuncSqrt() && nodes[1].IsFuncSqrt()) {
			if (nodes[0].GetCount() == 1 && nodes[1].GetCount() == 1) {
				nodes[0] = nodes[0].At(0);
				nodes[1] = nodes[1].At(0);
				return TypeSimplify();
			}
		}
		Node tmp = Op(OP_EQ);
		tmp.Add(Subtract(nodes[0], nodes[1]));
		tmp.Add(Int(0));
		tmp.At(0).TypeSimplify();
		*this = tmp;
	}
	if (IsMultiple()) {
		for(int i = 0; i < nodes.GetCount(); i++) {
			nodes[i].TypeSimplify();
		}
	} else {
		*this = MathCore::TypeSimplify(*this);
	}
	return *this;
}

Node Node::Parse(const String& expr) {
	*this = ParseExpression(expr);
	return *this;
}

Node Node::Replace(Node& find, const Node replacing) {
	Node tmp(find);
	if (tmp.Compare(replacing) == 0) return *this;
	
	bool mul_find = find.GetInt() == OP_MUL;
	
	IteratorDeep it = BeginDeep();
	for (; !it.IsEnd();) {
		if (it->Compare(find) == 0) {
			*it = replacing;
			// Not replacing inside replacing.
			it.IncNotDeep();
		}
		else if (mul_find) {
			if(it->GetInt() == OP_MUL) {
				if (it->At(0).GetInt() == OP_MUL) {
					if ((it->At(0).At(0).Compare(find.At(0)) == 0 && it->At(1).Compare(find.At(1)) == 0) ||
						(it->At(0).At(0).Compare(find.At(1)) == 0 && it->At(1).Compare(find.At(0)) == 0)) {
						it->At(0).At(0) = replacing;
						it->At(1) = Int(1);
						it.IncNotDeep();
					}
					else if ((it->At(0).At(1).Compare(find.At(0)) == 0 && it->At(1).Compare(find.At(1)) == 0) ||
						(it->At(0).At(1).Compare(find.At(1)) == 0 && it->At(1).Compare(find.At(0)) == 0)) {
						it->At(0).At(1) = replacing;
						it->At(1) = Int(1);
						it.IncNotDeep();
					}
					else it++;
				}
				else if (it->At(1).GetInt() == OP_MUL) {
					if ((it->At(1).At(0).Compare(find.At(0)) == 0 && it->At(0).Compare(find.At(1)) == 0) ||
						(it->At(1).At(0).Compare(find.At(1)) == 0 && it->At(0).Compare(find.At(0)) == 0)) {
						it->At(1).At(0) = replacing;
						it->At(0) = Int(1);
						it.IncNotDeep();
					}
					else if ((it->At(1).At(1).Compare(find.At(0)) == 0 && it->At(0).Compare(find.At(1)) == 0) ||
						(it->At(1).At(1).Compare(find.At(1)) == 0 && it->At(0).Compare(find.At(0)) == 0)) {
						it->At(1).At(1) = replacing;
						it->At(0) = Int(1);
						it.IncNotDeep();
					}
					else it++;
				}
				else it++;
			}
			else it++;
		}
		else
			it++;
		
	}
	return *this;
}

Node Node::Replace(Node& equation) {
	ASSERT(equation.IsOp() && equation.IsOp(OP_EQ));
	return Replace(equation.At(0), equation.At(1));
}

Node Node::Remove(Node* node) {
	IteratorDeep it = BeginDeep();
	for (; !it.IsEnd(); it++) {
		Node& n = *it;
		for(int i = 0; i < n.GetCount(); i++) {
			Node& ptr = n.At(i);
			if ((int64)&ptr == (int64)node) {
				n.Remove(i);
			}
		}
	}
	return *this;
}

Node Node::Remove(int i) {
	nodes[i] = Void();
	Calculate();
	return *this;
}

Node Node::Remove(const Node& n) {
	const Node *ptr = &n;
	if ((int64)ptr == (int64)this) {
		*this = Void();
		return *this;
	}
	IteratorDeep it = BeginDeep();
	for (; !it.IsEnd();) {
		if (it->Compare(n) == 0) {
			*it = Void();
			it.IncNotDeep();
		}
		else
			it++;
	}
	Calculate();
	return *this;
}

Node Node::RemoveIds() {
	if (type == TYPE_ID)
		*this = Void();
	else {
		for(int i = 0; i < nodes.GetCount(); i++) {
			nodes[i].RemoveIds();
		}
	}
	return *this;
}

Node Node::RemoveAllExceptOpId() {
	if (type != TYPE_ID && type != TYPE_OP && type != TYPE_UNARY)
		*this = Void();
	else if (type == TYPE_OP && nodes[0].IsInt() && nodes[1].IsInt()) {
		*this = Void();
	}
	else {
		for(int i = 0; i < nodes.GetCount(); i++) {
			nodes[i].RemoveAllExceptOpId();
		}
	}
	return *this;
}

Node Node::RemoveTypes() {
	if (type == TYPE_TYPE) {
		*this = nodes[0];
		return RemoveTypes();
	}
	else {
		for(int i = 0; i < nodes.GetCount(); i++) {
			nodes[i].RemoveTypes();
		}
	}
	return *this;
}

Node Node::SolveDomain() {
	for(int i = 0; i < nodes.GetCount(); i++) {
		nodes[i].SolveDomain();
		nodes[i].Calculate();
		if (nodes[i].IsNull()) {
			nodes.Remove(i);
			i--;
		}
	}
	
	if (type == TYPE_OP) {
		if (int_data == OP_BELONGS) {
			Node n0 = nodes[0];
			Node n1 = nodes[1];
			if (n0.int_data == OP_DIV) {
				// 1 / x === {[-inf, -1], [1, inf]}
				if (n0.At(0).IsInt() && n0.At(0).GetInt() == 1 && n0.At(1).IsId()) {
					n0 = n0.At(1);
					n1 = NegativeSpace(n1);
				}
			}
		}
		else if (int_data == OP_EQ || int_data == OP_INEQ) {
			Node vars = FindVars(*this);
			if (vars.IsArray() && vars.GetCount() == 1) {
				int op = GetInt();
				SetDataInt(OP_EQ);
				Solve(vars.At(0), true);
				SetDataInt(op);
			}
		}
		else if (int_data == OP_DIV) {
			int_data = OP_AND;
			nodes[1] = nodes[1] != 0;
		}
		else if (nodes.GetCount() == 2) {
			if (nodes[0].Compare(SpaceReal()) == 0) {
				if (nodes[1].Compare(SpaceReal()) == 0)
					*this = SpaceReal();
				else if (int_data == OP_AND) {
					*this = nodes[1];
				}
			}
			else if (nodes[1].Compare(SpaceReal()) == 0) {
				if (int_data == OP_AND) {
					*this = nodes[0];
				}
			}
		}
	}
	else if (type == TYPE_ID || type == TYPE_VECTORID) {
		String value = GetString();
		if (value == "M_asin" || value == "M_acos") {
			*this = Range(Int(-1), Int(1), true, true);
		}
		/*else if (value == "M_acos") {
			Node out = Set();
			out.Add( Range(NegInf(), Int(-1), true, true) );
			out.Add( Range(Int(1), Inf(), true, true) );
			*this = *out;
			// *this = Range(Int(-1), Int(1), true, true);
		}*/
		else if (value == "M_sin" || value == "M_cos") {
			*this = SpaceReal();
		}
		else if (value == "M_sqrt") {
			*this = Range(Int(0), Inf(), true, true);
		}
		else if (value == "M_abs") {
			*this = SpaceReal();
		}
		else if (value == "M_ln") {
			*this = Range(Int(-1), Int(1), false, false);
		}
		else {
			//*this = *SpaceReal();
		}
	}
	else if (type == TYPE_INT || type == TYPE_DOUBLE) {
		//*this = *SpaceReal();
	}
	else if (type == TYPE_SET) {
		if (nodes.GetCount() == 0)
			*this = Void();
		else {
			bool changes = true;
			while (changes) {
				changes = false;
				for(int i = 0; i < nodes.GetCount(); i++) {
					if (nodes[i].GetType() == TYPE_SET) {
						nodes.Append(nodes[i].nodes);
						nodes.Remove(i);
						changes = true;
						break;
					}
				}
			}
			for(int i = 0; i < nodes.GetCount(); i++) {
				for(int j = i+1; j < nodes.GetCount(); j++) {
					// Is invalid
					if (IsConflictingSpaces(nodes[i], nodes[j])) {
						*this = Void();
						return *this;
					}
					Node n = JoinSpaces(nodes[i], nodes[j]);
					if (!n.IsNull()) {
						nodes[i] = n;
						nodes.Remove(j);
						i--;
						break;
					}
				}
			}
		}
	}
	else if (type == TYPE_FUNCTION) {
		
		
	}
	
	Calculate();
	
	return *this;
}

Node Node::SolveImage() {
	
	for(int i = 0; i < nodes.GetCount(); i++) {
		nodes[i].SolveImage();
		if (nodes[i].IsNull()) {
			nodes.Remove(i);
			i--;
		}
	}
	
	if (type == TYPE_OP && nodes.GetCount() == 2) {
		Node n0 = nodes[0];
		Node n1 = nodes[1];
		
		// 1 / x --> { [-inf,0[, ]0, inf] }
		if (int_data == OP_DIV) {
			if (n0.IsInt() && n1.IsId()) {
				*this = Set()
					.Add(Range(NegInfReal(), Int(0), true, false))
					.Add(Range(Int(0),   InfReal(),  false, true));
			}
		}
	}
	else if (type == TYPE_FUNCTION) {
		int fn = GetInt();
		if (fn == FUNC_ASIN) {
			*this = Range(Pi() / Int(-2), Pi() / Int(2), true, true) && *this;
		}
		else if (fn == FUNC_ACOS) {
			*this = Range(Int(0), Pi(), true, true) && *this;
		}
		else if (fn == FUNC_SIN || fn == FUNC_COS) {
			*this = Range(-1, 1, true, true) && *this;
		}
		else if (fn == FUNC_SQRT) {
			*this = Range(0, InfReal(), true, true) && *this;
		}
	}
	else if (type == TYPE_ID) {
		*this = RangeReal();
	}
	return *this;
}

Node Node::Derive() {
	if (IsOp() && GetInt() == OP_EQ) {
		*this = DeriveFunction(*this);
	} else {
		*this = MathCore::Derive(*this);
		/*Node vars = *FindVars(*this);
		if (vars.GetCount() == 1)
			Derive(vars->At(0));
		else
			Derive(id("x"));*/
	}
	return *this;
}

Node Node::Derive(const Node key) {
	*this = MathCore::Derive(*this, key);
	return *this;
}

Node Node::Integrate() {
	*this = MathCore::Integrate(*this);
	return *this;
}

Node Node::Integrate(const Node key) {
	*this = MathCore::Integrate(*this, key);
	return *this;
}

Node Node::EraseSigns() {
	IteratorDeep it = BeginDeep();
	for ( ;!it.IsEnd(); ) {
		Node n = *it;
		if (n.IsFunction() && n.GetString() == "sign") {
			n = n.At(0);
		}
		else it++;
	}
	return *this;
}

Node Node::Evaluate() {
	*this = MathCore::Evaluate(*this);
	return *this;
}

Node Node::Ceiling() {
	Node fn = Function("ceiling").Add(*this);
	*this = MathCore::Evaluate(fn);
	return *this;
}

/*Node Node::SimplifyStepsPrint(String s) {
	
}

Node Node::CalculateStepsPrint(String s) {
	
}*/

Node Node::GetMul(const Node other_in_mul) {
	IteratorDeep it = BeginDeep();
	for (; !it.IsEnd();) {
		if (it->Compare(other_in_mul) == 0) {
			Node* inner = it.Inner();
			if (inner) {
				if (inner->IsOp() && inner->IsOp(OP_MUL) && inner->GetCount() == 2) {
					if (inner->At(0).Compare(other_in_mul) == 0)
						return inner->At(1);
					else
						return inner->At(0);
				}
			}
		}
		it++;
	}
	return *this;
}

Node Node::FindMin(const Node key) {
	return MathCore::FindMin(*this, key);
}

Node Node::FindMax(const Node key) {
	return MathCore::FindMax(*this, key);
}

Node Node::FindArrayTwoEquals() {
	if (!IsMultiple()) return Void();
	Upp::Array<Node> level_array = SplitLevelArray();
	for(int i = 0; i < level_array.GetCount(); i++) {
		for(int j = i+1; j < level_array.GetCount(); j++) {
			if (level_array[i].Compare(level_array[j]) == 0)
				return level_array[i];
		}
	}
	return Void();
}

Node Node::FindSinCos(bool must_have_var) {
	IteratorDeep it = BeginDeep();
	for ( ;!it.IsEnd(); it++) {
		Node n = *it;
		if (n.IsFunction() && (n.IsFuncSin() || n.IsFuncCos()) ) {
			if (!must_have_var)
				return n;
			else if (HasVar(n))
				return n;
		}
	}
	return Node();
}

Node Node::GetOpJoinNode(int op, bool accept_equal) {
	Node* n = this;
	/*
	int op_level = GetOpLevel();
	int last_level = 0;
	int level_steps = 0;
	int first_level = 0;
	
	int steps = 0;
	while (n->inner) {
		int ntype = n->type;
		int nlevel = n->GetOpLevel();
		int ntype_peek = n->inner->type;
		int nop_peek   = n->inner->int_data;
		int nlevel_peek = n->inner->GetOpLevel();
		
		if (nlevel) {
			if (!last_level) {
				last_level = nlevel;
				first_level = nlevel;
			}
			else if (nlevel != last_level)
				level_steps++;
		}
		
		if (!accept_equal && level_steps == 0) {
			if (nlevel == 4)
				break;
			// fix to: 1^((2^3)^4) goes (1^(2^3))^4
			if (nlevel == 3 && int_data == OP_POW)
				break;
			// fix to: 3/2+c --> 3/2*1+c  ==> (3/2+c)*1
			if (nlevel == 2 && op_level == 2)
				break;
		} else {
			if (ntype == TYPE_OP) {
				int op = n->int_data;
				if (nlevel == 1) {
					if ( op == OP_ADD || op == OP_SUB )
						break;
					if ( op == OP_MUL || op == OP_DIV )
						break;
					if ( op == OP_POW )
						break;
				}
				else if (nlevel == 2) {
					if ( op == OP_MUL || op == OP_DIV )
						break;
					if ( op == OP_POW )
						break;
				}
			}
			else if (ntype == TYPE_NULL || ntype == TYPE_ID || ntype == TYPE_INT || ntype == TYPE_DOUBLE) {
				if (nlevel_peek == 4 && n->inner->GetCount() == 1)
					break;
				if ( int_data == OP_MUL || int_data == OP_DIV )
					break;
				if ( int_data == OP_POW )
					break;
			}
			
			if (ntype_peek == TYPE_OP) {
				if (nop_peek == OP_DIV)
					break;
				if (nop_peek == OP_POW && n->inner->GetCount() == 2 && n->inner->At(1).IsEqual(*n) )
					break;
			}
		}
		
		if (nlevel_peek == 4)
			break;
		
		n = n->inner;
		steps++;
	}*/
	
	return *n;
}

Upp::Array<Node> Node::SplitLevelAdd() {
	Upp::Array<Node> out;
	if (IsOp() && (GetInt() == OP_ADD || GetInt() == OP_SUB)) {
		for(int i = 0; i < nodes.GetCount(); i++) {
			if (i && GetInt() == OP_SUB) {
				Upp::Array<Node> tmp = nodes[i].SplitLevelAdd();
				for(int i = 0; i < tmp.GetCount(); i++)
					tmp[i] = (Int(-1) * tmp[i]);
				out.Append(tmp);
			}
			else
				out.Append(nodes[i].SplitLevelAdd());
		}
	} else {
		out.Add(*this);
	}
	return out;
}

Upp::Array<Node> Node::SplitLevelArray() {
	Upp::Array<Node> out;
	if (IsMultiple()) {
		for(int i = 0; i < GetCount(); i++) {
			out.Append(nodes[i].SplitLevelArray());
		}
	} else {
		out.Add(*this);
	}
	return out;
}


void Node::Trim() {
	// TODO: optimize... this really can be done a lot faster...
	
	if (nodes.GetCount() == 1 && (type==TYPE_PARENTHESIS || type==TYPE_NULL)) {
		Node n = nodes[0];
		while (n.nodes.GetCount() == 1 && (n.type==TYPE_PARENTHESIS || n.type==TYPE_NULL)) {
			n = n.nodes[0];
		}
		*this = n;
		
		Trim();
	} else {
		for(int i = 0; i < nodes.GetCount(); i++) {
			nodes[i].Trim();
		}
	}
}

void Node::Normalize() {
	IteratorDeep it = BeginDeep();
	for (; !it.IsEnd(); it++) {
		if (it->IsOp() && it->GetInt() == OP_LIST) {
			while (1) {
				bool no_edit = true;
				for(int i = 0; i < it->GetCount(); i++) {
					Node n = it->At(i);
					if (n.IsOp() && n.GetInt() == OP_LIST) {
						it->Insert(i+1+0, n.At(0));
						it->Insert(i+1+1, n.At(1));
						it->nodes.Remove(i);
						i--;
						no_edit = false;
					}
				}
				if (no_edit) break;
			}
		}
	}
	it = BeginDeep();
	for (; !it.IsEnd(); it++) {
		if ( (it->IsFunction() || it->IsMultiple()) && it->GetCount() && it->At(0).GetInt() == OP_LIST) {
			Node n = it->At(0);
			for(int i = 0; i < n.GetCount(); i++)
				it->Add(n.At(i));
			it->nodes.Remove(0);
		}
	}
	
}

int Node::Compare(const Node& n) {
	const Node *ptr = &n;
	if ((int64)ptr == (int64)this)
		return 0;
	
	if (IsTypeUsingString()) {
		if (!n.IsTypeUsingString())
			return 1;
		int cmp = GetString().Compare(n.GetString());
		if (type == TYPE_ID || type == TYPE_VECTORID)
			return cmp;
		if (cmp) return cmp;
	}
	if (type != n.type) return n.type - type;
	if (int_data != n.int_data) return n.int_data - int_data;
	
	if (!n.nodes.GetCount() && !nodes.GetCount())
		return 0;
	else if (n.nodes.GetCount() && nodes.GetCount()) {
		IteratorDeep it = BeginDeep();
		IteratorDeep comp_it = n.BeginDeep();
		it++;
		comp_it++;
		for (; !it.IsEnd() && !comp_it.IsEnd(); it++, comp_it++) {
			int comp;
			comp = it->Compare( *comp_it );
			if (comp) {
				//LOG(it->AsInlineString() << " != " << comp_it->AsInlineString());
				return comp;
			} else {
				//LOG(it->AsInlineString() << " == " << comp_it->AsInlineString());
			}
		}
		return !(it.IsEnd() && comp_it.IsEnd());
	} else {
		return n.GetCount() - GetCount();
	}
}

double Node::GetNumerical() const {
	if (IsNumber())
		return GetNumber();
	Node tmp = *this;
	tmp.Calculate();
	if (tmp.IsNumber())
		return tmp.GetNumber();
	else {
		tmp = MathCore::Floating(tmp);
		if (tmp.IsNumber())
			return tmp.GetNumber();
		if (tmp.IsId() && tmp.GetString() == "infinity")
			return DBL_MAX;
	}
	return 0;
}

int Node::GetOpLevel() const {
	int out = 0;
	if (type == TYPE_OP) {
		if (int_data == OP_ADD || int_data == OP_SUB)
			out = 1;
		else if (int_data == OP_MUL || int_data == OP_DIV)
			out = 2;
		else if (int_data == OP_POW)
			out = 3;
		else
			out = 5;
	}
	else if (type == TYPE_PARENTHESIS || type == TYPE_ARRAY || type == TYPE_FUNCTION)
		out = 4;
	
	return out;
}

String Node::GetFunctionName() const {
	if (type != TYPE_FUNCTION) return "";
	if (int_data == FUNC_CUSTOM) return GetString();
	switch (int_data) {
		case FUNC_ABS:			return "abs";
		case FUNC_LENGTH:		return "len";
		case FUNC_CONJUGATE:	return "conj";
		case FUNC_DERIVE:		return "derive";
		case FUNC_INTEGRATE:	return "integrate";
		case FUNC_INTEGRATED_UNPLACED:
								return "integrated_unplaced";
		case FUNC_SIN:			return "sin";
		case FUNC_COS:			return "cos";
		case FUNC_TAN:			return "tan";
		case FUNC_ASIN:			return "asin";
		case FUNC_ACOS:			return "acos";
		case FUNC_ATAN :		return "atan";
		case FUNC_SQRT:			return "sqrt";
		case FUNC_POW:			return "pow";
		case FUNC_EXP:			return "exp";
		case FUNC_LOG:			return "log";
		case FUNC_SUM:			return "sum";
		case FUNC_LIMIT:		return "limit";
		case FUNC_PLUSMIN:		return "plusmin";
		case FUNC_CROSS:		return "cross";
		
		case FUNC_FACTORIAL:	return "factorial";
		case FUNC_BINCOEFF:		return "binomial";
	}
	return "";
}

String Node::GetOpString() const {
	if (!IsOp()) return "";
	if (type == TYPE_FUNCTION) return GetFunctionName();
	switch (int_data) {
		case	OP_NULL:		return "";
		case	OP_ASSIGN:		return ":=";
		case	OP_PLUSEQ:		return "+=";
		case	OP_LIST:		return ",";
		case	OP_ADD:			return "+";
		case	OP_SUB:			return "-";
		case	OP_MUL:			return "*";
		case	OP_DIV:			return "/";
		case	OP_POW:			return "^";
		case	OP_CALL:		return "()";
		case	OP_BELONGS:		return "-->";
		case	OP_EQ:			return "=";
		case	OP_INEQ:		return "!=";
		case	OP_GREQ:		return ">=";
		case	OP_LSEQ:		return "<=";
		case	OP_LESS:		return "<";
		case	OP_GRT:			return ">";
		case	OP_AND:			return "&&";
		case	OP_OR:			return "||";
		case	OP_ISEQ:		return "==";
	}
	return "";
}

String Node::GetTypeString() const {
	switch (type) {
		case TYPE_NULL:			return "void";
		case TYPE_INT:			return "int";
		case TYPE_DOUBLE:		return "double";
		case TYPE_TEXT:			return "text";
		case TYPE_ID:			return "id";
		case TYPE_VECTORID:		return "vector-id";
		case TYPE_TYPE:			return "type";
		case TYPE_IMG:			return "img";
		case TYPE_OP:			return "op";
		case TYPE_UNARY:		return "unary";
		case TYPE_FUNCTION:		return "function";
		case TYPE_PARENTHESIS:	return "innerhesis";
		case TYPE_ARRAY:		return "array";
		case TYPE_MATRIX:		return "matrix";
		case TYPE_SET:			return "set";
		case TYPE_LINEARSET:	return "linear-set";
		case TYPE_SPACE:		return "space";
		case TYPE_RANGE:		return "range";
	}
	return "unknown";
}

bool Node::IsSIUnit() const {
	if (!IsId()) return false;
	String value = GetString();
	if		(value == "m") return true;
	else if (value == "g") return true;
	else if (value == "kg") return true;
	else if (value == "s") return true;
	else if (value == "A") return true;
	else if (value == "K") return true;
	else if (value == "mol") return true;
	else if (value == "cd") return true;
	else if (value == "Hz") return true;
	else if (value == "rad") return true;
	else if (value == "sr") return true;
	else if (value == "N") return true;
	else if (value == "Pa") return true;
	else if (value == "J") return true;
	else if (value == "W") return true;
	else if (value == "C") return true;
	else if (value == "V") return true;
	else if (value == "F") return true;
	else if (value == "ohm") return true;
	else if (value == "S") return true;
	else if (value == "Wb") return true;
	else if (value == "T") return true;
	else if (value == "H") return true;
	else if (value == "Cel") return true;
	else if (value == "lm") return true;
	else if (value == "lx") return true;
	else if (value == "Bq") return true;
	else if (value == "Gy") return true;
	else if (value == "Sv") return true;
	else if (value == "kat") return true;
	return false;
}

bool Node::IsTypeUsingString() const {
	return
		type == TYPE_TEXT ||
		type == TYPE_ID ||
		type == TYPE_VECTORID ||
		(type == TYPE_FUNCTION && int_data == FUNC_CUSTOM);
}

void Node::SetOp(int op) {
	this->int_data = op;
	type = TYPE_OP;
}

void Node::SetType(int type) {
	this->type = type;
}

void Node::SetInt(int i) {
	int_data = i;
	type = TYPE_INT;
}

void Node::SetDataInt(int i) {
	int_data = i;
}

void Node::SetDouble(double d) {
	dbl_data = d;
	type = TYPE_DOUBLE;
}

void Node::SetDataDouble(double d) {
	dbl_data = d;
}

void Node::SetString(String str) {
	str_data = str;
	type = TYPE_ID;
}

void Node::SetDataString(String str) {
	str_data = str;
}

void Node::SetNull() {
	type = TYPE_NULL;
	int_data = 0;
	dbl_data = 0.0;
	str_data.Clear();
	nodes.Clear();
}

void Node::SetFunction(int f) {
	int_data = f;
	str_data.Clear();
	type = TYPE_FUNCTION;
}

void Node::SetFunction(const String& s) {
	str_data = s;
	int_data = 0;
	type = TYPE_FUNCTION;
}

String Node::AsString(int indent) {
	String s;
	for(int i = 0; i < indent; i++)
		s += "-";
	s += ": " + GetString() + ", " + IntStr(GetInt()) + ", " + GetOpString() + ", " + GetTypeString() + "\n";
	for(int i = 0; i < nodes.GetCount(); i++) {
		s += nodes[i].AsString(indent+1);
	}
	return s;
}

String Node::AsInlineString() const {
	return GetString() + ", " + IntStr(GetInt()) + ", " + GetOpString() + ", " + GetTypeString();
}

String Node::AsDataInlineString() const {
	return GetString() + ", " + IntStr(GetInt()) + ", " + GetOpString() + ", " + GetTypeString();
}




Node operator && (Node a, Node b) {
	return Op(OP_AND).Add(a).Add(b);
}

Node operator || (Node a, Node b) {
	return Op(OP_OR).Add(a).Add(b);
}

Node operator == (Node a, Node b) {
	return Op(OP_EQ).Add(a).Add(b);
}

Node operator == (int a, Node b) {
	return Op(OP_EQ).Add(Node(a)).Add(b);
}

Node operator != (Node a, Node b) {
	return Op(OP_INEQ).Add(a).Add(b);
}

Node operator >= (Node a, Node b) {
	return Op(OP_GREQ).Add(a).Add(b);
}

Node operator <= (Node a, Node b) {
	return Op(OP_LSEQ).Add(a).Add(b);
}

Node operator > (Node a, Node b) {
	return Op(OP_GRT).Add(a).Add(b);
}

Node operator < (Node a, Node b) {
	return Op(OP_LESS).Add(a).Add(b);
}

Node operator + (Node a, Node b) {
	return Op(OP_ADD).Add(a).Add(b);
}

Node operator - (Node a, Node b) {
	return Op(OP_SUB).Add(a).Add(b);
}

Node operator - (Node a) {
	return Negate(a);
}

Node operator * (Node a, Node b) {
	return Op(OP_MUL).Add(a).Add(b);
}

Node operator * (String a, Node b) {
	return Op(OP_MUL).Add(Node(a)).Add(b);
}

Node operator * (Node a, String s) {
	return Op(OP_MUL).Add(a).Add(Node(s));
}

Node operator / (Node a, Node b) {
	return Op(OP_DIV).Add(a).Add(b);
}

Node operator ^ (Node a, Node b) {
	return Op(OP_POW).Add(a).Add(b);
}

Node operator & (Node a, Node b) {
	return Op(OP_SETAND).Add(a).Add(b);
}

Node operator | (Node a, Node b) {
	return Op(OP_SETOR).Add(a).Add(b);
}

Node operator ! (Node a) {
	return Unary(UNARY_NEGSET).Add(a);
}


Node Void() {
	Node out;
	return out;
}

Node Int(int i) {
	Node out;
	out.SetType(TYPE_INT);
	out.SetDataInt(i);
	return out;
}

Node Double(double d) {
	Node out;
	out.SetType(TYPE_DOUBLE);
	out.SetDataDouble(d);
	return out;
}

Node Text(String s) {
	Node out;
	out.SetType(TYPE_TEXT);
	out.SetDataString(s);
	return out;
}

Node id(String id) {
	Node out;
	out.SetType(TYPE_ID);
	out.SetDataString(id);
	return out;
}

Node VectorId(String id) {
	Node out;
	out.SetType(TYPE_VECTORID);
	out.SetDataString(id);
	return out;
}

Node Type(Node n, Node type) {
	Node out;
	out.SetType(TYPE_TYPE);
	out.Add(n);
	out.Add(type);
	return out;
}

Node Imaginary() {
	Node out;
	out.SetType(TYPE_IMG);
	out.SetDataInt(IMG_I);
	return out;
}

Node Function(String fn_name) {
	Node out;
	out.SetType(TYPE_FUNCTION);
	out.SetDataInt(FUNC_CUSTOM);
	out.SetDataString(fn_name);
	return out;
}

Node Function(int fn) {
	Node out;
	out.SetType(TYPE_FUNCTION);
	out.SetDataInt(fn);
	return out;
}

Node Array() {
	Node out;
	out.SetType(TYPE_ARRAY);
	out.SetDataInt(0);
	return out;
}

Node Array(int size) {
	Node out;
	out.SetType(TYPE_ARRAY);
	out.SetDataInt(0);
	for(int i = 0; i < size; i++)
		out.Add(Void());
	return out;
}

Node Array(Node x, Node y) {
	Node out = Array();
	out.Add(x);
	out.Add(y);
	return out;
}

Node Array(Node x, Node y, Node z) {
	Node out = Array();
	out.Add(x);
	out.Add(y);
	out.Add(z);
	return out;
}

Node Parenthesis() {
	Node out;
	out.SetType(TYPE_PARENTHESIS);
	return out;
}

Node Range(Node min, Node max, bool also_min_equal, bool also_max_equal) {
	Node out;
	out.SetType(TYPE_RANGE);
	out.Add(min);
	out.Add(max);
	out.Add(Int(also_min_equal));
	out.Add(Int(also_max_equal));
	return out;
}

Node RangeReal() {
	return Range(NegInfReal(), InfReal(), true, true);
}

Node RangeInteger() {
	return Range(NegInfInt(), InfInt(), true, true);
}

Node RangeRealNotInf() {
	return Range(NegInfReal(), InfReal(), false, false);
}

Node RangeIntegerNotInf() {
	return Range(NegInfInt(), InfInt(), false, false);
}

Node RangeNatural() {
	return Range(Int(0), InfInt(), true, true);
}

Node Matrix(int rows, int cols) {
	Node out;
	out.SetType(TYPE_MATRIX);
	Node row = Array(cols);
	for(int i = 0; i < rows; i++)
		out.Add(row);
	return out;
}

Node MatrixAlpha(int rows, int cols) {
	Node out;
	out.SetType(TYPE_MATRIX);
	Node row = Array(cols);
	for(int i = 0; i < rows; i++)
		out.Add(row);
	int chr = 'a';
	for(int i = 0; i < rows * cols; i++) {
		int row = i / cols;
		int col = i % cols;
		String id_;
		id_.Cat(chr);
		chr++;
		out.At(row).At(col) = id(id_);
	}
	return out;
}

Node SpaceReal() {
	Node n;
	n.SetType(TYPE_SPACE);
	n.SetDataInt(SPACE_REAL);
	return n;
}

Node SpaceInteger() {
	Node n;
	n.SetType(TYPE_SPACE);
	n.SetDataInt(SPACE_INTEGER);
	return n;
}

Node Coordinate(Node x, Node y) {
	return Array().Add(x).Add(y);
}

Node RealNumbers() {
	return SpaceReal();
}

Node Set() {
	Node out;
	out.SetType(TYPE_SET);
	out.SetDataInt(0);
	return out;
}

Node LinearSet() {
	Node out;
	out.SetType(TYPE_LINEARSET);
	out.SetDataInt(0);
	return out;
}

Node SetOrder(Node first, Node second) {
	Node out;
	out.SetType(TYPE_OP);
	out.SetDataInt(OP_SETORDER);
	out.Add(first);
	out.Add(second);
	return out;
}

Node Subscript(Node id_, Node id_sub) {
	String value = id_.GetString() + "_" + id_sub.GetString();
	return id(value);
}

Node BelongsTo(Node element, Node space) {
	Node out = Op(OP_BELONGS);
	out.Add(element);
	out.Add(space);
	return out;
}

Node Follows(Node var, Node dist) {
	Node out = Op(OP_FOLLOWS);
	out.Add(var);
	out.Add(dist);
	return out;
}

Node Op(int op) {
	Node out;
	out.SetDataInt(op);
	out.SetType(TYPE_OP);
	return out;
}

Node Unary(int unary_op) {
	Node out;
	out.SetDataInt(unary_op);
	out.SetType(TYPE_UNARY);
	return out;
}

Node Add(Node in0, Node in1) {
	if (in0.IsNull()) return in1;
	if (in1.IsNull()) return in0;
	Node out = Op(OP_ADD);
	out.Add(in0);
	out.Add(in1);
	return out;
}

Node Subtract(Node in0, Node in1) {
	Node out = Op(OP_SUB);
	out.Add(in0);
	out.Add(in1);
	return out;
}

Node PlusMinus(Node in0) {
	Node out;
	out.SetType(TYPE_FUNCTION);
	out.SetDataInt(FUNC_PLUSMIN);
	out.Add(in0);
	return out;
}

Node Multiply(Node in0, Node in1) {
	Node out = Op(OP_MUL);
	out.Add(in0);
	out.Add(in1);
	return out;
}

Node Divide(Node in0, Node in1) {
	Node out = Op(OP_DIV);
	out.Add(in0);
	out.Add(in1);
	return out;
}

Node VectorAdd(Node vec0, Node vec1) {
	if (vec0.GetCount() != vec1.GetCount() || !vec0.IsArray() || !vec1.IsArray())
		return Void();
	Node out = Array();
	for(int i = 0; i < vec0.GetCount(); i++)
		out.Add(vec0[i] + vec1[i]);
	return out;
}

Node VectorSubtract(Node vec0, Node vec1) {
	if (vec0.GetCount() != vec1.GetCount() || !vec0.IsArray() || !vec1.IsArray()) return Void();
	Node out = Array();
	for(int i = 0; i < vec0.GetCount(); i++)
		out.Add(vec0[i] - vec1[i]);
	return out;
}

Node VectorMultiply(Node vec0, Node vec1) {
	if (vec0.GetCount() != vec1.GetCount() || !vec0.IsArray() || !vec1.IsArray()) return Void();
	Node out = Array();
	for(int i = 0; i < vec0.GetCount(); i++)
		out.Add(vec0[i] * vec1[i]);
	return out;
}

Node VectorDivide(Node vec0, Node vec1) {
	if (vec0.GetCount() != vec1.GetCount() || !vec0.IsArray() || !vec1.IsArray()) return Void();
	Node out = Array();
	for(int i = 0; i < vec0.GetCount(); i++)
		out.Add(vec0[i] / vec1[i]);
	return out;
}

Node VectorSum(Node vec0) {
	if (!vec0.IsArray())
		return Void();
	Node out;
	for(int i = 0; i < vec0.GetCount(); i++) {
		Node n = vec0[i];
		if (n.IsArray()) {
			if (!i) out = VectorSum(n);
			else out = (out + VectorSum(n));
		} else {
			if (!i) out = n;
			else out = (out + n);
		}
	}
	out.Evaluate();
	return out;
}

Node CrossProduct(Node in0, Node in1) {
	Node out = Function(FUNC_CROSS);
	out.Add(in0); out.Add(in1);
	return out;
}

Node Fraction(int in0, int in1) {
	Node out = Op(OP_DIV);
	out.Add(Int(in0)); out.Add(Int(in1));
	return out;
}

Node Negate(Node in0) {
	Node out = Unary(UNARY_NEG);
	out.Add(in0);
	return out;
}

Node MultiplyImaginary(Node in0) {
	return Multiply(Imaginary(), in0);
}

Node Conjugate(Node in0) {
	Node out = Function(FUNC_CONJUGATE);
	out.Add(in0);
	return out;
}

Node Absolute(Node in0) {
	Node out = Function(FUNC_ABS);
	out.Add(in0);
	return out;
}

Node Abs(Node in0) {
	Node out = Function(FUNC_ABS);
	out.Add(in0);
	return out;
}

Node Length(Node in0) {
	Node out = Function(FUNC_LENGTH);
	out.Add(in0);
	return out;
}

Node Pow2(Node in0) {
	Node out = Op(OP_POW);
	out.Add(in0); out.Add(2);
	return out;
}

Node Pow10(Node in0) {
	Node out = Op(OP_POW);
	out.Add(10); out.Add(in0);
	return out;
}

Node Pow(Node in0, Node in1) {
	Node out = Op(OP_POW);
	out.Add(in0); out.Add(in1);
	return out;
}

Node Sqrt(Node in0) {
	Node out = Function(FUNC_SQRT);
	out.Add(in0);
	return out;
}

Node Log(Node in0) {
	Node out = Function(FUNC_LOG);
	out.Add(in0);
	return out;
}

Node Factorial(Node in) {
	Node out = Function(FUNC_FACTORIAL);
	out.Add(in);
	return out;
}

Node BinomialCoefficient(Node n, Node k) {
	Node out = Function(FUNC_BINCOEFF);
	out.Add(n);
	out.Add(k);
	return out;
}

Node Parenthesis(Node in0) {
	Node out;
	out.SetType(TYPE_PARENTHESIS);
	return out;
}

Node FunctionDerive(Node in0) {
	Node out = Function(FUNC_DERIVE);
	out.Add(in0);
	return out;
}

Node FunctionDerive(Node in0, Node key) {
	Node out = (id("partdiff") / (id("partdiff")*key)) * in0;
	return out;
}

Node FunctionIntegrate(Node in0, Node key, Node begin, Node end) {
	Node out = Function(FUNC_INTEGRATE);
	out.Add(in0);
	out.Add(key);
	out.Add(begin);
	out.Add(end);
	return out;
}

Node FunctionIntegrate(Node in0, Node key) {
	Node out = Function(FUNC_INTEGRATE);
	out.Add(in0);
	out.Add(key);
	return out;
}

Node FunctionIntegrateUnplace(Node in0, Node key, Node begin, Node end) {
	Node out = Function(FUNC_INTEGRATED_UNPLACED);
	out.Add(in0);
	out.Add(begin);
	out.Add(end);
	out.At(0).Integrate(key);
	return out;
}

Node FunctionSum(Node expr, Node var, Node begin, Node end) {
	Node out = Function(FUNC_SUM);
	out.Add(expr).Add(var).Add(begin).Add(end);
	return out;
}

Node FunctionLimit(Node expr, Node var, Node limit) {
	Node out = Function(FUNC_LIMIT);
	out.Add(expr).Add(var).Add(limit);
	return out;
}

Node FunctionProbability(String id_) {
	return Function("P").Add(id(id_));
}

Node FunctionProbability(Node n) {
	return Function("P").Add(n);
}

Node Prob(const String& id_) {
	return Function("P").Add(id(id_));
}

Node Prob(Node n) {
	return Function("P").Add(n);
}

Node Sin(Node expr) {
	Node out = Function(FUNC_SIN);
	out.Add(expr);
	return out;
}

Node Cos(Node expr) {
	Node out = Function(FUNC_COS);
	out.Add(expr);
	return out;
}

Node Tan(Node expr) {
	Node out = Function(FUNC_TAN);
	out.Add(expr);
	return out;
}

Node ArcSin(Node expr) {
	Node out = Function(FUNC_ASIN);
	out.Add(expr);
	return out;
}

Node ArcCos(Node expr) {
	Node out = Function(FUNC_ACOS);
	out.Add(expr);
	return out;
}

Node ArcTan(Node expr) {
	Node out = Function(FUNC_ATAN);
	out.Add(expr);
	return out;
}

Node IncrementRatio(Node a, Node b) {
	Node out = Op(OP_INCRATIO);
	out.Add(a);
	out.Add(b);
	return out;
}

Node SetNegate(Node in0) {
	Node out = Unary(UNARY_NEGSET);
	out.Add(in0);
	return out;
}

Node SetComplement(Node in0, Node in1) {
	Node out = Op(OP_SETCOMPL);
	out.Add(in0);
	out.Add(in1);
	return out;
}

Node Pi() {
	return id("pi");
}

Node Euler() {
	return id("e");
}

Node EulerMascheroni() {
	return id("gamma");
}

Node Pythagora() {
	return Sqrt(Int(2));
}

Node GoldenRatio() {
	return id("PHI");
}

Node InverseGoldenRatio() {
	return id("phi");
}

Node SilverRatio() {
	return id("delta s");
}

Node Inf() {
	return id("inf");
}

Node NegInf() {
	return Negate(id("inf"));
}

Node InfInt() {
	return id("inf");
}

Node NegInfInt() {
	return Negate(id("inf"));
}

Node InfReal() {
	return id("inf");
}

Node NegInfReal() {
	return Negate(id("inf"));
}

Node LineCircle(Node radius) {
	return Pow2(id("x")) + Pow2(id("y")) == radius;
}

Node AreaSquare(Node side) {
	return Pow2(side);
}

Node AreaRect(Node side0, Node side1) {
	return side0 * side1;
}

Node AreaTrapezoid(Node side0, Node side1, Node height) {
	return (height / 2) * (side0 + side1);
}

Node AreaCircle(Node radius) {
	return Pi() * Pow2(radius);
}

Node AreaEllipse(Node radius0, Node radius1) {
	return Pi() * radius0 * radius1;
}

Node AreaTriangle(Node side, Node height) {
	return Fraction(1, 2) * side * height;
}

Node AreaTriangle(Node side0, Node side1, Node side2) {
	Node s = (side0 + side1 + side2) / Int(2);
	return s * (s - side0) * (s - side1) * (s - side2);
}

Node AreaCube(Node side) {
	return 6 * Pow2(side);
}

Node AreaPrism(Node area, Node perimeter, Node height) {
	return perimeter * height + 2 * area;
}

Node AreaSphere(Node radius) {
	return 4 * Pi() * Pow2(radius);
}

Node VolumeCube(Node height) {
	return Pow(height, Int(3));
}

Node VolumeRectPrism(Node x, Node y, Node z) {
	return x * y * z;
}

Node VolumeIrregularPrism(Node area, Node height) {
	return area * height;
}

Node VolumeCylinder(Node radius, Node height) {
	return Pi() * Pow2(radius) * height;
}

Node VolumePyramid(Node side, Node height) {
	return Fraction(1, 3) * side * height;
}

Node VolumeCone(Node radius, Node height) {
	return Fraction(1, 3) * Pi() * Pow2(radius) * height;
}

Node VolumeSphere(Node radius) {
	return Fraction(4, 3) * Pi() * Pow(radius, 3);
}

Node VolumeEllipsoid(Node r1, Node r2, Node r3) {
	return Fraction(4, 3) * Pi() * r1 * r2 * r3;
}

Node VolumeError() {
	return
		Absolute(id("DELTA V")) <=
			Absolute(FunctionDerive(id("V")) /
				FunctionDerive(IdRadius)) *
			Absolute(id("DELTA r"));
}


}



// Function, which COULD handle + - * / etc. operators in ExeCore.
/*template <> void ClassObjOperator(
	ClassObj& ret,
	ExeM& obj,
	ClassWrap& b,
	int op)
{
	
	switch ( op )
	{
		case LEX_PLUSEQUAL:
			//SetInt(da + db); return this;
			
		case LEX_MINUSEQUAL:
			//SetInt(da - db); return this;
			
		case '+':
			//return NewMemory ( da + db );

		case '-':
			//return NewMemory ( da -db );
		
		case '*':
			//return NewMemory ( da*db );

		case '/':
			//if (db == 0)
			//	throw RuntimeError("Zero divide", ExeVar(MemExeNumber(*this), this));
			//return NewMemory ( da / db );

		case '&':
			//return NewMemory ( da&db );

		case '|':
			//return NewMemory ( da | db );

		case '^':
			//return NewMemory ( da ^ db );

		case '%':
			//return NewMemory ( da % db );

		case LEX_EQUAL:
			//return NewMemory ( da == db );

		case LEX_NEQUAL:
			//return NewMemory ( da != db );

		case '<':
			//return NewMemory ( da < db );

		case LEX_LEQUAL:
			//return NewMemory ( da <= db );

		case '>':
			//return NewMemory ( da > db );

		case LEX_GEQUAL:
			//return NewMemory ( da >= db );

		case LEX_LSHIFT:
			//return NewMemory ( da << db );
			
		case LEX_RSHIFT:
			//return NewMemory ( da >> db );
		
		case LEX_RSHIFTUNSIGNED:
			//return NewMemory ( (int)(uint32(da) >> db) );
			
		case LEX_LSHIFTEQUAL:
			//SetInt ( da << db ); return this;
			
		case LEX_RSHIFTEQUAL:
			//SetInt ( da >> db ); return this;
		
		case LEX_ANDEQUAL:
			//SetInt(da && db); return this;
			
	    case LEX_ANDAND:
			//return NewMemory ( da && db );
			
	    case LEX_OREQUAL:
			//SetInt(da || db); return this;
			
	    case LEX_OROR:
			//return NewMemory ( da || db );
			
	    case LEX_XOREQUAL:
			//SetInt((int)da ^ (int)db); return this;
	    
		default:
			return;
			//throw ScriptException ( "Operation " + MathOpStr ( op ) + " not supported on the Int datatype" );
	}
}*/
