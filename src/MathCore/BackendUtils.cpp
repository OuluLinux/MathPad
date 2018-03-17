#include "MathCore.h"


namespace MathCore {
	

bool IsExprFunc(Node expr) {
	return expr.IsOp() && expr.GetInt() == OP_EQ && expr[0].IsFunction();
}

bool IsExprTrigFunc(Node expr) {
	if (!expr.IsFunction()) return false;
	int s = expr.GetInt();
	return s == FUNC_SIN || s == FUNC_COS || s == FUNC_TAN || s == FUNC_ASIN || s == FUNC_ACOS || s == FUNC_ATAN;
}

bool IsExprTrigFuncSinCos(Node expr) {
	if (!expr.IsFunction()) return false;
	int s = expr.GetInt();
	return s == FUNC_SIN || s == FUNC_COS;
}

bool IsExprInequality(Node expr) {
	if (!expr.IsOp()) return false;
	int op = expr.GetInt();
	return op == OP_GRT || op == OP_LESS || op == OP_GREQ || op == OP_LSEQ || op == OP_INEQ;
}

bool IsExprArrayEqual(Node expr) {
	if (!expr.IsArray()) return false;
	if (!expr.GetCount()) return false;
	for(int i = 1; i < expr.GetCount(); i++) {
		if (expr[i].Compare(expr[0]) != 0)
			return false;
	}
	return true;
}

bool IsConflictingSpaces(Node space1, Node space2) {
	
	
	return false;
}



Node ExprTrigFuncConvert(Node expr, int func) {
	if (!expr.IsFunction()) return Node();
	Node out = expr;
	int oldfunc = out.GetInt();
	if (func == FUNC_SIN) {
		if (oldfunc == FUNC_COS) {
			out[0] = Add(Pi() / 2, out[0]);
			out.SetDataInt(FUNC_SIN);
		}
	}
	else if (func == FUNC_COS) {
		if (oldfunc == FUNC_SIN) {
			out[0] = Subtract(Pi() / 2, out[0]);
			out.SetDataInt(FUNC_COS);
		}
	}
	return out;
}

Node ExprOpSwap(Node expr) {
	if (!expr.IsOp() || expr.GetCount() != 2) return expr;
	Node out = Op(expr.GetInt());
	out.Add(expr[1]).Add(expr[0]);
	return out;
}

Node ExprJoinAddExp2(Node expr) {
	if (!expr.IsMultiple()) return Node();
	if (expr.GetCount() != 2) return Node();
	if (expr[0].GetInt() != OP_EQ || expr[1].GetInt() != OP_EQ) return Node();
	Node out;
	
	out = (Pow2(expr[0][0]) + Pow2(expr[1][0])) == (Pow2(expr[0][1]) + Pow2(expr[1][1]));
	
	return out;
}

Node ExprArrayPositive(Node expr) {
	if (expr.GetInt() == OP_EQ)
		return ExprArrayPositive(expr[1]);
	
	// ...default giac behavior...
	if (expr.IsArray() && expr.GetCount() == 2 && !expr[0].IsArray() && !expr[1].IsArray()) {
		Node n0 = expr[0];
		Node n1 = expr[1];
		if (n0.GetInt() == OP_SUB && n0[0].IsNull())
			return expr[1];
		if (n0.IsInt() && n1.IsInt() && n1.GetInt() > n0.GetInt())
			return expr[1];
		
		return expr[0];
	}
	
	// ...not reliable logic begins
	Node::IteratorDeep it = expr.BeginDeep();
	for (;!it.IsEnd();it++) {
		Node n = *it;
		if (n.IsArray()) {
			for(int i = 0; i < n.GetCount(); i++) {
				Node n2 = n[i];
				if (n2.IsArray()) {
					Node n3 = ExprArrayPositive(n2);
					if (!n3.IsNull()) return n3;
				}
				else if (n2.GetInt() == OP_SUB && n2[0].IsNull())
					continue;
				
				// Best found so far...
				return n2;
			}
		}
	}
	
	return Node();
}

Node ExprCurveFromFunc(Node expr) {
	Node key = expr[0][0];
	if (key.IsNull()) return Node();
	
	// Change var to x
	Node out = id("y") == ExprFuncPlace(expr, id("x"));
	return out;
}

String ChrStr(int i) {
	String out; out.Cat(i); return out;
}

Node VectorExpr(Node n) {
	int chr = (n.GetCount() <= 3 ? 'x' : 'a');
	Node out;
	for(int i = 0; i < n.GetCount(); i++) {
		out = Add(out, Multiply(id(ChrStr(chr+i)), n[i]));
	}
	return out;
}

Node VectorPoints(Node a, Node b) {
	if (a.GetCount() != b.GetCount()) return Node();
	Node out = Array();
	for(int i = 0; i < a.GetCount(); i++) {
		out.Add(Subtract(b[i], a[i]));
	}
	return out;
}

Node ExprVectorLength(Node vec) {
	if (!vec.IsArray()) return Node();
	Node out;
	for(int i = 0; i < vec.GetCount(); i++) {
		if (!i)
			out = (vec[i]^2);
		else
			out = out + (vec[i]^2);
	}
	return Sqrt(out);
}

Node ExprMultiplyVectors(Node vec0, Node vec1) {
	if (!vec0.IsArray() || !vec1.IsArray()) return Node();
	if (vec0.GetCount() != vec1.GetCount()) return Node();
	Node out;
	for(int i = 0; i < vec0.GetCount(); i++) {
		out = out + vec0[i] * vec1[i];
	}
	return out;
}

Node ExprAngleVectors(Node vec0, Node vec1) {
	if (!vec0.IsArray() || !vec1.IsArray()) return Node();
	if (vec0.GetCount() != vec1.GetCount()) return Node();
	Node out;
	out = Function(FUNC_COS).Add(id("alpha")) == (ExprMultiplyVectors(vec0, vec1)) / (ExprVectorLength(vec0) * ExprVectorLength(vec1));
	return out;
}

Node ExprCrossVector(Node vec0, Node vec1) {
	if (!vec0.IsArray() || !vec1.IsArray()) return Node();
	if (vec0.GetCount() != vec1.GetCount()) return Node();
	if (vec0.GetCount() == 2) {
		Node out;
		out = (((vec0[0] * vec1[1]) - (vec0[1] * vec1[0])));
		return out;
	}
	else if (vec0.GetCount() == 3) {
		Node out = Array();
		out.Add((vec0[1] * vec1[2]) - (vec0[2] * vec1[1]));
		out.Add((vec0[2] * vec1[0]) - (vec0[0] * vec1[2]));
		out.Add((vec0[0] * vec1[1]) - (vec0[1] * vec1[0]));
		return out;
	}
	else return Node();
}

Node ExprCrossVectorMatrix(Node mat) {
	int count = mat.GetCount();
	if (!mat.IsMatrix() || count < 2) return Node();
	for(int i = 0; i < count; i++) {
		if (mat[i].GetCount() != count)
			return Node();
	}
	
	Node out;
	for(int i = 0; i < count; i++) {
		Node plus, minus;
		for(int j = 0; j < count; j++) {
			int row = j;
			int plus_col = i + j;
			if (plus_col >= count) plus_col -= count;
			int minus_col = i - j;
			if (minus_col < 0) minus_col += count;
			if (j) {
				plus = plus * mat[row][plus_col];
				minus = minus * mat[row][minus_col];
			} else {
				plus = mat[row][plus_col];
				minus = mat[row][minus_col];
			}
		}
		if (i)
			out = out + plus - minus;
		else
			out = plus - minus;
	}
	return out;
}

Node ExprEqualVectors(Node vec0, Node vec1) {
	if (!vec0.IsArray() || !vec1.IsArray()) return Node();
	if (vec0.GetCount() != vec1.GetCount()) return Node();
	Node out = LinearSet();
	for(int i = 0; i < vec0.GetCount(); i++) {
		out.Add( vec0[i] == vec1[i] );
	}
	return out;
}

Node ExprLineVectorPoint(Node vec, Node pt) {
	if (!vec.IsArray() || !pt.IsArray()) return Node();
	if (vec.GetCount() != pt.GetCount()) return Node();
	Node out = Array();
	for(int i = 0; i < pt.GetCount(); i++) 
		out.Add(pt[i]);
	for(int i = 0; i < vec.GetCount(); i++) 
		out.Add(vec[i]);
	return out;
}

Node ExprParamVecFormVector(Node vec) {
	int c = vec.GetCount();
	// 2D and 3D vectors
	if (c != 2 && c != 3) return Node();
	Node out = id("v") == ((vec[0] * "i") + (vec[1] * "j") + (vec[2] * "k"));
	return out;
}

Node ExprParamVecFormLine(Node line) {
	int c = line.GetCount();
	// 2D and 3D lines
	if (c != 4 && c != 6) return Node();
	Node out = (id("p") == 
		(((line[0] * "i") + (line[1] * "j") + (line[2] * "k")) + 
		Id("t") * ((line[3] * "i") + (line[4] * "j") + (line[5] * "k")) )) ==
		( (line[0] + "t" * line[3]) * "i" + (line[1] + "t" * line[4]) * "j" + (line[2] + "t" * line[5]) * "k");
	return out;
}

Node ExprParamCoordFormLine(Node line) {
	int c = line.GetCount();
	// 2D and 3D lines
	if (c != 4 && c != 6) return Node();
	Node out = (Array(id("x"), id("y"), id("z")) == 
		(Array(line[0], line[1], line[2]) + id("t") * Array(line[3], line[4], line[5]) )) ==
		Array((line[0] + "t" * line[3]), (line[1] + "t" * line[4]), (line[2] + "t" * line[5]));
	return out;
}

Node ExprCoordFormLine(Node line, String key) {
	int c = line.GetCount();
	if (c != 4 && c != 6) return Node();
	return Array((line[0] + key * line[3]), (line[1] + key * line[4]), (line[2] + key * line[5]));
}

Node ExprFuncPlace(Node func, Node replacing, bool keep_function) {
	if (!IsExprFunc(func)) return Node();
	Node out = (keep_function ? func : func[1]);
	if (func[0].IsFuncDerive()) {
		if (func[0][0].IsFuncDerive()) {
			out.Replace(func[0][0][0][0], replacing);
		}
		else out.Replace(func[0][0][0], replacing);
	} 
	else out.Replace(func[0][0], replacing);
	return out;
}


Node LineVec(Node line) {
	if (!line.IsArray()) return Node();
	int c = line.GetCount();
	if (c != 4 && c != 6) return Node();
	int mid = c / 2;
	Node out = Array();
	for(int i = mid; i < line.GetCount(); i++)
		out.Add(line[i]);
	return out;
}

Node LinePt(Node line) {
	if (!line.IsArray()) return Node();
	int c = line.GetCount();
	if (c != 4 && c != 6) return Node();
	int mid = c / 2;
	Node out = Array();
	for(int i = 0; i < mid; i++)
		out.Add(line[i]);
	return out;
}


bool IsConstant(const String& s) {
	if (s == "pi") return true;
	if (s == "phi") return true;
	if (s == "e") return true;
	if (s == "i") return true;
	return false;
}

Node FindNode(Node n, Node cmp) {
	Node::IteratorDeep it = n.BeginDeep();
	Node out;
	for (; !it.IsEnd(); it++) {
		if (it->Compare(cmp) == 0)
			return *it;
	}
	return out;
}

Node FindVar(Node n) {
	Node::IteratorDeep it = n.BeginDeep();
	Node out;
	for (; !it.IsEnd(); it++) {
		if (it->IsId() && it->GetCount() == 0 && !IsConstant(it->GetString()))
			return *it;
	}
	return out;
}

Node FindVars(Node n) {
	Node::IteratorDeep it = n.BeginDeep();
	Node out = Array();
	Index<String> vars;
	for (; !it.IsEnd(); it++) {
		if (it->IsId() && it->GetCount() == 0 && !IsConstant(it->GetString())) {
			if (vars.Find(it->GetString()) == -1)
				vars.Add(it->GetString());
		}
	}
	for(int i = 0; i < vars.GetCount(); i++)
		out.Add( id( vars[i] ) );
	return out;
}

bool HasVar(Node n) {
	Node::IteratorDeep it = n.BeginDeep();
	Node out;
	for (; !it.IsEnd(); it++) {
		if (it->IsId() && it->GetCount() == 0 && !IsConstant(it->GetString()))
			return true;
	}
	return false;
}

bool HasVar(Node n, Node var) {
	Node::IteratorDeep it = n.BeginDeep();
	Node out;
	for (; !it.IsEnd(); it++) {
		if (it->Compare(var) == 0)
			return true;
	}
	return false;
}

bool HasFunction(Node n, String name) {
	Node::IteratorDeep it = n.BeginDeep();
	Node out;
	for (; !it.IsEnd(); it++) {
		if (it->IsFunction()) {
			if (it->GetFunctionName() == name)
				return true;
		}
	}
	return false;
}

Node FindFunction(Node n, String name) {
	Node::IteratorDeep it = n.BeginDeep();
	Node out;
	for (; !it.IsEnd(); it++) {
		if (it->IsFunction()) {
			if (it->GetFunctionName() == name)
				return *it;
		}
	}
	return 0;
}


Node FindFn(Node n) {
	if (n.GetInt() == OP_EQ && n[0].GetType() == TYPE_FUNCTION && n[0].GetCount() == 1 && n[0][0].GetType() == TYPE_ID) {
		return n[0].CopyWithoutNodes();
	}
	return Node();
}

Node FindFnVar(Node n) {
	if (n.GetInt() == OP_EQ && n[0].GetType() == TYPE_FUNCTION && n[0].GetCount() == 1 && n[0][0].GetType() == TYPE_ID) {
		return n[0][0].CopyWithoutNodes();
	}
	return Node();
}


Node FindDiscontinuous(Node n) {
	Node::IteratorDeep it = n.BeginDeep();
	Node out = Set();
	for (;!it.IsEnd(); it++) {
		Node n = *it;
		if (n.IsFunction()) {
			if (n.GetCount() != 1) 
				continue;
			if (n.IsFuncSqrt()) {
				Node continuous = n[0] >= 0;
				if (Test(continuous))
					out.Add(continuous);
			}
			
		}
		else if (n.GetInt() == OP_DIV) {
			Node continuous = n[1] != 0;
			Node vars = FindVars(n[1]);
			if (vars.GetCount() == 0 || !Test(continuous))
				out.Add(continuous);
		}
		else if (n.GetInt() == OP_POW) {
			Node continuous1 = (n[1] < 1 && n[1] > -1 && n[1] >= 0);
			Node continuous2 = (n[1] >= -1 && n[1] < 0 && n[1] != 0);
			if (Test(continuous1))
				out.Add(continuous1);
			if (Test(continuous2))
				out.Add(continuous2);
		}
	}
	return out;
}

Node FindFunctionDomains(Node n) {
	if (IsExprFunc(n)) {
		return FindFunctionDomains(n[1]);
	}
	
	Node::IteratorDeep it = n.BeginDeep();
	Node out = Set();
	for (;!it.IsEnd(); it++) {
		Node n = *it;
		if (n.IsFunction()) {
			out.Add( BelongsTo( n[0], id("M_" + n.GetFunctionName())) );
		}
		else if (n.IsOp()) {
			int op = n.GetInt();
			if (op == OP_DIV) {
				out.Add( SpaceReal() & !(n[1] == 0).Solve() );
			}
		}
	}
	return out;
}


Node GetMulA(Node n) {
	if (n.GetCount() != 2) return Node();
	Node expr = n[0];
	Node find = n[1];
	Node::IteratorDeep it 	= expr.BeginDeep();
	int i = 0;
	for ( ;!it.IsEnd(); it++) {
		if (it->IsOp(OP_MUL)) {
			if ((*it)[1].Compare(find) == 0) {
				return (*it)[0];
			}
		}
	}
	return 0;
}

Node SolveInequation(Node n) {
	return Solve(n);
}

Node SolveStepsSum(Node expr, Node key, Node min, Node max) {
	int i_min = min.GetNumerical();
	int i_max = max.GetNumerical();
	
	bool first = true;
	Node out;
	for(int i = i_min; i <= i_max; i++) {
		Node tmp = expr;
		tmp.Replace(key, Int(i));
		out = Add(out, tmp);
	}
	return out;
}

Node GetElement(Node n, int i, Node key) {
	if (i < 0 || i >= n.GetCount() / 2) return Node();
	if (!n.IsArray()) return Node();
	return n[i] + key * n[n.GetCount() / 2 + i];
}


Node VectorLength(Node vec) {
	if (vec.GetCount() < 2) return Node();
	Node out = Add(Pow2(vec[0]), Pow2(vec[1]));
	for(int i = 2; i < vec.GetCount(); i++) {
		out = Add(out, Pow2(vec[i]));
	}
	return Sqrt(out);
}

void GetIds(Index<String>& ids, Node n) {
	Node::IteratorDeep it = n.BeginDeep();
	for (; !it.IsEnd(); it++) {
		if (it->IsId() && ids.Find(it->GetString()) == -1)
			ids.Add(it->GetString());
	}
}

Node GetUnusedId(Index<String>& ids) {
	for(int i = 0; i < 25; i++) {
		String var;
		var.Cat('a' + i);
		if (ids.Find(var) == -1)
			return id(var);
	}
	for(int i = 0; i < 25; i++) {
		for(int j = 0; j < 25; j++) {
			String var;
			var.Cat('a' + i);
			var.Cat('a' + j);
			if (ids.Find(var) == -1)
				return id(var);
		}
	}
	return Node();
}

Node GetUnusedId(Node n) {
	Index<String> ids;
	GetIds(ids, n);
	return GetUnusedId(ids);
}

Node GetUnusedId(Node n0, Node n1) {
	Index<String> ids;
	GetIds(ids, n0);
	GetIds(ids, n1);
	return GetUnusedId(ids);
}

Node GetUnusedId(Node n0, Node n1, Node n2) {
	Index<String> ids;
	GetIds(ids, n0);
	GetIds(ids, n1);
	GetIds(ids, n2);
	return GetUnusedId(ids);
}

Node RemoveInequality(Node n0) {
	Node out(n0);
	Node::IteratorDeep it = out.BeginDeep();
	for (; !it.IsEnd(); it++) {
		if (it->IsOp()) {
			int op = it->GetInt();
			if (op == OP_GRT || op == OP_LESS || op == OP_GREQ || op == OP_LSEQ || op == OP_INEQ) {
				it->SetOp(OP_EQ);
			}
		}
	}
	return out;
}

Node Split(Node n, int key) {
	Node out = Array();
	Node::IteratorDeep it = n.BeginDeep();
	int abs_count = 0;
	for (; !it.IsEnd(); it++) {
		if (it->IsFunction()) {
			int val = it->GetInt();
			if (val == key) {
				abs_count++;
			}
		}
	}
	if (abs_count == 0) {
		out.Add(n);
		return out;
	}
	
	if (abs_count > 16) return Node();
	int comb_count = (1 << abs_count);
	int comb = 0;
	for(int i = 0; i < comb_count; i++) {
		Vector<bool> v;
		v.SetCount(abs_count);
		for(int j = 0; j < abs_count; j++)
			v[j] = ((i >> j) & 1);
		out.Add(n);
		Node m = out[i];
		Node::IteratorDeep it = m.BeginDeep();
		int abs_count = 0;
		for (; !it.IsEnd();) {
			if (it->IsFunction()) {
				int val = it->GetInt();
				if (val == key) {
					bool invert = v[abs_count];
					if (invert) {
						*it = Multiply(-1, it->At(0));
					} else {
						*it = it->At(0);
					}
					abs_count++;
				}
			}
			it++;
		}
	}
	return out;
}

Node SplitAbs(Node n) {
	return Split(n, FUNC_ABS);
}

Node SplitPlusMinus(Node n) {
	return Split(n, FUNC_PLUSMIN);
}

Node JoinSpaces(Node sp1, Node sp2) {
	Node out;
	// TODO: all combinations should be checked... now it is not easy
	// TODO: vertaa koko ja laita viittauksiin toinen isommaksi aina
	if (sp1.IsFunction()) {
		if (sp2.IsOp()) {
			if (sp1.GetInt() == OP_BELONGS) {
				if (sp2.GetInt() == OP_BELONGS) {
					if (sp1[1].IsSpaceReal())
						return sp2;
					if (sp2[1].IsSpaceReal())
						return sp1;
				}
			}
		}
		else if (sp2.IsOp()) {
			if (sp2.GetInt() == OP_INEQ) {
				return sp2;
			}
		}
	} else {
		LOG(sp1.AsString());
		LOG(sp2.AsString());
		//Panic("Unknown space");
		return Array().Add(sp1).Add(sp2);
	}
	return out;
}

Node NegativeSpace(Node space) {
	// Init space
	Node out = Array().Add( SpaceReal() );
	
	//LOG(space.AsString());

	if (space.GetType() == TYPE_SET) {
		/*if (space.GetCount() == 1 && space[0].GetType() == TYPE_ARRAY) {
			Node arr = space[0];
			for(int i = 0; i < arr.GetCount(); i++) {
				if (arr[i].GetType() != TYPE_RANGE)
					throw Error("Unknown type in space");
				out = SpaceSubtract(out, arr[i]);
			}
			return out;
		}*/
		
		for(int i = 0; i < space.GetCount(); i++) {
			if (space[i].GetType() != TYPE_RANGE)
				throw Exc("Unknown type in space");
			out = SpaceSubtract(out, space[i]);
		}
	}
	else if (space.GetType() == TYPE_RANGE) {
		out = SpaceSubtract(out, space);
	}
	
	//Panic("Can't make negative space");
	return out;
}

Node SpaceSubtract(Node sp1, Node sp2) {
	//LOG(sp1.AsString());
	//LOG(sp2.AsString());
	if (sp1.GetType() == TYPE_ARRAY && sp1.GetCount() && sp2.GetType() == TYPE_RANGE) {
		Node out = sp1;
		for(int i = 0; i < out.GetCount(); i++) {
			Node a = out[i];
			a = Subtract(a, sp2);
			/*if (a.GetType() == TYPE_RANGE) {
				
			}*/
		}
		return out;
	}
	return Node();
}

Node ExprRangeToEquation(Node n) {
	Node out;
	if (n[2].GetInt())
		out = n[0] <= id("x");
	else
		out = n[0] < id("x");
	if (n[3].GetInt())
		out = out <= n[1];
	else
		out = out < n[1];
	return out;
}

Node ExprRangeToTest(Node n) {
	Node out;
	if (n[2].GetInt())
		out = n[0] <= id("x");
	else
		out = n[0] <  id("x");
	if (n[3].GetInt())
		out = out && (id("x") <= n[1]);
	else
		out = out && (id("x") <  n[1]);
	return out;
}

Node RangeValue(Node n) {
	Node min = n[0];
	Node max = n[1];
	Node out;
	if (min.Compare(NegInfInt()) == 0) {
		if (max.Compare(InfInt()) == 0) {
			return Int(0);
		} else {
			if (n[3].GetInt())
				return n[1];
			out = n[1] - Int(1);
		}
	} else {
		if (max.Compare(InfInt()) == 0) {
			if (n[2].GetInt())
				return n[0];
			out = n[0] + Int(1);
		} else {
			out = n[0] + (n[1] - n[0]) / 2;
		}
	}
	out.Simplify();
	return out;
}

Node ExprNormalizeVariables(Node n, int begin_char) {
	ArrayMap<String, String> ids;
	Node out = n;
	Node::IteratorDeep it = out.BeginDeep();
	for (;!it.IsEnd();it++) {
		if (it->IsId()) {
			String value = it->GetString();
			int i = ids.Find(value);
			if (i == -1) {
				int chr = begin_char + ids.GetCount();
				String id; id.Cat(chr);
				ids.Add(value, id);
				it->SetString(id);
			} else {
				it->SetString(ids[i]);
			}
		}
	}
	return out;
}

Node LaplaceTransformSplitted(Node expr, Node var, Node result_var) {
	UPP::Array<Node> nodes = expr.SplitLevelAdd();
	Node out;
	for(int i = 0; i < nodes.GetCount(); i++) {
		Node n = nodes[i];
		
		if (!i)
			out = LaplaceTransform(n, var, result_var);
		else
			out = out + LaplaceTransform(n, var, result_var);
	}
	return out;
}

}

