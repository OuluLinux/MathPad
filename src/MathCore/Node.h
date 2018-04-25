#ifndef _MathCore_Node_h_
#define _MathCore_Node_h_



namespace MathCore {
	
enum {
	OP_NULL, OP_ASSIGN, OP_PLUSEQ, OP_LIST,
	OP_ADD, OP_SUB, OP_MUL, OP_DIV,
	OP_POW, OP_CALL, OP_BELONGS, OP_FOLLOWS,
	OP_EQ, OP_INEQ, OP_GREQ, OP_LSEQ,
	OP_LESS, OP_GRT, OP_AND, OP_OR,
	OP_ISEQ, OP_LABEL, OP_SETAND, OP_SETOR,
	OP_SETORDER, OP_SETCOMPL,OP_INCRATIO
};

enum {
	UNARY_NULL, UNARY_NEG, UNARY_NEGSET
};

enum {
	TYPE_NULL, TYPE_INT, TYPE_DOUBLE, TYPE_TEXT, TYPE_ID, TYPE_VECTORID,
	TYPE_TYPE, TYPE_IMG, TYPE_OP, TYPE_UNARY, TYPE_FUNCTION, TYPE_PARENTHESIS,
	TYPE_ARRAY, TYPE_MATRIX, TYPE_SET, TYPE_LINEARSET, TYPE_SPACE, TYPE_RANGE,
	
	// Ctrl
	TYPE_LABEL, TYPE_ERROR, TYPE_IDSUB, TYPE_OTHER, TYPE_EXERCISE
};

enum {
	SPACE_REAL, SPACE_INTEGER, SPACE_NATURAL
};

enum {
	FUNC_UNKNOWN, FUNC_ABS, FUNC_LENGTH, FUNC_CONJUGATE,FUNC_DERIVE,
	FUNC_INTEGRATE, FUNC_INTEGRATED_UNPLACED,
	FUNC_SIN, FUNC_COS, FUNC_TAN, FUNC_ASIN, FUNC_ACOS, FUNC_ATAN,
	FUNC_SIGNMAP, FUNC_SUM, FUNC_LIMIT, FUNC_RESIDY,
	FUNC_PLUSMIN,
	FUNC_SQRT, FUNC_POW, FUNC_EXP,
	FUNC_FACTORIAL, FUNC_BINCOEFF,
	FUNC_LOG,
	FUNC_CROSS,
	FUNC_CUSTOM
};

enum {
	IMG_NULL, IMG_I
};

class Node {
	
public:
	Array<Node> nodes;
	
	typedef Array<Node>::Iterator Iterator;
	typedef Array<Node>::ConstIterator ConstIterator;
	
	class IteratorDeep {
		friend class Node;
		Iterator begin;
		Vector<Node*> cur;
		Vector<int> pos;
	protected:
		IteratorDeep(Node* cur);
		IteratorDeep(const Node* cur);
	public:
		IteratorDeep(const IteratorDeep& it) {
			*this = it;
		}
		IteratorDeep& operator = (const IteratorDeep& it) {
			begin = it.begin;
			cur <<= it.cur;
			pos <<= it.pos;
			return *this;
		}
		int GetDepth() {return pos.GetCount();}
		int64 GetCurrentAddr() {return (int64)&*cur[cur.GetCount() - 1];}
		
		bool IsEnd() {return pos.GetCount() == 1 && pos[0] == 1;}
		bool operator == (IteratorDeep& it);
		bool operator != (IteratorDeep& it) {return !(*this == it);}
		void operator ++(int i);
		void IncNotDeep();
		void DecToInner();
		
		Node* operator->();
		Node& operator*();
		Node* Inner();
	};
	
	class IteratorDeepest {
		friend class Node;
		struct OneNode : Moveable<OneNode> {
			Node* ptr = NULL;
			int depth, pos;
		};
		struct OneNodeSorter {
			bool operator()(const OneNode& a, const OneNode& b) const {return a.depth > b.depth || (a.depth == b.depth && a.pos < b.pos);}
		};
		Vector<OneNode> nodes;
		Vector<OneNode>::Iterator it;
		
		
	protected:
		IteratorDeepest(IteratorDeep deep);
	public:
		bool IsEnd() {return it == nodes.End();}
		bool operator == (IteratorDeepest& it);
		bool operator != (IteratorDeepest& it) {return !(*this == it);}
		void operator ++(int i);
		Node* operator->();
		Node& operator*();
	};
	
	IteratorDeepest BeginDeepest();
	IteratorDeep BeginDeep();
	const IteratorDeep BeginDeep() const;
	Iterator Begin();
	Iterator End();
	
protected:
	
	int int_data;
	double dbl_data;
	String str_data;
	int type;
	//Node* inner = NULL;
	
public:
	
	Node() {type = 0; int_data = 0; dbl_data = 0;}
	Node(int i) {type = TYPE_INT; int_data = i; dbl_data = 0;}
	Node(double d) {type = TYPE_DOUBLE; int_data = 0; dbl_data = d;}
	Node(String s) {type = TYPE_ID; int_data = 0; dbl_data = 0; str_data = s;}
	Node(const Node& m) {*this = m;}
	
	uint32 GetHashValue() const;
	void operator=(const Node& n);
	bool IsEqual(const Node& n);
	
	Node* GetInner() {return NULL;}
	Node& operator[](int i);
	Node& At(int i);
	const Node& AtConst(int i) const;
	Node CopyWithoutNodes();
	//Node DeepCopy();
	Node& Add();
	Node& Add(const Node& m);
	Node& AddId(String s) {return Add(Node(s));}
	Node& Insert(int pos, Node n);
	/*Node Print(String title);
	Node Print(const char* title);
	Node Print(bool lower=false);
	Node Print(String title, bool lower);
	Node PrintLower(String title) {return Print(title, true);}*/
	Node& Floating();
	Node& Solve();
	Node& Solve(String key, bool keep_equal_expr=false) {return Solve(Node(key), keep_equal_expr);}
	Node& Solve(Node key, bool keep_equal_expr=false);
	Node& SolveComplex();
	Node& SolveComplex(const Node key, bool keep_equal_expr=false);
	Node& FSolve();
	Node& FSolve(Node key, bool keep_equal_expr=false);
	Node& Simplify();
	Node& Calculate();
	Node& Factor();
	Node& TypeSimplify();
	Node& Parse(const String& expr);
	Node& Replace(String id, const Node& replacing) {return Replace(Node(id), replacing);}
	Node& Replace(Node& find, const Node& replacing);
	Node& Replace(Node& equation);
	Node& ReplaceEq(const Node equation) {return ReplaceEq(equation);}
	Node& Remove(Node* node);
	Node& Remove(int i);
	Node& Remove(const Node& n);
	Node& RemoveIds();
	Node& RemoveAllExceptOpId();
	Node& RemoveTypes();
	Node& SolveDomain();
	Node& SolveImage();
	Node& Derive();
	Node& Derive(const Node key);
	Node& Integrate();
	Node& Integrate(const Node key);
	Node& EraseSigns();
	Node& Evaluate();
	Node& Ceiling();
	//Node SimplifyStepsPrint(String s);
	//Node CalculateStepsPrint(String s);
	Node& GetMul(const Node other_in_mul);
	Node& FindMin(const Node key);
	Node& FindMax(const Node key);
	Node& FindArrayTwoEquals();
	Node& FindSinCos(bool must_have_var=false);
	Node& GetOpJoinNode(int op, bool accept_equal);
	Upp::Array<Node> SplitLevelAdd();
	Upp::Array<Node> SplitLevelArray();
	
	void Trim();
	void Normalize();
	int Compare(const Node& n);
	
	int GetCount() const {return nodes.GetCount();}
	int GetType() const {return type;}
	int GetInt() const {return int_data;}
	int GetOpLevel() const;
	double GetDouble() const {return dbl_data;}
	double GetNumber() const {if (type == TYPE_INT) return int_data; else return dbl_data;}
	double GetNumerical() const;
	String GetString() const {return str_data;}
	String GetFunctionName() const;
	String GetOpString() const;
	String GetTypeString() const;
	bool IsType(int i) const {return type == i;}
	bool IsArray() const {return type == TYPE_ARRAY;}
	bool IsMatrix() const {return type == TYPE_MATRIX;}
	bool IsSet() const {return type == TYPE_SET || type == TYPE_LINEARSET;}
	bool IsMultiple() const {return IsArray() || IsSet();}
	bool IsFunction() const {return type == TYPE_FUNCTION;}
	bool IsNumber() const {return type == TYPE_DOUBLE || type == TYPE_INT;}
	bool IsInt() const {return type == TYPE_INT;}
	bool IsDouble() const {return type == TYPE_DOUBLE;}
	bool IsString() const {return type == TYPE_TEXT;}
	bool IsId() const {return type == TYPE_ID || type == TYPE_VECTORID;}
	bool IsOp() const {return type == TYPE_OP;}
	bool IsNull() const {return type == TYPE_NULL;}
	bool IsParenthesis() const {return type == TYPE_PARENTHESIS;}
	bool IsUnary() const {return type == TYPE_UNARY;}
	bool IsType() const {return type == TYPE_TYPE;}
	bool IsRange() const {return type == TYPE_RANGE;}
	bool IsOp(int i) const {return int_data == i;}
	bool IsOpId(int i) const {return int_data == i;}
	bool IsUnary(int i) const {return int_data == i;}
	bool IsUnaryId(int i) const {return int_data == i;}
	bool IsAdd() const {return IsOp() && GetInt() == OP_ADD;}
	bool IsSubtract() const {return IsOp() && GetInt() == OP_SUB;}
	bool IsMultiply() const {return IsOp() && GetInt() == OP_MUL;}
	bool IsDivide() const {return IsOp() && GetInt() == OP_DIV;}
	bool IsPow() const {return IsOp() && GetInt() == OP_POW;}
	bool IsFuncSin() const {return type == TYPE_FUNCTION && int_data == FUNC_SIN;}
	bool IsFuncCos() const {return type == TYPE_FUNCTION && int_data == FUNC_COS;}
	bool IsFuncTan() const {return type == TYPE_FUNCTION && int_data == FUNC_TAN;}
	bool IsFuncATan() const {return type == TYPE_FUNCTION && int_data == FUNC_ATAN;}
	bool IsFuncAbs() const {return type == TYPE_FUNCTION && int_data == FUNC_ABS;}
	bool IsFuncDerive() const {return type == TYPE_FUNCTION && int_data == FUNC_DERIVE;}
	bool IsFuncIntegrate() const {return type == TYPE_FUNCTION && int_data == FUNC_INTEGRATE;}
	bool IsFuncSqrt() const {return type == TYPE_FUNCTION && int_data == FUNC_SQRT;}
	bool IsSpaceReal() const {return type == TYPE_SPACE && int_data == SPACE_REAL;}
	bool IsSpaceInteger() const {return type == TYPE_SPACE && int_data == SPACE_INTEGER;}
	bool IsSpaceNatural() const {return type == TYPE_SPACE && int_data == SPACE_NATURAL;}
	bool IsSetAnd() const {return type == TYPE_OP && int_data == OP_SETAND;}
	bool IsSetOr() const {return type == TYPE_OP && int_data == OP_SETOR;}
	bool IsSetOrder() const {return type == TYPE_OP && int_data == OP_SETORDER;}
	bool IsSetNegate() const {return type == TYPE_UNARY && int_data == UNARY_NEGSET;}
	bool IsSetComplement() const {return type == TYPE_OP && int_data == OP_SETCOMPL;}
	bool IsNegate() const {return type == TYPE_UNARY && int_data == UNARY_NEG;}
	bool IsSIUnit() const;
	bool IsTypeUsingString() const;
	
	void SetOp(int op);
	void SetType(int type);
	void SetInt(int i);
	void SetDataInt(int i);
	void SetDouble(double d);
	void SetDataDouble(double d);
	void SetString(String str);
	void SetDataString(String str);
	void SetNull();
	void SetFunction(int f);
	void SetFunction(const String& s);
	
	String AsString(int indent=0);
	String AsInlineString() const;
	String AsDataInlineString() const;
	
};



Node operator && (Node a, Node b);
Node operator || (Node a, Node b);
Node operator == (Node a, Node b);
Node operator == (int a, Node b);
Node operator != (Node a, Node b);
Node operator >= (Node a, Node b);
Node operator <= (Node a, Node b);
Node operator > (Node a, Node b);
Node operator < (Node a, Node b);
Node operator + (Node a, Node b);
Node operator - (Node a, Node b);
Node operator - (Node a);
Node operator * (Node a, Node b);
Node operator * (String a, Node b);
Node operator * (Node a, String s);
Node operator / (Node a, Node b);
Node operator ^ (Node a, Node b);
Node operator & (Node a, Node b);
Node operator | (Node a, Node b);
Node operator ! (Node a);

Node Void();
Node Int(int i);
Node Double(double d);
Node Text(String s);
Node id(String s);
inline Node V(String s) {return id(s);}
inline Node MathId(String s) {return id(s);}
Node VectorId(String s);
Node Type(Node n, Node type);
Node Imaginary();
Node Function(String fn_name);
inline Node MathFunction(String fn_name) {return Function(fn_name);}
inline Node MFunction(String fn_name) {return Function(fn_name);}
inline Node MFn(String fn_name) {return Function(fn_name);}
Node Function(int fn);
Node Array();
Node Array(int size);
Node Array(Node x, Node y);
Node Array(Node x, Node y, Node z);
inline Node MathArray() {return Array();}
inline Node MArray() {return Array();}
Node Parenthesis();
Node Range(Node min, Node max, bool also_min_equal, bool also_max_equal);
Node RangeReal();
Node RangeInteger();
Node RangeRealNotInf();
Node RangeIntegerNotInf();
Node RangeNatural();
Node Matrix(int rows, int cols);
Node MatrixAlpha(int rows, int cols);
Node SpaceReal();
Node SpaceInteger();
Node Coordinate(Node x, Node y);
Node RealNumbers();
Node Set();
Node LinearSet();
Node SetOrder(Node first, Node second);
Node Subscript(Node id, Node id_sub);
Node BelongsTo(Node element, Node space);
Node Follows(Node var, Node dist);
Node Op(int op);
Node Unary(int unary_op);
Node Add(Node in0, Node in1);
Node Subtract(Node in0, Node in1);
Node PlusMinus(Node in0);
Node Multiply(Node in0, Node in1);
Node Divide(Node in0, Node in1);
Node VectorAdd(Node vec0, Node vec1);
inline Node MathVectorAdd(Node vec0, Node vec1) {return VectorAdd(vec0, vec1);}
Node VectorSubtract(Node vec0, Node vec1);
Node VectorMultiply(Node vec0, Node vec1);
Node VectorDivide(Node vec0, Node vec1);
Node VectorSum(Node vec0);
Node CrossProduct(Node in0, Node in1);
Node Fraction(int in0, int in1);
Node Negate(Node in0);
Node MultiplyImaginary(Node in0);
Node Conjugate(Node in0);
Node Absolute(Node in0);
Node Abs(Node in0);
Node Length(Node in0);
inline Node MathLength(Node in0) {return Length(in0);}
inline Node MLength(Node in0) {return Length(in0);}
Node Pow2(Node in0);
Node Pow10(Node in0);
Node Pow(Node in0, Node in1);
Node Sqrt(Node in0);
Node Log(Node in0);
Node Factorial(Node in);
Node BinomialCoefficient(Node n, Node k);
Node Parenthesis(Node in0);
Node FunctionDerive(Node in0);
Node FunctionDerive(Node in0, Node key);
Node FunctionIntegrate(Node in0, Node key, Node begin, Node end);
Node FunctionIntegrate(Node in0, Node key);
inline Node FunctionIntegrateRange(Node in0, Node key, Node begin, Node end) {return FunctionIntegrate(in0, key, begin, end);}
Node FunctionIntegrateUnplace(Node in0, Node key, Node begin, Node end);
Node FunctionSum(Node expr, Node var, Node begin, Node end);
Node FunctionLimit(Node expr, Node var, Node limit);
Node FunctionResidy(Node expr, Node var, Node limit);
Node FunctionProbability(String id);
Node FunctionProbability(Node n);
Node Prob(const String& id);
Node Prob(Node n);
Node Sin(Node expr);
Node Cos(Node expr);
Node Tan(Node expr);
Node ArcSin(Node expr);
Node ArcCos(Node expr);
Node ArcTan(Node expr);
Node IncrementRatio(Node a, Node b);
Node SetNegate(Node in0);
Node SetComplement(Node in0, Node in1);
Node Pi(); // 3.141 592 653 589 
Node Euler(); // 2.718 281 828 459
Node EulerMascheroni(); // 0.577 215 664 901
Node Pythagora(); // sqrt(2) = 1.414 213 562 373
Node GoldenRatio(); // 1.618 033 988 749
Node InverseGoldenRatio(); // 0.618 033 988 749
Node SilverRatio(); // 2.414 213 562 373
Node Inf();
Node NegInf();
Node InfInt();
Node NegInfInt();
Node InfReal();
Node NegInfReal();
Node LineCircle(Node radius=Int(1));
Node AreaSquare(Node side);
Node AreaRect(Node side0, Node side1);
Node AreaTrapezoid(Node side0, Node side1, Node height);
Node AreaCircle(Node radius);
Node AreaEllipse(Node radius0, Node radius1);
Node AreaTriangle(Node side, Node height);
Node AreaTriangle(Node side0, Node side1, Node side2);
inline Node AreaTriangleSides(Node side0, Node side1, Node side2) {return AreaTriangle(side0, side1, side2);}
Node AreaCube(Node side);
Node AreaPrism(Node area, Node perimeter, Node height);
Node AreaSphere(Node radius);
Node VolumeCube(Node height);
Node VolumeRectPrism(Node x, Node y, Node z);
Node VolumeIrregularPrism(Node area, Node height);
Node VolumeCylinder(Node radius, Node height);
Node VolumePyramid(Node side, Node height);
Node VolumeCone(Node radius, Node height);
Node VolumeSphere(Node radius=id("r"));
Node VolumeEllipsoid(Node r1, Node r2, Node r3);
Node VolumeError();


}


#endif
