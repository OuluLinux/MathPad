#ifndef _Xcas_CasEdit_h_
#define _Xcas_CasEdit_h_

/*

	CasEdit
	 - handles multiple ctrls (expressions, figures, graphs, etc.)
	 - handles groups of ctrls, hides/shows groups, moves them
	 - handles vertical resizing of ctrls
	 - loads/saves xml files (like original xwd format, but easier to implement)

*/

namespace Xcas {

class CasEdit : public ParentCtrl {
	
protected:
	friend class Editor;
	giac::context*	ctx = NULL;
	CasGroup		doc;
	ScrollBar		sb;
	
	
public:
	typedef CasEdit CLASSNAME;
	CasEdit();
	~CasEdit();
	
	virtual void	Layout();
	virtual void	MouseWheel(Point, int zdelta, dword);
	virtual bool	Key(dword key, int);
	
	void			Print();
	Editor&			NewExpression();
	CasItem*		GetSelectedItem();
	
	RichText		Get();
	String			GetXml();
	CasGroup&		GetSelectedGroup();
	
	void			SetXml(const String& xml);
	
	Callback		WhenRefreshBar;
	
};

}

#endif
