#ifndef _Exercises_Exercises_h
#define _Exercises_Exercises_h

#include <CtrlLib/CtrlLib.h>
#include <MathCtrl/MathCtrl.h>
using namespace Upp;
using namespace MathCore;
using namespace MathCtrl;

class Exercises : public TopWindow {
	
public:
	typedef Exercises CLASSNAME;
	Exercises();
	
	::MathCtrl::MathCtrl mc;
	
	void HelloWorld();
	
	void ComplexAnalysis1();
	void ComplexAnalysis2();
	void ComplexAnalysis3();
	void ComplexAnalysis4();
	
};

#endif
