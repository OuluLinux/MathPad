#ifndef _Xcas_Graph_h_
#define _Xcas_Graph_h_

namespace Xcas {

class Graph : public GenOutput {
	
	
public:
	typedef Graph CLASSNAME;
	Graph();
	
	virtual void Paint(Draw& w);
	
};

}

#endif
