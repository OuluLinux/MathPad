#ifndef _SubMathCtrl_Navigator_h_
#define _SubMathCtrl_Navigator_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <MathCore/MathCore.h>



namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;

class SubMathCtrl;

class Navigator : public ParentCtrl {
	Upp::Id list_id;
	ArrayCtrl classes, list;
	DocEdit description;
	Splitter horz, vert;
	
	Label lsearch;
	EditString search;
	Button ok, cancel;
	
	SubMathCtrl* ptr;
	TopWindow* wndptr;
	
	bool tmp_loses_focus;
	
	void CompareAdd(const String& cb, const String& value, const String& value_incase, int id);
public:
	typedef Navigator CLASSNAME;
	Navigator();
	
	void RefreshData();
	void RefreshCursor();
	
	void CloseThis();
	void CloseAndAccept();
	void CloseAndAcceptExample(int i);
	
	void SetAssist();
	void SetDefault();
	void SetSubMathCtrl(SubMathCtrl* ptr);
	void Run();
	
	void DoFocusEdit();
	void DoSetFocus();
	void RecoverFocus();
	
	virtual void LostFocus();
	virtual bool Key(dword key, int count);
};



Navigator& NewNavigator();
void RemoveNavigator(Navigator* nav);
int GetNavigatorCount();

}

#endif
