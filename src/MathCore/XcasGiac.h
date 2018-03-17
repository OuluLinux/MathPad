#ifndef _MathCore_XcasGiac_h_
#define _MathCore_XcasGiac_h_

#include "BackendUtils.h"

namespace MathCore {
	
class ScriptRunner;
void RegisterFunctions_L0(ScriptRunner& proc);



// Tool
bool Test(Node n);
bool TestEqual(Node in0, Node in1);
bool IsFunctionIncreasing(Node n);
bool IsFunctionDecreasing(Node n, Node range);

Node Eliminate(Node list, Node var);
Node Solve(Node n);
Node SolveVar(Node expr, Node key);
Node SolveComplex(Node n);
Node SolveComplexVar(Node expr, Node key);
Node FSolve(Node n);
Node FSolveVar(Node expr, Node key);
Node SimplifySum(Node expr, Node key, Node min, Node max);
Node Simplify(Node n);
Node TypeSimplify(Node n);
Node Integrate(Node n);
Node Integrate(Node expr, Node min, Node max);
Node Integrate(Node expr, Node key);
Node Integrate(Node expr, Node key, Node min, Node max);
inline Node IntegrateRange(Node expr, Node key, Node min, Node max) {return Integrate(expr, key, min, max);}
Node Floating(Node n);
Node FindMin(Node expr, Node var);
Node FindMax(Node expr, Node var);
Node Limit(Node expr, Node key, Node closing);
Node LimitSimplify(Node expr, Node key, Node closing);
Node LinearSolve(Node expr_list, Node key_list);
Node GetVectorSpaceVector(Node space, Node vec);
//Node VectorLength(Node vec);
Node AngleVecs(Node vec0, Node vec1);
Node SimplifyVector(Node vec);
Node Solve2(Node expr0, Node expr1, Node key0, Node key1);
Node GetVectorCrossVectors(Node vec0, Node vec1);
Node SolveVector2(Node vec0, Node vec1, Node key0, Node key1);
Node LinePoint(Node line, Node point);
Node LineCross(Node line0, Node line1, Node key0, Node key1);
Node Ellipse(Node n);
Node Hyperbola(Node n);
Node Parabola(Node n);
Node Derive(Node n);
Node Derive(Node n, Node key);
inline Node DeriveVar(Node n, Node key) {return Derive(n, key);}
Node ATan2(Node x, Node y);
Node PartialFractionDecomposition(Node n);
Node PartialFractionDecomposition(Node n, Node key);
Node Factor(Node n);
Node Sum(Node expr, Node var, Node begin, Node end);
Node Real(Node n);
Node Imag(Node n);

Node ExpTrig(Node exp);
Node SolveDifferentialEquation(Node expr, Node var);
Node LaplaceTransform(Node expr, Node var);
Node LaplaceTransform(Node expr, Node var, Node result_var);
Node LaplaceTransformInversed(Node expr, Node var);
Node LaplaceTransformInversed(Node expr, Node var, Node result_var);
}


#endif
