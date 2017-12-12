#ifndef _Xcas_Equation_h_
#define _Xcas_Equation_h_

namespace Xcas {


class Equation : public GenOutput {
	
	
public:
	typedef Equation CLASSNAME;
	Equation();
	
	virtual void Paint(Draw& w);
	virtual void Layout() {Set(value); Refresh();}
	virtual void Set(giac::gen& g);
	virtual int GetContentHeight() {return 30;}
	
	
	// Compat
	//giac::eqwdata data;
	Size sz;
	int x() {return sz.cx/2;}
	int y() {return sz.cy/2;}
	int w() {return sz.cx;}
	int h() {return sz.cy;}
	void redraw() {}
	void remove_selection() {}
	
	giac::gen data;
	giac::attributs attr;
	int active_pos = 0, clip_x = 0, clip_y = 0, clip_w = 0, clip_h = 0;
	int begin_sel = 0, end_sel = 0;
	bool need_active_parse = 0;
};

}

#endif
