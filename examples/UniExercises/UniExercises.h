#ifndef _UniExercises_UniExercises_h
#define _UniExercises_UniExercises_h

#include <CtrlLib/CtrlLib.h>
#include <MathCtrl/MathCtrl.h>
using namespace Upp;
using namespace MathCore;
using namespace MathCtrl;

class UniExercises : public TopWindow {
	
public:
	typedef UniExercises CLASSNAME;
	UniExercises();
	
	::MathCtrl::MathCtrl mc;
	
};

#endif
