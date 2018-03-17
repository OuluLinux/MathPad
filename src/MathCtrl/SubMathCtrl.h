#ifndef _MathCtrl_SubMathCtrl_h_
#define _MathCtrl_SubMathCtrl_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <MathCore/MathCore.h>

#include "ExprCtrl.h"
#include "Navigator.h"



namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;


class SubMathCtrl : public ExprCtrl {
	VectorMap<int, Color> type_colors;
	//enum {TYPE_OP, TYPE_ID, TYPE_IDSUB, TYPE_INT, TYPE_DOUBLE, TYPE_FUNCTION, TYPE_ERROR, 
	//	TYPE_PARENTHESIS, TYPE_LABEL, TYPE_TYPE, TYPE_OTHER};
	
	Upp::Array<SubMathCtrl> nodes;
	Image img;
	ImageDraw* idraw;
	short cursor, cursor_begin, cursor_end;
	Node* node = NULL;
	byte default_wantfocus;
	bool right_hand_focus;
	uint8 cursor_off;
	
	static int previous_key;
	
	void ConvertImage();
	void ClearImageDraw();
	SubMathCtrl& SetSource(Node& node);
	SubMathCtrl& SetRHand(bool b=true);
	
	bool HasStringCursor();
	
	void SetDefaultWantFocus(bool b) {default_wantfocus = b;}
	
	void ResizeContent(Size new_sz);
public:
	typedef SubMathCtrl CLASSNAME;
	
	
	SubMathCtrl(int width, int height, bool snap=true);
	SubMathCtrl(const SubMathCtrl& mc);
	SubMathCtrl();
	~SubMathCtrl();
	
	String AsString(int indent=0);
	
	Node* GetPtr() const;
	void SetPtr(Node* ptr);
	void SetCursorBegin(int i);
	void SetCursorEnd(int i);
	
	void SetRightHandFocus(bool b=true) {right_hand_focus = b;}
	bool IsRightHandFocus() const {return right_hand_focus;}
	
	int GetCount() const {return nodes.GetCount();}
	SubMathCtrl& operator [] (int i) {return nodes[i];}
	SubMathCtrl& At(int i) {return nodes[i];}
	
	bool IsDefaultWantFocus() const {return default_wantfocus;}
	void EnableSubFocus(bool b);
	void FocusSub(int i);
	void RefreshNode(Node* cursor_dest=0);
	
	SubMathCtrl& operator = (const SubMathCtrl& node);
	
	void SetNode(Node& node);
	void Redraw();
	void Render();
	Size GetRenderSize();
	
	
	virtual void Layout();
	virtual void Paint(Draw& d);
	
	virtual bool Key(dword key, int count);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);
	virtual void GotFocus();
	virtual void LostFocus();
	virtual void MouseEnter(Point p, dword keyflags);
	virtual void MouseLeave();
	
	static Image cursor_before_override;
	static bool cursor_is_overridden;
	
	SubMathCtrl Exp(Node& p, bool nobrackets=false);
	
	
	static Callback WhenNeedsRedraw, WhenNeedsFullRefresh, WhenCursorLeft, WhenCursorRight;
	
	
	class Cursor {
		Vector<int> node_pos;
		int text_pos;
		Node* ptr = NULL;
		SubMathCtrl* mc;
	public:
		Cursor() {text_pos = 0; ptr = 0; mc = 0;}
		Cursor(const Cursor& cursor) {*this = cursor;}
		Cursor& operator =(const Cursor& cur) {text_pos = cur.text_pos; node_pos <<= cur.node_pos; ptr = cur.ptr; mc = cur.mc; return *this;}
		void Add(int i) {node_pos.Add(i);}
		int GetCount() const {return node_pos.GetCount();}
		int operator [] (int i) const {return node_pos[i];}
		void SetTextPos(int i) {text_pos = i;}
		int GetTextPos() const {return text_pos;}
		void Remove(int i) {node_pos.Remove(i);}
		void Append(const Cursor& cur) {node_pos.Append(cur.node_pos); text_pos = cur.text_pos; if (cur.ptr) ptr = cur.ptr;}
		void SetPtr(Node* ptr) {this->ptr = ptr;}
		void SetSubMathCtrl(SubMathCtrl* mc) {this->mc = mc;}
		Node* GetPtr() const {return ptr;}
		SubMathCtrl* GetSubMathCtrl() const {return mc;}
		
		String AsString() const {
			String out = Format("%X", (int64)&*mc) + " : " + Format("%X", (int64)&*ptr) + " : ";
			for(int i = 0; i < node_pos.GetCount(); i++) out += IntStr(node_pos[i]) + " ";
			out += ": " + IntStr(text_pos);
			return out;
		}
	};
	
	static Callback1<Cursor> WhenNeedsLogicalCursor, WhenNeedsAbsoluteCursor;
	static Callback1<Node> WhenAnswer;
	
	Cursor GetCursor() const;
	int GetCursorX();
	void SetCursor(const Cursor& cursor);
	
	
	typedef Upp::Array<SubMathCtrl>::Iterator Iterator;
	typedef Upp::Array<SubMathCtrl>::ConstIterator ConstIterator;
	
	class IteratorDeep {
		friend class SubMathCtrl;
		Iterator begin;
		Vector<SubMathCtrl*> cur;
		Vector<int> pos;
		SubMathCtrl* begin_ptr;
	protected:
		IteratorDeep(SubMathCtrl* cur);
		IteratorDeep(const SubMathCtrl* cur);
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
		int64 GetCurrentAddr() {return (int64)cur[cur.GetCount() - 1];}
		
		bool IsEnd() {return pos.GetCount() == 1 && pos[0] == 1;}
		bool IsBegin() {return cur[cur.GetCount()-1] == begin_ptr;}
		bool operator == (IteratorDeep& it);
		bool operator != (IteratorDeep& it) {return !(*this == it);}
		void operator ++(int i);
		void operator --(int i);
		SubMathCtrl* operator->();
		SubMathCtrl& operator*();
		void IncNotDeep();
		SubMathCtrl* Higher();
	};
	
	IteratorDeep BeginDeep();
	
private:
	void GetCursorLoop(Cursor& cursor) const;
	void SetCursor(const Cursor& cursor, int pos);
	
	void DrawMath(int x, int y, SubMathCtrl& mc);
	void DrawMath(int x, int y, int w, int h, SubMathCtrl& mc);
	void DrawLine(int x0, int y0, int x1, int y1, int line_width, Color clr=Black());
	void DrawText(int x, int y, WString str, Font& fnt, Color clr=Black());
	void DrawPolygon(Vector<Point>& pts, Color clr=Black());
	
	SubMathCtrl Text(String text, int type, bool italic = false, int offsetX=0, int offsetY=0, Node src=Node());
	
	SubMathCtrl ParenthesisBracket(int height, bool right);
	SubMathCtrl ArrayBracket(int height, bool right);
	SubMathCtrl SetBracket(int height, bool right);
	SubMathCtrl AbsBracket(int height, bool right);
	
	SubMathCtrl SignMap(Node signmap);
	
	SubMathCtrl Range(Node min, Node max, Node min_eq, Node max_eq);
private:
	String Replace(String str, String find, String replace);
	String ReplaceSymbols(String var);
	SubMathCtrl Term(Node n, bool noBracket = false);	
	SubMathCtrl Mul(Node n);
	
private:
	SubMathCtrl Bracket(SubMathCtrl &data);
	SubMathCtrl Abs(SubMathCtrl &data);
	SubMathCtrl Sqrt(SubMathCtrl &right);
	SubMathCtrl Integral(SubMathCtrl &data, SubMathCtrl& key, SubMathCtrl &sub, SubMathCtrl &sup);
	SubMathCtrl IntegratedUnplaced(SubMathCtrl &data, SubMathCtrl &sub, SubMathCtrl &sup);
	SubMathCtrl Sumat(SubMathCtrl &data, SubMathCtrl &sub, SubMathCtrl &sup);
	SubMathCtrl Sumat(Node expr, Node key, Node begin, Node end);
	SubMathCtrl Limit(Node expr, Node key, Node limit);
	SubMathCtrl Exp(SubMathCtrl &data, SubMathCtrl &exp);
	SubMathCtrl Function(String function, SubMathCtrl &content);
	SubMathCtrl Equal(SubMathCtrl &left, SubMathCtrl &right);
	SubMathCtrl NumDenom(SubMathCtrl &num, SubMathCtrl &denom);
	
	SubMathCtrl OpTextBetween(SubMathCtrl& a, const String& chr, const String& size_str, Node b, bool brackets=false);
	SubMathCtrl OpTextBetween(SubMathCtrl& x, const String& chr, SubMathCtrl& y);
	
	SubMathCtrl JoinBottom(SubMathCtrl &left, SubMathCtrl &right);
	SubMathCtrl JoinCenter(SubMathCtrl &left, SubMathCtrl &right);
	SubMathCtrl JoinVert(SubMathCtrl &num, SubMathCtrl &denom, bool centered=false);
	SubMathCtrl JoinTriple(SubMathCtrl& left, SubMathCtrl& middle, SubMathCtrl& right);
	
	SubMathCtrl Expression(String str);

private:
	SubMathCtrl SubSup(SubMathCtrl &drwText, SubMathCtrl &drwSub, SubMathCtrl &drwSup);
	SubMathCtrl SubSup(String text, String sub, String sup, Node src=Node());
	SubMathCtrl SubSup(SubMathCtrl &drwText, String sub, String sup);
	SubMathCtrl SubSupInv(SubMathCtrl &drwText, SubMathCtrl &drwSub, SubMathCtrl &drwSup);
	SubMathCtrl SubSupInv(String text, String sub, String sup);
	SubMathCtrl SubSupInv(SubMathCtrl &drwText, String sub, String sup);
	SubMathCtrl JoinFlex(SubMathCtrl &left, SubMathCtrl &right);
	
private:
	VectorMap<String, WString> symbols;
	
	
	
	
	
	
	
	
	void MenuLocal(Bar& bar);
	void MenuShortcuts(Bar& bar);
	void MenuActions(Bar& bar);
	void MenuNumber(Bar& bar);
	void MenuAlgebra(Bar& bar);
	void MenuCalculus(Bar& bar);
	void MenuProbability(Bar& bar);
	void MenuStatistics(Bar& bar);
	void MenuMatrix(Bar& bar);
	void MenuFinance(Bar& bar);
	void FuncCatalog();
	
	void FuncSolveVar(String var);
	void FuncDeriveVar(String var);
	void FuncIntegrateVar(String var);
	
	void FuncAssignVariable();
	void FuncGetVariable();
	void FuncDeleteVariable();
	void FuncClearAZ();
	void FuncClearHistory();
	 
	void FuncConvertDecimal();
	void FuncApptroximateFraction();
	void FuncNumFactor();
	void FuncLeastCommonMultiple();
	void FuncGreatestCommonMultiple();
	void FuncRemainder();
	void MenuNumFractionTools(Bar& bar);
	void MenuNumberTools(Bar& bar);
	void MenuComplexNumberTools(Bar& bar);
	 
	void FuncSolve();
	void FuncFactor();
	void FuncExpand();
	void FuncZeros();
	void FuncCompleteSquare();
	void FuncNumericalSolve();
	void MenuSolveSystem(Bar& bar);
	void MenuPolynomialTools(Bar& bar);
	void MenuFractionTools(Bar& bar);
	void MenuConvertExpression(Bar& bar);
	void MenuTrigonometry(Bar& bar);
	void MenuComplex(Bar& bar);
	void MenuExtract(Bar& bar);
	 
	void FuncDerivative();
	void FuncDerivativePoint();
	void FuncIntegral();
	void FuncLimit();
	void FuncSum();
	void FuncProduct();
	void FuncMinimum();
	void FuncMaximum();
	void FuncTangentLine();
	void FuncNormalLine();
	void FuncArcLength();
	void MenuSeries(Bar& bar);
	void FuncDifferentialEquationSolver();
	void FuncImplicitDifferentiation();
	void MenuNumericalCalculations(Bar& bar);
	 
	void FuncFactorial();
	void FuncPermutations();
	void FuncCombinations();
	void RandomMenu(Bar& bar);
	void DistributionsMenu(Bar& bar);
	 
	void MenuStatCalculations(Bar& bar);
	void FuncStatResults();
	void MenuListMath(Bar& bar);
	void MenuListOperations(Bar& bar);
	void MenuDistributions(Bar& bar);
	void MenuConfidenceIntervals(Bar& bar);
	void MenuStatTests(Bar& bar);
	 
	void MenuCreate(Bar& bar);
	void FuncTranspose();
	void FuncDeterminant();
	void FuncRowEchelonForm();
	void FuncReducedRowEchelonForm();
	void FuncSimultaneous();
	void MenuNorms(Bar& bar);
	void MenuDimensions(Bar& bar);
	void MenuRowOperations(Bar& bar);
	void MenuElementOperations(Bar& bar);
	void MenuAdvanced(Bar& bar);
	void MenuVector(Bar& bar);
	 
	void FuncFinanceSolver();
	void MenuTVMFuncs(Bar& bar);
	void MenuAmortization(Bar& bar);
	void MenuCashFlows(Bar& bar);
	void MenuInterestConversion(Bar& bar);
	void FuncDaysBetweenDates();
	
	
	
	
	
	
	
	
	void FuncNumProperFraction();
	void FuncNumGetNumerator();
	void FuncNumGetDenominator();
	void FuncNumCommonDenominator();

	void FuncNumRound();
	void FuncIntegerPart();
	void FuncFractionalPart();
	void FuncSign();
	void FuncMod();
	void FuncFloor();
	void FuncCeiling();

	void FuncComplexConjugate();
	void FuncRealPart();
	void FuncImaginaryPart();
	void FuncPolarAngle();
	void FuncMagnitude();
	void FuncConvertPolar();
	void FuncConvertRectangular();

	void FuncSolveSystemEquations();
	void FuncSovleSystemLinearEquations();

	void FuncRootsOfPolynomial();
	void FuncRealRootsOfPolynomial();
	void FuncComplexRootsOfPolynomial();
	void FuncRemainderOfPolynomial();
	void FuncQuotientOfPolynomial();
	void FuncGreatestOfPolynomial();
	void FuncCoefficientOfPolynomial();
	void FuncDegreeOfPolynomial();

	void FuncProperFraction();
	void FuncGetNumerator();
	void FuncGetDenominator();
	void FuncCommonDenominator();

	void FuncConverToLn();
	void FuncConverToLogbase();
	void FuncConverToExp();
	void FuncConverToSin();
	void FuncConverToCos();

	void FuncTrigExpand();
	void FuncTrigCollect();

	void FuncComplexSolve();
	void FuncComplexFactor();
	void FuncComplexZeros();

	void FuncLeft();
	void FuncRight();

	void FuncTaylorPolynomial();
	void FuncGeneralizedSeries();
	void FuncDominantTerm();

	void FuncNumericalDerivativePoint();
	void FuncCentralDiffQuotStep();
	void FuncNumericalIntegral();
	void FuncNumericalMinimum();
	void FuncNumericalMaximum();

	void FuncRandomNumber();
	void FuncRandomInteger();
	void FuncRandomBinomial();
	void FuncRandomNormal();
	void FuncRandomSample();
	void FuncRandomSeed();

	void FuncDistNormalPdf();
	void FuncDistNormalCdf();
	void FuncDistInverseNormal();
	void FuncDistTPdf();
	void FuncDistTCdf();
	void FuncDistInverseT();
	void FuncDistX2Pdf();
	void FuncDistX2Cdf();
	void FuncDistInverseX2();
	void FuncDistFPdf();
	void FuncDistFCdf();
	void FuncDistInverseF();
	void FuncDistBinomialPdf();
	void FuncDistBinomialCdf();
	void FuncDistGeometricPdf();
	void FuncDistGeometricCdf();
	void FuncDistPoissonPdf();
	void FuncDistPoissonCdf();

	void FuncStatOneVariable();
	void FuncStatTwoVariable();
	void FuncStatLinearRegressionMXB();
	void FuncStatLinearRegressionABX();
	void FuncStatMedianMedianLine();
	void FuncStatQuadraticRegression();
	void FuncStatCubicRegression();
	void FuncStatQuarticRegression();
	void FuncStatPowerRegression();
	void FuncStatExpRegression();
	void FuncStatLogarithmicRegression();
	void FuncStatSinusoidalRegression();
	void FuncStatLogisticRegressionDZero();
	void FuncStatLogisticRegressionDNonZero();
	void FuncStatMultipleLinearRegression();
	void FuncStatCorrelationMatrix();

	void FuncStatMinimum();
	void FuncStatMaximum();
	void FuncStatMean();
	void FuncStatMedian();
	void FuncStatSumElements();
	void FuncStatSampleStdDeviation();
	void FuncStatSampleVariance();
	void FuncStatPopStdDev();
	void FuncStatPopVar();

	void FuncStatSortAsc();
	void FuncStatSortDesc();
	void FuncStatCumSumList();
	void FuncStatFill();
	void FuncStatSequence();
	void FuncStatDifferenceList();
	void FuncStatAugment();
	void FuncStatConvertListToMatrix();
	void FuncStatConvertMatrixToList();
	void FuncStatLeft();
	void FuncStatMid();
	void FuncStatRight();

	void FuncStatDistNormalPdf();
	void FuncStatDistNormalCdf();
	void FuncStatDistInverseNormal();
	void FuncStatDistTPdf();
	void FuncStatDistTCdf();
	void FuncStatDistInverseT();
	void FuncStatDistX2Pdf();
	void FuncStatDistX2Cdf();
	void FuncStatDistInverseX2();
	void FuncStatDistFPdf();
	void FuncStatDistFCdf();
	void FuncStatDistInverseF();
	void FuncStatDistBinomialPdf();
	void FuncStatDistBinomialCdf();
	void FuncStatDistGeometricPdf();
	void FuncStatDistGeometricCdf();
	void FuncStatDistPoissonPdf();
	void FuncStatDistPoissonCdf();

	void FuncStatZInterval();
	void FuncStatTInterval();
	void FuncStatTwoSampleZInterval();
	void FuncStatTwoSampleTInterval();
	void FuncStatOnePropZInterval();
	void FuncStatTwoPropZInterval();
	void FuncStatLinearRegTIntervals();
	void FuncStatMultipleRegIntervals();

	void FuncStatZTest();
	void FuncStatTTest();
	void FuncStatTwoSampleZTest();
	void FuncStatTwoSampleTTest();
	void FuncStatOnePropZTest();
	void FuncStatTwoPropZTest();
	void FuncStatX2GOF();
	void FuncStatX2TwoWayTest();
	void FuncStatTwoSampleFTest();
	void FuncStatLinearRegTTest();
	void FuncStatMultipleRegTests();
	void FuncStatANOVA();
	void FuncStatANOVATwoWay();

	void FuncMatrixCreateMatrix();
	void FuncMatrixCreateZeroMatrix();
	void FuncMatrixCreateIdentity();
	void FuncMatrixCreateDiagonal();
	void FuncMatrixCreateRandomMatrix();
	void FuncMatrixCreateFill();
	void FuncMatrixCreateSubmatrix();
	void FuncMatrixCreateAugment();
	void FuncMatrixCreateColumnAugment();
	void FuncMatrixCreateConstructMatrix();

	void FuncMatrixNormsNorm();
	void FuncMatrixNormsRow();
	void FuncMatrixNormsColumn();

	void FuncMatrixDimensionsMatrix();
	void FuncMatrixDimensionsRow();
	void FuncMatrixDimensionsColumn();

	void FuncMatrixRowOpSwap();
	void FuncMatrixRowOpAdd();
	void FuncMatrixRowOpMul();
	void FuncMatrixRowOpMulAdd();

	void FuncMatrixElOpAdd();
	void FuncMatrixElOpSub();
	void FuncMatrixElOpMul();
	void FuncMatrixElOpDiv();
	void FuncMatrixElOpPow();

	void FuncMatrixAdvTrace();
	void FuncMatrixAdvLUDecomp();
	void FuncMatrixAdvQRDecomp();
	void FuncMatrixAdvEigenvalues();
	void FuncMatrixAdvEigenvectors();
	void FuncMatrixAdvCharPoly();

	void FuncMatrixVecUnit();
	void FuncMatrixVecCross();
	void FuncMatrixVecDot();
	void FuncMatrixVecConvPolar();
	void FuncMatrixVecConvRect();
	void FuncMatrixVecConvCyl();
	void FuncMatrixVecConvSpher();

	void FuncFinTVMNumPeriods();
	void FuncFinTVMInterestRatePerYear();
	void FuncFinTVMPresentValue();
	void FuncFinTVMPaymentAmount();
	void FuncFinTVMFutureValue();

	void FuncFinAmortTable();
	void FuncFinAmortBalance();
	void FuncFinAmortInterestPaid();
	void FuncFinAmortPrincipalPaid();

	void FuncFinCashFlowNetPresentValue();
	void FuncFinCashFlowIntROR();
	void FuncFinCashFlowModIntROR();

	void FuncFinIntConvNominalInterestRate();
	void FuncFinIntConvEffectiveInterestRate();
	
	
	
	
	
	
	struct Error : public Exc                 {
		Error() {}
		Error(String s) {*(String*)this = s;}
	};
};


}

#endif
