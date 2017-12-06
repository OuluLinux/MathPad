#ifndef _Xcas_Equation_h_
#define _Xcas_Equation_h_

namespace Xcas {

class Equation : public GenOutput {
	
	
public:
	typedef Equation CLASSNAME;
	Equation();
	
	virtual void Paint(Draw& w);
	
};

}

#endif
