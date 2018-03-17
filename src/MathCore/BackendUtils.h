#ifndef _MathCore_BackendUtils_h_
#define _MathCore_BackendUtils_h_

#include "Parser.h"

namespace MathCore {

// Core
bool IsExprFunc(Node expr);
bool IsExprTrigFunc(Node expr);
bool IsExprTrigFuncSinCos(Node expr);
bool HasVar(Node n);
bool HasVar(Node n, Node var);
bool HasFunction(Node n, String name);
Node FindFunction(Node n, String name);
bool IsExprInequality(Node expr);
bool IsExprArrayEqual(Node expr);
bool IsConflictingSpaces(Node space1, Node space2);

String ChrStr(int i);
Node LineVec(Node line);
Node LinePt(Node line);
Node VectorExpr(Node n);
Node VectorPoints(Node a, Node b);
Node ExprVectorLength(Node vec);
Node ExprMultiplyVectors(Node vec0, Node vec1);
Node ExprAngleVectors(Node vec0, Node vec1);
Node ExprCrossVector(Node vec0, Node vec1);
Node ExprCrossVectorMatrix(Node mat);
Node ExprEqualVectors(Node vec0, Node vec1);
Node ExprLineVectorPoint(Node vec, Node pt);
Node ExprParamVecFormVector(Node vec);
Node ExprParamVecFormLine(Node line);
Node ExprParamCoordFormLine(Node line);
Node ExprCoordFormLine(Node line, String key="t");
Node ExprFuncPlace(Node func, Node replacing, bool keep_function=false);
Node ExprTrigFuncConvert(Node expr, int func);
Node ExprOpSwap(Node expr);
Node ExprJoinAddExp2(Node expr);
Node ExprArrayPositive(Node expr);
Node ExprCurveFromFunc(Node expr);
Node DeriveFunction(Node n);
Node DeriveFunction(Node n, Node key);
Node DeriveCurve(Node n);
Node Evaluate(Node n);

Node FindNode(Node n, Node cmp);
Node FindVar(Node n);
Node FindVars(Node n);
Node FindFn(Node n);
Node FindFnVar(Node n);
Node FindDiscontinuous(Node n);
Node FindFunctionDomains(Node n);
Node GetMulA(Node n);
Node SolveInequation(Node n); 
Node SolveStepsSum(Node expr, Node key, Node min, Node max);
Node GetElement(Node n, int i, Node key);
Node VectorLength(Node vec);
Node GetUnusedId(Node n);
Node GetUnusedId(Node n0, Node n1);
Node GetUnusedId(Node n0, Node n1, Node n2);
Node RemoveInequality(Node n0);
Node SplitAbs(Node n0);
Node SplitPlusMinus(Node n);
Node JoinSpaces(Node sp1, Node sp2);
Node NegativeSpace(Node space);
Node SpaceSubtract(Node sp1, Node sp2);
Node ExprRangeToEquation(Node n);
Node ExprRangeToTest(Node n);
Node RangeValue(Node n);
Node ExprNormalizeVariables(Node n, int begin_char);

Node LaplaceTransformSplitted(Node expr, Node var, Node result_var);

}


#endif
