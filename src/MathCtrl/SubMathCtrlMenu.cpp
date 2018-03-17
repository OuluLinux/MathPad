#include "SubMathCtrl.h"

namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;

void SubMathCtrl::RightDown(Point p, dword keyflags) {
	if (IsWantFocus() && !HasFocus()) {
		SetFocus();
		Refresh();
	}
	MenuBar::Execute(THISBACK(MenuLocal));
}

void SubMathCtrl::MenuLocal(Bar& bar) {
	bar.Add("Shortcuts", THISBACK(MenuShortcuts));
	bar.Separator();
	bar.Add("Actions", THISBACK(MenuActions));
	bar.Add("Number", THISBACK(MenuNumber));
	bar.Add("Algebra", THISBACK(MenuAlgebra));
	bar.Add("Calculus", THISBACK(MenuCalculus));
	bar.Add("Probability", THISBACK(MenuProbability));
	bar.Add("Statistics", THISBACK(MenuStatistics));
	bar.Add("Matrix & Vector", THISBACK(MenuMatrix));
	bar.Add("Finance", THISBACK(MenuFinance));
	bar.Separator();
	bar.Add("Catalog", THISBACK(FuncCatalog));
	
}

void SubMathCtrl::MenuShortcuts(Bar& bar) {
	if (node && node->GetType() == TYPE_ID) {
		String var = node->GetString();
		if (var.GetCount()) {
			bar.Add("Solve for '" + var + "'", THISBACK1(FuncSolveVar, var));
			bar.Add("Derive for '" + var + "'", THISBACK1(FuncDeriveVar, var));
			bar.Add("Integrate for '" + var + "'", THISBACK1(FuncIntegrateVar, var));
		}
	}
}

void SubMathCtrl::FuncSolveVar(String var) {
	if (!node) return;
	Node* n = node;
	Node* par = n->GetInner();
	while (par) {
		n = par;
		par = n->GetInner();
	}
	Node& ans = *n;
	ans.Solve(id(var), true);
	WhenAnswer(ans);
}

void SubMathCtrl::FuncDeriveVar(String var) {
	if (!node) return;
	Node* n = node;
	Node* par = n->GetInner();
	while (par) {
		n = par;
		par = n->GetInner();
	}
	Node& ans = *n;
	ans.Derive(MathCore::id(var));
	WhenAnswer(ans);
}

void SubMathCtrl::FuncIntegrateVar(String var) {
	if (!node) return;
	Node* n = node;
	Node* par = n->GetInner();
	while (par) {
		n = par;
		par = n->GetInner();
	}
	Node& ans = *n;
	ans.Integrate(MathCore::id(var));
	WhenAnswer(ans);
}

void SubMathCtrl::MenuActions(Bar& bar) {
	bar.Add("Assign variable", THISBACK(FuncAssignVariable));
	bar.Add("Get variable", THISBACK(FuncGetVariable));
	bar.Separator();
	bar.Add("Delete variable", THISBACK(FuncDeleteVariable));
	bar.Add("Clear a-z...", THISBACK(FuncClearAZ));
	bar.Add("Clear history", THISBACK(FuncClearHistory));
	
}

void SubMathCtrl::MenuNumber(Bar& bar) {
	bar.Add("Convert to Decimal", THISBACK(FuncConvertDecimal));
	bar.Add("Approximate to Fraction", THISBACK(FuncApptroximateFraction));
	bar.Add("Factor", THISBACK(FuncNumFactor));
	bar.Add("Least Common Multiple", THISBACK(FuncLeastCommonMultiple));
	bar.Add("Greatest Common Multiple", THISBACK(FuncGreatestCommonMultiple));
	bar.Add("Remainder", THISBACK(FuncRemainder));
	bar.Add("Fraction Tools", THISBACK(MenuNumFractionTools));
	bar.Add("Number Tools", THISBACK(MenuNumberTools));
	bar.Add("Complex Number Tools", THISBACK(MenuComplexNumberTools));
	
	
}

void SubMathCtrl::MenuAlgebra(Bar& bar) {
	bar.Add("Solve", THISBACK(FuncSolve));
	bar.Add("Factor", THISBACK(FuncFactor));
	bar.Add("Expand", THISBACK(FuncExpand));
	bar.Add("Zeros", THISBACK(FuncZeros));
	bar.Add("Complete the Square", THISBACK(FuncCompleteSquare));
	bar.Add("Numerical Solve", THISBACK(FuncNumericalSolve));
	bar.Add("Solve System of Equations", THISBACK(MenuSolveSystem));
	bar.Add("Polynomial Tools", THISBACK(MenuPolynomialTools));
	bar.Add("Fraction Tools", THISBACK(MenuFractionTools));
	bar.Add("Convert Expression", THISBACK(MenuConvertExpression));
	bar.Add("Trigonometry", THISBACK(MenuTrigonometry));
	bar.Add("Complex", THISBACK(MenuComplex));
	bar.Add("Extract", THISBACK(MenuExtract));
	
}

void SubMathCtrl::MenuCalculus(Bar& bar) {
	bar.Add("Derivative", THISBACK(FuncDerivative));
	bar.Add("Derivative at a Point...", THISBACK(FuncDerivativePoint));
	bar.Add("Integral", THISBACK(FuncIntegral));
	bar.Add("Limit", THISBACK(FuncLimit));
	bar.Add("Sum", THISBACK(FuncSum));
	bar.Add("Product", THISBACK(FuncProduct));
	bar.Add("Function Minimum", THISBACK(FuncMinimum));
	bar.Add("Function Maximum", THISBACK(FuncMaximum));
	bar.Add("Tangent Line", THISBACK(FuncTangentLine));
	bar.Add("Normal Line", THISBACK(FuncNormalLine));
	bar.Add("Arc Length", THISBACK(FuncArcLength));
	bar.Add("Series", THISBACK(MenuSeries));
	bar.Add("Differential Equation Solver", THISBACK(FuncDifferentialEquationSolver));
	bar.Add("Implicit Differentiation", THISBACK(FuncImplicitDifferentiation));
	bar.Add("Numerical Calculations", THISBACK(MenuNumericalCalculations));
	
}

void SubMathCtrl::MenuProbability(Bar& bar) {
	bar.Add("Factorial (!)", THISBACK(FuncFactorial));
	bar.Add("Permutations", THISBACK(FuncPermutations));
	bar.Add("Combinations", THISBACK(FuncCombinations));
	bar.Add("Random", THISBACK(RandomMenu));
	bar.Add("Distributions", THISBACK(DistributionsMenu));
	
}

void SubMathCtrl::MenuStatistics(Bar& bar) {
	bar.Add("Stat Calculations", THISBACK(MenuStatCalculations));
	bar.Add("Stat Results", THISBACK(FuncStatResults));
	bar.Add("List Math", THISBACK(MenuListMath));
	bar.Add("List Operations", THISBACK(MenuListOperations));
	bar.Add("Distributions", THISBACK(MenuDistributions));
	bar.Add("Confidence Intervals", THISBACK(MenuConfidenceIntervals));
	bar.Add("Stat Tests", THISBACK(MenuStatTests));
	
}

void SubMathCtrl::MenuMatrix(Bar& bar) {
	bar.Add("Create", THISBACK(MenuCreate));
	bar.Add("Transpose", THISBACK(FuncTranspose));
	bar.Add("Determinant", THISBACK(FuncDeterminant));
	bar.Add("Row-Echelon Form", THISBACK(FuncRowEchelonForm));
	bar.Add("Reduced Row-Echelon Form", THISBACK(FuncReducedRowEchelonForm));
	bar.Add("Simultaneous", THISBACK(FuncSimultaneous));
	bar.Add("Norms", THISBACK(MenuNorms));
	bar.Add("Dimensions", THISBACK(MenuDimensions));
	bar.Add("Row Operations", THISBACK(MenuRowOperations));
	bar.Add("Element Operations", THISBACK(MenuElementOperations));
	bar.Add("Advanced", THISBACK(MenuAdvanced));
	bar.Add("Vector", THISBACK(MenuVector));
	
}

void SubMathCtrl::MenuFinance(Bar& bar) {
	bar.Add("Finance Solver...", THISBACK(FuncFinanceSolver));
	bar.Separator();
	bar.Add("TVNode Functions", THISBACK(MenuTVMFuncs));
	bar.Add("Amortization", THISBACK(MenuAmortization));
	bar.Add("Cash Flows", THISBACK(MenuCashFlows));
	bar.Add("Interest Conversion", THISBACK(MenuInterestConversion));
	bar.Add("Days between Dates", THISBACK(FuncDaysBetweenDates));
	
	
	
}














#define FN(x)

#define FUNCNUM(x, y) *node = MathCore::Function(x).Add(*node);\
	for(int i = 1; i < y; i++) node->Add(Void());\
	Cursor cur;\
	cur.SetPtr(&(*node)[0]);\
	cur.SetTextPos(0);\
	WhenNeedsRedraw();\
	WhenNeedsAbsoluteCursor(cur);\

#define FUNC(x) FUNCNUM(x, 1)

#define FUNC2(x) FUNCNUM(x, 2)
#define FUNC3(x) FUNCNUM(x, 3)
#define FUNC4(x) FUNCNUM(x, 4)
#define FUNC5(x) FUNCNUM(x, 5)

#define FUNC_NOVAR(x) *node = MathCore::Function(x);\
	Cursor cur;\
	cur.SetPtr(node);\
	cur.SetTextPos(0);\
	WhenNeedsRedraw();\
	WhenNeedsAbsoluteCursor(cur);\


void SubMathCtrl::FuncCatalog() {
	
}


void SubMathCtrl::FuncAssignVariable() {
	*node = Op(OP_ASSIGN).Add(*node).Add(Void());
	Cursor cur;
	cur.SetPtr(&node[1]);
	cur.SetTextPos(0);
	WhenNeedsRedraw();
	WhenNeedsAbsoluteCursor(cur);
}

void SubMathCtrl::FuncGetVariable() {
	// TODO: show var list
}


void SubMathCtrl::FuncDeleteVariable() {
	FUNC("DelVar");
}

void SubMathCtrl::FuncClearAZ() {
	FUNC_NOVAR("ClearAZ");
}

void SubMathCtrl::FuncClearHistory() {
	// TODO: clear screen
}

 
void SubMathCtrl::FuncConvertDecimal() {
	FUNC("Decimal");
}

void SubMathCtrl::FuncApptroximateFraction() {
	FUNC("ApproxFraction");
}

void SubMathCtrl::FuncNumFactor() {
	FUNC2("factor");
}

void SubMathCtrl::FuncLeastCommonMultiple() {
	FUNC2("lcm");
}

void SubMathCtrl::FuncGreatestCommonMultiple() {
	FUNC2("gcd");
}

void SubMathCtrl::FuncRemainder() {
	FUNC("remain");
}

void SubMathCtrl::MenuNumFractionTools(Bar& bar) {
	bar.Add("Proper Fraction", THISBACK(FuncNumProperFraction));
	bar.Add("Get Numerator", THISBACK(FuncNumGetNumerator));
	bar.Add("Get Denominator", THISBACK(FuncNumGetDenominator));
	bar.Add("Common Denominator", THISBACK(FuncNumCommonDenominator));
	
	
}

void SubMathCtrl::MenuNumberTools(Bar& bar) {
	bar.Add("Round", THISBACK(FuncNumRound));
	bar.Add("Integer Part", THISBACK(FuncIntegerPart));
	bar.Add("Fractional Part", THISBACK(FuncFractionalPart));
	bar.Add("Sign", THISBACK(FuncSign));
	bar.Add("Mod", THISBACK(FuncMod));
	bar.Add("Floor", THISBACK(FuncFloor));
	bar.Add("Ceiling", THISBACK(FuncCeiling));
	
}

void SubMathCtrl::MenuComplexNumberTools(Bar& bar) {
	bar.Add("Complex Conjugate", THISBACK(FuncComplexConjugate));
	bar.Add("Real Part", THISBACK(FuncRealPart));
	bar.Add("Imaginary Part", THISBACK(FuncImaginaryPart));
	bar.Add("Polar Angle", THISBACK(FuncPolarAngle));
	bar.Add("Magnitude", THISBACK(FuncMagnitude));
	bar.Add("Convert to Polar", THISBACK(FuncConvertPolar));
	bar.Add("Convert to Rectangular", THISBACK(FuncConvertRectangular));
	
}

 
void SubMathCtrl::FuncSolve() {
	FUNC2("solve");
}

void SubMathCtrl::FuncFactor() {
	FUNC2("factor");
}

void SubMathCtrl::FuncExpand() {
	FUNC2("expand");
}

void SubMathCtrl::FuncZeros() {
	FUNC2("zeros")
}

void SubMathCtrl::FuncCompleteSquare() {
	FUNC("CompleteSquare");
}

void SubMathCtrl::FuncNumericalSolve() {
	FUNC2("NSolve");
}

void SubMathCtrl::MenuSolveSystem(Bar& bar) {
	bar.Add("Solve System of Equations...", THISBACK(FuncSolveSystemEquations));
	bar.Add("Solve System of Linear Equations...", THISBACK(FuncSovleSystemLinearEquations));
	
}

void SubMathCtrl::MenuPolynomialTools(Bar& bar) {
	bar.Add("Find Roots of Polynomial...", THISBACK(FuncRootsOfPolynomial));
	bar.Separator();
	bar.Add("Real Roots of Polynomial", THISBACK(FuncRealRootsOfPolynomial));
	bar.Add("Complex Roots of Polynomial", THISBACK(FuncComplexRootsOfPolynomial));
	bar.Add("Remainder of Polynomial", THISBACK(FuncRemainderOfPolynomial));
	bar.Add("Quotient of Polynomial", THISBACK(FuncQuotientOfPolynomial));
	bar.Add("Greatest of Polynomial", THISBACK(FuncGreatestOfPolynomial));
	bar.Add("Coefficient of Polynomial", THISBACK(FuncCoefficientOfPolynomial));
	bar.Add("Degree of Polynomial", THISBACK(FuncDegreeOfPolynomial));
	
	
}

void SubMathCtrl::MenuFractionTools(Bar& bar) {
	bar.Add("Proper Fraction", THISBACK(FuncProperFraction));
	bar.Add("Get Numerator", THISBACK(FuncGetNumerator));
	bar.Add("Get Denominator", THISBACK(FuncGetDenominator));
	bar.Add("Common Denominator", THISBACK(FuncCommonDenominator));
	
	
}

void SubMathCtrl::MenuConvertExpression(Bar& bar) {
	bar.Add("Convert to ln", THISBACK(FuncConverToLn));
	bar.Add("Convert to logbase", THISBACK(FuncConverToLogbase));
	bar.Add("Convert to exp", THISBACK(FuncConverToExp));
	bar.Add("Convert to sin", THISBACK(FuncConverToSin));
	bar.Add("Convert to cos", THISBACK(FuncConverToCos));
	
}

void SubMathCtrl::MenuTrigonometry(Bar& bar) {
	bar.Add("Expand", THISBACK(FuncTrigExpand));
	bar.Add("Collect", THISBACK(FuncTrigCollect));
	
}

void SubMathCtrl::MenuComplex(Bar& bar) {
	bar.Add("Solve", THISBACK(FuncComplexSolve));
	bar.Add("Factor", THISBACK(FuncComplexFactor));
	bar.Add("Zeros", THISBACK(FuncComplexZeros));
	
}

void SubMathCtrl::MenuExtract(Bar& bar) {
	bar.Add("Left", THISBACK(FuncLeft));
	bar.Add("Right", THISBACK(FuncRight));
	
}

 
void SubMathCtrl::FuncDerivative() {
	FUNC2("derivative");
}

void SubMathCtrl::FuncDerivativePoint() {
	FUNC2("DerivativePoint");
}

void SubMathCtrl::FuncIntegral() {
	FUNC2("integral");
}

void SubMathCtrl::FuncLimit() {
	FUNC3("limit");
}

void SubMathCtrl::FuncSum() {
	FUNC3("sum");
}

void SubMathCtrl::FuncProduct() {
	FUNC4("ProdSeq");
}

void SubMathCtrl::FuncMinimum() {
	FUNC("FMin");
}

void SubMathCtrl::FuncMaximum() {
	FUNC("FMax");
}

void SubMathCtrl::FuncTangentLine() {
	FUNC3("TangentLine");
}

void SubMathCtrl::FuncNormalLine() {
	FUNC3("NormalLine");
}

void SubMathCtrl::FuncArcLength() {
	FUNC4("ArcLen");
}

void SubMathCtrl::MenuSeries(Bar& bar) {
	bar.Add("Taylor Polynomial", THISBACK(FuncTaylorPolynomial));
	bar.Add("Generalized Series", THISBACK(FuncGeneralizedSeries));
	bar.Add("Dominant Term", THISBACK(FuncDominantTerm));
	
}

void SubMathCtrl::FuncDifferentialEquationSolver() {
	FUNC3("DESolve");
}

void SubMathCtrl::FuncImplicitDifferentiation() {
	FUNC3("ImpDif");
}

void SubMathCtrl::MenuNumericalCalculations(Bar& bar) {
	bar.Add("Numerical Derivative at a Point", THISBACK(FuncNumericalDerivativePoint));
	bar.Add("Central Difference Quotient with Step", THISBACK(FuncCentralDiffQuotStep));
	bar.Add("Numerical Integral", THISBACK(FuncNumericalIntegral));
	bar.Add("Numerical Function Minimum", THISBACK(FuncNumericalMinimum));
	bar.Add("Numerical Function Maximum", THISBACK(FuncNumericalMaximum));
	
}

 
void SubMathCtrl::FuncFactorial() {
	//TODO: op factorial
}

void SubMathCtrl::FuncPermutations() {
	FUNC2("NPr");
}

void SubMathCtrl::FuncCombinations() {
	FUNC2("NCr");
}

void SubMathCtrl::RandomMenu(Bar& bar) {
	bar.Add("Number", THISBACK(FuncRandomNumber));
	bar.Add("Integer", THISBACK(FuncRandomInteger));
	bar.Add("Binomial", THISBACK(FuncRandomBinomial));
	bar.Add("Normal", THISBACK(FuncRandomNormal));
	bar.Add("Sample", THISBACK(FuncRandomSample));
	bar.Add("Seed", THISBACK(FuncRandomSeed));
	
	
}

void SubMathCtrl::DistributionsMenu(Bar& bar) {
	bar.Add("Normal Pdf...", THISBACK(FuncDistNormalPdf));
	bar.Add("Normal Cdf...", THISBACK(FuncDistNormalCdf));
	bar.Add("Inverse Normal...", THISBACK(FuncDistInverseNormal));
	bar.Add("t Pdf...", THISBACK(FuncDistTPdf));
	bar.Add("t Cdf...", THISBACK(FuncDistTCdf));
	bar.Add("Inverse t...", THISBACK(FuncDistInverseT));
	bar.Add("X^2 Pdf...", THISBACK(FuncDistX2Pdf));
	bar.Add("X^2 Cdf...", THISBACK(FuncDistX2Cdf));
	bar.Add("Inverse X^2...", THISBACK(FuncDistInverseX2));
	bar.Add("F Pdf...", THISBACK(FuncDistFPdf));
	bar.Add("F Cdf...", THISBACK(FuncDistFCdf));
	bar.Add("Inverse F...", THISBACK(FuncDistInverseF));
	bar.Add("Binomial Pdf...", THISBACK(FuncDistBinomialPdf));
	bar.Add("Binomial Cdf...", THISBACK(FuncDistBinomialCdf));
	bar.Add("Geometric Pdf...", THISBACK(FuncDistGeometricPdf));
	bar.Add("Geometric Cdf...", THISBACK(FuncDistGeometricCdf));
	bar.Add("Poisson Pdf...", THISBACK(FuncDistPoissonPdf));
	bar.Add("Poisson Cdf...", THISBACK(FuncDistPoissonCdf));
	
}

 
void SubMathCtrl::MenuStatCalculations(Bar& bar) {
	bar.Add("One-Variable Statistics...", THISBACK(FuncStatOneVariable));
	bar.Add("Two-Variable Statistics...", THISBACK(FuncStatTwoVariable));
	bar.Add("Linear Regression (mx+b)...", THISBACK(FuncStatLinearRegressionMXB));
	bar.Add("Linear Regression (a+bx)...", THISBACK(FuncStatLinearRegressionABX));
	bar.Add("Median-Median Line...", THISBACK(FuncStatMedianMedianLine));
	bar.Add("Quadratic Regression...", THISBACK(FuncStatQuadraticRegression));
	bar.Add("Cubic Regression...", THISBACK(FuncStatCubicRegression));
	bar.Add("Quartic Regression...", THISBACK(FuncStatQuarticRegression));
	bar.Add("Power Regression...", THISBACK(FuncStatPowerRegression));
	bar.Add("Exponential Regression...", THISBACK(FuncStatExpRegression));
	bar.Add("Logarithmic Regression...", THISBACK(FuncStatLogarithmicRegression));
	bar.Add("Sinusoidal Regression...", THISBACK(FuncStatSinusoidalRegression));
	bar.Add("Logistic Regression (d=0)...", THISBACK(FuncStatLogisticRegressionDZero));
	bar.Add("Logistic Regression (d!=0)...", THISBACK(FuncStatLogisticRegressionDNonZero));
	bar.Add("Multiple Linear Regression...", THISBACK(FuncStatMultipleLinearRegression));
	bar.Add("Correlation Matrix...", THISBACK(FuncStatCorrelationMatrix));
		
}

void SubMathCtrl::FuncStatResults() {
	FUNC_NOVAR("Stat.Results");
}

void SubMathCtrl::MenuListMath(Bar& bar) {
	bar.Add("Minimum", THISBACK(FuncStatMinimum));
	bar.Add("Maximum", THISBACK(FuncStatMaximum));
	bar.Add("Mean", THISBACK(FuncStatMean));
	bar.Add("Median", THISBACK(FuncStatMedian));
	bar.Add("Sum of Elements", THISBACK(FuncStatSumElements));
	bar.Add("Sample Standard Deviation", THISBACK(FuncStatSampleStdDeviation));
	bar.Add("Sample Variance", THISBACK(FuncStatSampleVariance));
	bar.Add("Population Standard Deviation", THISBACK(FuncStatPopStdDev));
	bar.Add("Population Variance", THISBACK(FuncStatPopVar));
	
}

void SubMathCtrl::MenuListOperations(Bar& bar) {
	bar.Add("Sort Ascending", THISBACK(FuncStatSortAsc));
	bar.Add("Sort Descending", THISBACK(FuncStatSortDesc));
	bar.Add("Cumulative Sum List", THISBACK(FuncStatCumSumList));
	bar.Add("Fill", THISBACK(FuncStatFill));
	bar.Add("Sequence", THISBACK(FuncStatSequence));
	bar.Add("Difference List", THISBACK(FuncStatDifferenceList));
	bar.Add("Augment", THISBACK(FuncStatAugment));
	bar.Add("Convert List to Matrix", THISBACK(FuncStatConvertListToMatrix));
	bar.Add("Convert Matrix to List", THISBACK(FuncStatConvertMatrixToList));
	bar.Add("Left", THISBACK(FuncStatLeft));
	bar.Add("Mid", THISBACK(FuncStatMid));
	bar.Add("Right", THISBACK(FuncStatRight));
	
	
}

void SubMathCtrl::MenuDistributions(Bar& bar) {
	bar.Add("Normal Pdf...", THISBACK(FuncStatDistNormalPdf));
	bar.Add("Normal Cdf...", THISBACK(FuncStatDistNormalCdf));
	bar.Add("Inverse Normal...", THISBACK(FuncStatDistInverseNormal));
	bar.Add("t Pdf...", THISBACK(FuncStatDistTPdf));
	bar.Add("t Cdf...", THISBACK(FuncStatDistTCdf));
	bar.Add("Inverse t...", THISBACK(FuncStatDistInverseT));
	bar.Add("X^2 Pdf...", THISBACK(FuncStatDistX2Pdf));
	bar.Add("X^2 Cdf...", THISBACK(FuncStatDistX2Cdf));
	bar.Add("Inverse X^2...", THISBACK(FuncStatDistInverseX2));
	bar.Add("F Pdf...", THISBACK(FuncStatDistFPdf));
	bar.Add("F Cdf...", THISBACK(FuncStatDistFCdf));
	bar.Add("Inverse F...", THISBACK(FuncStatDistInverseF));
	bar.Add("Binomial Pdf...", THISBACK(FuncStatDistBinomialPdf));
	bar.Add("Binomial Cdf...", THISBACK(FuncStatDistBinomialCdf));
	bar.Add("Geometric Pdf...", THISBACK(FuncStatDistGeometricPdf));
	bar.Add("Geometric Cdf...", THISBACK(FuncStatDistGeometricCdf));
	bar.Add("Poisson Pdf...", THISBACK(FuncStatDistPoissonPdf));
	bar.Add("Poisson Cdf...", THISBACK(FuncStatDistPoissonCdf));
	
}

void SubMathCtrl::MenuConfidenceIntervals(Bar& bar) {
	bar.Add("z Interval...", THISBACK(FuncStatZInterval));
	bar.Add("t Interval...", THISBACK(FuncStatTInterval));
	bar.Add("2-Sample z Interval...", THISBACK(FuncStatTwoSampleZInterval));
	bar.Add("2-Sample t Interval...", THISBACK(FuncStatTwoSampleTInterval));
	bar.Add("1-Prop z Interval...", THISBACK(FuncStatOnePropZInterval));
	bar.Add("2-Prop z Interval...", THISBACK(FuncStatTwoPropZInterval));
	bar.Add("Linear Reg t Intervals...", THISBACK(FuncStatLinearRegTIntervals));
	bar.Add("Multiple Reg Intervals...", THISBACK(FuncStatMultipleRegIntervals));
	
}

void SubMathCtrl::MenuStatTests(Bar& bar) {
	bar.Add("z Test...", THISBACK(FuncStatZTest));
	bar.Add("t Test...", THISBACK(FuncStatTTest));
	bar.Add("2-Sample z Test...", THISBACK(FuncStatTwoSampleZTest));
	bar.Add("2-Sample t Test...", THISBACK(FuncStatTwoSampleTTest));
	bar.Add("1-Prop z Test...", THISBACK(FuncStatOnePropZTest));
	bar.Add("2-Prop z Test...", THISBACK(FuncStatTwoPropZTest));
	bar.Add("X^2 GOF...", THISBACK(FuncStatX2GOF));
	bar.Add("X^2 2-way Test...", THISBACK(FuncStatX2TwoWayTest));
	bar.Add("2-Sample F Test...", THISBACK(FuncStatTwoSampleFTest));
	bar.Add("Linear Reg t Test...", THISBACK(FuncStatLinearRegTTest));
	bar.Add("Multiple Reg Tests...", THISBACK(FuncStatMultipleRegTests));
	bar.Add("ANOVA...", THISBACK(FuncStatANOVA));
	bar.Add("ANOVA 2-Way...", THISBACK(FuncStatANOVATwoWay));
	
}

 
void SubMathCtrl::MenuCreate(Bar& bar) {
	bar.Add("Matrix...", THISBACK(FuncMatrixCreateMatrix));
	bar.Add("Zero Matrix", THISBACK(FuncMatrixCreateZeroMatrix));
	bar.Add("Identity", THISBACK(FuncMatrixCreateIdentity));
	bar.Add("Diagonal", THISBACK(FuncMatrixCreateDiagonal));
	bar.Add("Random Matrix", THISBACK(FuncMatrixCreateRandomMatrix));
	bar.Add("Fill", THISBACK(FuncMatrixCreateFill));
	bar.Add("Submatrix", THISBACK(FuncMatrixCreateSubmatrix));
	bar.Add("Augment", THISBACK(FuncMatrixCreateAugment));
	bar.Add("Column Augment", THISBACK(FuncMatrixCreateColumnAugment));
	bar.Add("Construct Matrix", THISBACK(FuncMatrixCreateConstructMatrix));
	
}

void SubMathCtrl::FuncTranspose() {
	FUNC("Transpose");
}

void SubMathCtrl::FuncDeterminant() {
	FUNC("Det");
}

void SubMathCtrl::FuncRowEchelonForm() {
	FUNC("REF");
}

void SubMathCtrl::FuncReducedRowEchelonForm() {
	FUNC("REFRREF");
}

void SubMathCtrl::FuncSimultaneous() {
	FUNC2("Simult");
}

void SubMathCtrl::MenuNorms(Bar& bar) {
	bar.Add("Norm", THISBACK(FuncMatrixNormsNorm));
	bar.Add("Row", THISBACK(FuncMatrixNormsRow));
	bar.Add("Column", THISBACK(FuncMatrixNormsColumn));
	
}

void SubMathCtrl::MenuDimensions(Bar& bar) {
	bar.Add("Matrix", THISBACK(FuncMatrixDimensionsMatrix));
	bar.Add("Row", THISBACK(FuncMatrixDimensionsRow));
	bar.Add("Column", THISBACK(FuncMatrixDimensionsColumn));
	
}

void SubMathCtrl::MenuRowOperations(Bar& bar) {
	bar.Add("Swap Rows", THISBACK(FuncMatrixRowOpSwap));
	bar.Add("Row Add", THISBACK(FuncMatrixRowOpAdd));
	bar.Add("Multiply Row", THISBACK(FuncMatrixRowOpMul));
	bar.Add("Multiply Row & Add", THISBACK(FuncMatrixRowOpMulAdd));
	
}

void SubMathCtrl::MenuElementOperations(Bar& bar) {
	bar.Add("Dot Add", THISBACK(FuncMatrixElOpAdd));
	bar.Add("Dot Subtract", THISBACK(FuncMatrixElOpSub));
	bar.Add("Dot Multiply", THISBACK(FuncMatrixElOpMul));
	bar.Add("Dot Divide", THISBACK(FuncMatrixElOpDiv));
	bar.Add("Dot Power", THISBACK(FuncMatrixElOpPow));
	
	
}

void SubMathCtrl::MenuAdvanced(Bar& bar) {
	bar.Add("Trace", THISBACK(FuncMatrixAdvTrace));
	bar.Add("LU Decomposition", THISBACK(FuncMatrixAdvLUDecomp));
	bar.Add("QR Decomposition", THISBACK(FuncMatrixAdvQRDecomp));
	bar.Add("Eigenvalues", THISBACK(FuncMatrixAdvEigenvalues));
	bar.Add("Eigenvectors", THISBACK(FuncMatrixAdvEigenvectors));
	bar.Add("Characteristic Polynomial", THISBACK(FuncMatrixAdvCharPoly));
	
}

void SubMathCtrl::MenuVector(Bar& bar) {
	bar.Add("Unit Vector", THISBACK(FuncMatrixVecUnit));
	bar.Add("Cross Product", THISBACK(FuncMatrixVecCross));
	bar.Add("Dot Product", THISBACK(FuncMatrixVecDot));
	bar.Add("Convert to Polar", THISBACK(FuncMatrixVecConvPolar));
	bar.Add("Convert to Rectangular", THISBACK(FuncMatrixVecConvRect));
	bar.Add("Convert to Cylindrical", THISBACK(FuncMatrixVecConvCyl));
	bar.Add("Convert to Spherical", THISBACK(FuncMatrixVecConvSpher));
	
}

 
void SubMathCtrl::FuncFinanceSolver() {
	FUNCNUM("FinSolve", 8);
}

void SubMathCtrl::MenuTVMFuncs(Bar& bar) {
	bar.Add("Number of Periods", THISBACK(FuncFinTVMNumPeriods));
	bar.Add("Interest Rate per Year", THISBACK(FuncFinTVMInterestRatePerYear));
	bar.Add("Present Value", THISBACK(FuncFinTVMPresentValue));
	bar.Add("Payment Amount", THISBACK(FuncFinTVMPaymentAmount));
	bar.Add("Future Value", THISBACK(FuncFinTVMFutureValue));
	
}

void SubMathCtrl::MenuAmortization(Bar& bar) {
	bar.Add("Amortization Table", THISBACK(FuncFinAmortTable));
	bar.Add("Balance", THISBACK(FuncFinAmortBalance));
	bar.Add("Interest Paid", THISBACK(FuncFinAmortInterestPaid));
	bar.Add("Principal Paid", THISBACK(FuncFinAmortPrincipalPaid));
	
}

void SubMathCtrl::MenuCashFlows(Bar& bar) {
	bar.Add("Net Present Value", THISBACK(FuncFinCashFlowNetPresentValue));
	bar.Add("Internal Rate of Return", THISBACK(FuncFinCashFlowIntROR));
	bar.Add("Modified Internal Rate of Return", THISBACK(FuncFinCashFlowModIntROR));
	
}

void SubMathCtrl::MenuInterestConversion(Bar& bar) {
	bar.Add("Nominal Interest Rate", THISBACK(FuncFinIntConvNominalInterestRate));
	bar.Add("Effective Interest Rate", THISBACK(FuncFinIntConvEffectiveInterestRate));
	
}

void SubMathCtrl::FuncDaysBetweenDates() {
	FUNC2("DBD");
}





















void SubMathCtrl::FuncNumProperFraction() {
	FUNC2("PropFrac");
}

void SubMathCtrl::FuncNumGetNumerator() {
	FUNC("GetNum");
}

void SubMathCtrl::FuncNumGetDenominator() {
	FUNC("GetDenom");
}

void SubMathCtrl::FuncNumCommonDenominator() {
	FUNC2("ComDenom");
}


void SubMathCtrl::FuncNumRound() {
	FUNC("round");
}

void SubMathCtrl::FuncIntegerPart() {
	FUNC("IPart");
}

void SubMathCtrl::FuncFractionalPart() {
	FUNC("FPart");
}

void SubMathCtrl::FuncSign() {
	FUNC("sign");
}

void SubMathCtrl::FuncMod() {
	FUNC2("mod");
}

void SubMathCtrl::FuncFloor() {
	FUNC("floor");
}

void SubMathCtrl::FuncCeiling() {
	FUNC("ceiling");
}


void SubMathCtrl::FuncComplexConjugate() {
	FUNC("conj");
}

void SubMathCtrl::FuncRealPart() {
	FUNC("real");
}

void SubMathCtrl::FuncImaginaryPart() {
	FUNC("imag");
}

void SubMathCtrl::FuncPolarAngle() {
	FUNC("angle");
}

void SubMathCtrl::FuncMagnitude() {
	FUNC("magn");
}

void SubMathCtrl::FuncConvertPolar() {
	FUNC("ToPolar");
}

void SubMathCtrl::FuncConvertRectangular() {
	FUNC("ToRect");
}


void SubMathCtrl::FuncSolveSystemEquations() {
	//TODO: wizard
}

void SubMathCtrl::FuncSovleSystemLinearEquations() {
	//TODO: wizard
}


void SubMathCtrl::FuncRootsOfPolynomial() {
	//TODO: wizard
}

void SubMathCtrl::FuncRealRootsOfPolynomial() {
	FUNC2("PolyRoots");
}

void SubMathCtrl::FuncComplexRootsOfPolynomial() {
	FUNC2("CPolyRoots");
}

void SubMathCtrl::FuncRemainderOfPolynomial() {
	FUNC3("PolyRemainder");
}

void SubMathCtrl::FuncQuotientOfPolynomial() {
	FUNC3("PolyQuotient");
}

void SubMathCtrl::FuncGreatestOfPolynomial() {
	FUNC2("PolyGCD");
}

void SubMathCtrl::FuncCoefficientOfPolynomial() {
	FUNC2("PolyCoeffs");
}

void SubMathCtrl::FuncDegreeOfPolynomial() {
	FUNC2("PolyDegree");
}


void SubMathCtrl::FuncProperFraction() {
	FUNC2("PropFrac");
}

void SubMathCtrl::FuncGetNumerator() {
	FUNC("GetNum");
}

void SubMathCtrl::FuncGetDenominator() {
	FUNC("GetDenom");
}

void SubMathCtrl::FuncCommonDenominator() {
	FUNC("ComDenom");
}


void SubMathCtrl::FuncConverToLn() {
	FUNC("ToLn");
}

void SubMathCtrl::FuncConverToLogbase() {
	FUNC("ToLogBase");
}

void SubMathCtrl::FuncConverToExp() {
	FUNC("ToExp");
}

void SubMathCtrl::FuncConverToSin() {
	FUNC("ToSin");
}

void SubMathCtrl::FuncConverToCos() {
	FUNC("ToCos");
}


void SubMathCtrl::FuncTrigExpand() {
	FUNC("TExpand");
}

void SubMathCtrl::FuncTrigCollect() {
	FUNC("TCollect");
}


void SubMathCtrl::FuncComplexSolve() {
	FUNC2("CSolve");
}

void SubMathCtrl::FuncComplexFactor() {
	FUNC2("CFactor");
}

void SubMathCtrl::FuncComplexZeros() {
	FUNC2("CZeros");
}


void SubMathCtrl::FuncLeft() {
	FUNC("left");
}

void SubMathCtrl::FuncRight() {
	FUNC("right");
}


void SubMathCtrl::FuncTaylorPolynomial() {
	FUNC3("Taylor");
}

void SubMathCtrl::FuncGeneralizedSeries() {
	FUNC3("Series");
}

void SubMathCtrl::FuncDominantTerm() {
	FUNC2("DominantTerm");
}


void SubMathCtrl::FuncNumericalDerivativePoint() {
	FUNC2("NDerivative");
}

void SubMathCtrl::FuncCentralDiffQuotStep() {
	FUNC2("CentralDiff");
}

void SubMathCtrl::FuncNumericalIntegral() {
	FUNC4("NInt");
}

void SubMathCtrl::FuncNumericalMinimum() {
	FUNC2("NFMin");
}

void SubMathCtrl::FuncNumericalMaximum() {
	FUNC2("NFMax");
}


void SubMathCtrl::FuncRandomNumber() {
	FUNC_NOVAR("rand");
}

void SubMathCtrl::FuncRandomInteger() {
	FUNC2("RandInt");
}

void SubMathCtrl::FuncRandomBinomial() {
	FUNC2("RandBin");
}

void SubMathCtrl::FuncRandomNormal() {
	FUNC2("RandNorm");
}

void SubMathCtrl::FuncRandomSample() {
	FUNC2("RandSamp");
}

void SubMathCtrl::FuncRandomSeed() {
	FUNC_NOVAR("RandSeed");
}


void SubMathCtrl::FuncDistNormalPdf() {
	FUNC3("NormPDF");
}

void SubMathCtrl::FuncDistNormalCdf() {
	FUNC4("NormCDF");
}

void SubMathCtrl::FuncDistInverseNormal() {
	FN("InvNorm");
}

void SubMathCtrl::FuncDistTPdf() {
	FN("TPDF");
}

void SubMathCtrl::FuncDistTCdf() {
	FN("TCDF");
}

void SubMathCtrl::FuncDistInverseT() {
	FN("InvT");
}

void SubMathCtrl::FuncDistX2Pdf() {
	FN("X2PDF");
}

void SubMathCtrl::FuncDistX2Cdf() {
	FN("X2PDF");
}

void SubMathCtrl::FuncDistInverseX2() {
	FN("InvX2");
}

void SubMathCtrl::FuncDistFPdf() {
	FN("FPdf");
}

void SubMathCtrl::FuncDistFCdf() {
	FN("FCdf");
}

void SubMathCtrl::FuncDistInverseF() {
	FN("InvF");
}

void SubMathCtrl::FuncDistBinomialPdf() {
	FN("BinPDF");
}

void SubMathCtrl::FuncDistBinomialCdf() {
	FN("BinCDF");
}

void SubMathCtrl::FuncDistGeometricPdf() {
	FN("GeomPDF");
}

void SubMathCtrl::FuncDistGeometricCdf() {
	FN("GeomCDF");
}

void SubMathCtrl::FuncDistPoissonPdf() {
	FN("PoissPDF");
}

void SubMathCtrl::FuncDistPoissonCdf() {
	FN("PoissCDF");
}


void SubMathCtrl::FuncStatOneVariable() {
	FN("OneVar");
}

void SubMathCtrl::FuncStatTwoVariable() {
	FN("TwoVar");
}

void SubMathCtrl::FuncStatLinearRegressionMXB() {
	FN("LinRegMXB");
}

void SubMathCtrl::FuncStatLinearRegressionABX() {
	FN("LinRegABX");
}

void SubMathCtrl::FuncStatMedianMedianLine() {
	FN("MMLine");
}

void SubMathCtrl::FuncStatQuadraticRegression() {
	FN("QuadReg");
}

void SubMathCtrl::FuncStatCubicRegression() {
	FN("CubicReg");
}

void SubMathCtrl::FuncStatQuarticRegression() {
	FN("QuartReg");
}

void SubMathCtrl::FuncStatPowerRegression() {
	FN("PowReg");
}

void SubMathCtrl::FuncStatExpRegression() {
	FN("ExpReg");
}

void SubMathCtrl::FuncStatLogarithmicRegression() {
	FN("LogReg");
}

void SubMathCtrl::FuncStatSinusoidalRegression() {
	FN("SinusReg");
}

void SubMathCtrl::FuncStatLogisticRegressionDZero() {
	FN("LogRegDZero");
}

void SubMathCtrl::FuncStatLogisticRegressionDNonZero() {
	FN("LogRegDNonZero");
}

void SubMathCtrl::FuncStatMultipleLinearRegression() {
	FN("MulLinReg");
}

void SubMathCtrl::FuncStatCorrelationMatrix() {
	FN("CorrMat");
}


void SubMathCtrl::FuncStatMinimum() {
	FN("min");
}

void SubMathCtrl::FuncStatMaximum() {
	FN("max");
}

void SubMathCtrl::FuncStatMean() {
	FN("mean");
}

void SubMathCtrl::FuncStatMedian() {
	FN("median");
}

void SubMathCtrl::FuncStatSumElements() {
	FN("sum");
}

void SubMathCtrl::FuncStatSampleStdDeviation() {
	FN("StDevSamp");
}

void SubMathCtrl::FuncStatSampleVariance() {
	FN("VarSamp");
}

void SubMathCtrl::FuncStatPopStdDev() {
	FN("StDevPop");
}

void SubMathCtrl::FuncStatPopVar() {
	FN("VarPop");
}


void SubMathCtrl::FuncStatSortAsc() {
	FN("SortA");
}

void SubMathCtrl::FuncStatSortDesc() {
	FN("SortD");
}

void SubMathCtrl::FuncStatCumSumList() {
	FN("CumulativeSum");
}

void SubMathCtrl::FuncStatFill() {
	FN("Fill");
}

void SubMathCtrl::FuncStatSequence() {
	FN("seq");
}

void SubMathCtrl::FuncStatDifferenceList() {
	FN("DiffList");
}

void SubMathCtrl::FuncStatAugment() {
	FN("Augment");
}

void SubMathCtrl::FuncStatConvertListToMatrix() {
	FN("ListToMatrix");
}

void SubMathCtrl::FuncStatConvertMatrixToList() {
	FN("MatrixToList");
}

void SubMathCtrl::FuncStatLeft() {
	FN("left");
}

void SubMathCtrl::FuncStatMid() {
	FN("mid");
}

void SubMathCtrl::FuncStatRight() {
	FN("right");
}


void SubMathCtrl::FuncStatDistNormalPdf() {
	FUNC3("NormPDF");
}

void SubMathCtrl::FuncStatDistNormalCdf() {
	FUNC4("NormCDF");
}

void SubMathCtrl::FuncStatDistInverseNormal() {
	FN("InvNorm");
}

void SubMathCtrl::FuncStatDistTPdf() {
	FN("TPDF");
}

void SubMathCtrl::FuncStatDistTCdf() {
	FN("TCDF");
}

void SubMathCtrl::FuncStatDistInverseT() {
	FN("InvT");
}

void SubMathCtrl::FuncStatDistX2Pdf() {
	FN("X2PDF");
}

void SubMathCtrl::FuncStatDistX2Cdf() {
	FN("X2PDF");
}

void SubMathCtrl::FuncStatDistInverseX2() {
	FN("InvX2");
}

void SubMathCtrl::FuncStatDistFPdf() {
	FN("FPdf");
}

void SubMathCtrl::FuncStatDistFCdf() {
	FN("FCdf");
}

void SubMathCtrl::FuncStatDistInverseF() {
	FN("InvF");
}

void SubMathCtrl::FuncStatDistBinomialPdf() {
	FN("BinPDF");
}

void SubMathCtrl::FuncStatDistBinomialCdf() {
	FN("BinCDF");
}

void SubMathCtrl::FuncStatDistGeometricPdf() {
	FN("GeomPDF");
}

void SubMathCtrl::FuncStatDistGeometricCdf() {
	FN("GeomCDF");
}

void SubMathCtrl::FuncStatDistPoissonPdf() {
	FN("PoissPDF");
}

void SubMathCtrl::FuncStatDistPoissonCdf() {
	FN("PoissCDF");
}


void SubMathCtrl::FuncStatZInterval() {
	FN("ZInterval");
}

void SubMathCtrl::FuncStatTInterval() {
	FN("TInterval");
}

void SubMathCtrl::FuncStatTwoSampleZInterval() {
	FN("ZInterval_2Samp");
}

void SubMathCtrl::FuncStatTwoSampleTInterval() {
	FN("TInterval_2Samp");
}

void SubMathCtrl::FuncStatOnePropZInterval() {
	FN("ZInterval_1Prop");
}

void SubMathCtrl::FuncStatTwoPropZInterval() {
	FN("ZInterval_2Prop");
}

void SubMathCtrl::FuncStatLinearRegTIntervals() {
	FN("LinRegTIntervals");
}

void SubMathCtrl::FuncStatMultipleRegIntervals() {
	FN("MulRegIntervals");
}


void SubMathCtrl::FuncStatZTest() {
	FN("ZTest");
}

void SubMathCtrl::FuncStatTTest() {
	FN("TTest");
}

void SubMathCtrl::FuncStatTwoSampleZTest() {
	FN("ZTest_2Samp");
}

void SubMathCtrl::FuncStatTwoSampleTTest() {
	
}

void SubMathCtrl::FuncStatOnePropZTest() {
	
}

void SubMathCtrl::FuncStatTwoPropZTest() {
	
}

void SubMathCtrl::FuncStatX2GOF() {
	
}

void SubMathCtrl::FuncStatX2TwoWayTest() {
	
}

void SubMathCtrl::FuncStatTwoSampleFTest() {
	
}

void SubMathCtrl::FuncStatLinearRegTTest() {
	
}

void SubMathCtrl::FuncStatMultipleRegTests() {
	
}

void SubMathCtrl::FuncStatANOVA() {
	
}

void SubMathCtrl::FuncStatANOVATwoWay() {
	
}


void SubMathCtrl::FuncMatrixCreateMatrix() {
	
}

void SubMathCtrl::FuncMatrixCreateZeroMatrix() {
	
}

void SubMathCtrl::FuncMatrixCreateIdentity() {
	
}

void SubMathCtrl::FuncMatrixCreateDiagonal() {
	
}

void SubMathCtrl::FuncMatrixCreateRandomMatrix() {
	
}

void SubMathCtrl::FuncMatrixCreateFill() {
	
}

void SubMathCtrl::FuncMatrixCreateSubmatrix() {
	
}

void SubMathCtrl::FuncMatrixCreateAugment() {
	
}

void SubMathCtrl::FuncMatrixCreateColumnAugment() {
	
}

void SubMathCtrl::FuncMatrixCreateConstructMatrix() {
	
}


void SubMathCtrl::FuncMatrixNormsNorm() {
	
}

void SubMathCtrl::FuncMatrixNormsRow() {
	
}

void SubMathCtrl::FuncMatrixNormsColumn() {
	
}


void SubMathCtrl::FuncMatrixDimensionsMatrix() {
	
}

void SubMathCtrl::FuncMatrixDimensionsRow() {
	
}

void SubMathCtrl::FuncMatrixDimensionsColumn() {
	
}


void SubMathCtrl::FuncMatrixRowOpSwap() {
	
}

void SubMathCtrl::FuncMatrixRowOpAdd() {
	
}

void SubMathCtrl::FuncMatrixRowOpMul() {
	
}

void SubMathCtrl::FuncMatrixRowOpMulAdd() {
	
}


void SubMathCtrl::FuncMatrixElOpAdd() {
	
}

void SubMathCtrl::FuncMatrixElOpSub() {
	
}

void SubMathCtrl::FuncMatrixElOpMul() {
	
}

void SubMathCtrl::FuncMatrixElOpDiv() {
	
}

void SubMathCtrl::FuncMatrixElOpPow() {
	
}


void SubMathCtrl::FuncMatrixAdvTrace() {
	
}

void SubMathCtrl::FuncMatrixAdvLUDecomp() {
	
}

void SubMathCtrl::FuncMatrixAdvQRDecomp() {
	
}

void SubMathCtrl::FuncMatrixAdvEigenvalues() {
	
}

void SubMathCtrl::FuncMatrixAdvEigenvectors() {
	
}

void SubMathCtrl::FuncMatrixAdvCharPoly() {
	
}


void SubMathCtrl::FuncMatrixVecUnit() {
	
}

void SubMathCtrl::FuncMatrixVecCross() {
	
}

void SubMathCtrl::FuncMatrixVecDot() {
	
}

void SubMathCtrl::FuncMatrixVecConvPolar() {
	
}

void SubMathCtrl::FuncMatrixVecConvRect() {
	
}

void SubMathCtrl::FuncMatrixVecConvCyl() {
	
}

void SubMathCtrl::FuncMatrixVecConvSpher() {
	
}


void SubMathCtrl::FuncFinTVMNumPeriods() {
	
}

void SubMathCtrl::FuncFinTVMInterestRatePerYear() {
	
}

void SubMathCtrl::FuncFinTVMPresentValue() {
	
}

void SubMathCtrl::FuncFinTVMPaymentAmount() {
	
}

void SubMathCtrl::FuncFinTVMFutureValue() {
	
}


void SubMathCtrl::FuncFinAmortTable() {
	
}

void SubMathCtrl::FuncFinAmortBalance() {
	
}

void SubMathCtrl::FuncFinAmortInterestPaid() {
	
}

void SubMathCtrl::FuncFinAmortPrincipalPaid() {
	
}


void SubMathCtrl::FuncFinCashFlowNetPresentValue() {
	
}

void SubMathCtrl::FuncFinCashFlowIntROR() {
	
}

void SubMathCtrl::FuncFinCashFlowModIntROR() {
	
}


void SubMathCtrl::FuncFinIntConvNominalInterestRate() {
	
}

void SubMathCtrl::FuncFinIntConvEffectiveInterestRate() {
	
}


}
