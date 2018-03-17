#ifndef _MathCtrl_ExprCtrl_h_
#define _MathCtrl_ExprCtrl_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <MathCore/MathCore.h>

namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;

class ExprParentCtrl;

class ExprCtrl {
protected:
	friend class ExprParentCtrl;
	Vector<ExprCtrl*> sub;
	Rect rect;
	bool want_focus;
	bool has_focus;
	bool visible;
	//bool changed;
	
	void* parent;
	byte parent_id;
	
	enum {PARENT_NONE, PARENT_EXPR, PARENT_PARENTEXPR};
	
public:
	
	ExprCtrl();
	
	//void SetDeepChangedRedraw(bool b);
	//bool IsChangedDeep() const;
	//void SetChanged(bool b=true);
	
	void Add(ExprCtrl& ec);
	void RemoveChild(ExprCtrl* ec);
	String AsString(int indent=0);
	
	void WantFocus(bool b=true);
	bool IsWantFocus();
	
	bool HasFocusDeep() const;
	bool HasFocus() const;
	
	
	void SetFocus();
	
	void Refresh();
	
	
	void SetRect(int x, int y, int w, int h);
	void SetRect(const Rect& r);
	
	void Show(bool b);
	bool IsVisible() const;
	
	Rect GetRect() const;
	Rect GetScreenRect() const;
	Size GetSize() const;
	
	ExprCtrl* GetParent();
	ExprParentCtrl* GetParentCtrl();
	TopWindow* GetTopWindow();
	
	//Image OverrideCursor(const Image& img);
	
	
	void RefreshLayout();
	
	virtual void Layout();
	virtual void Paint(Draw& d);
	virtual bool Key(dword key, int count);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);
	virtual void GotFocus();
	virtual void LostFocus();
	virtual void ChildGotFocus();
	virtual void ChildLostFocus();
	virtual void MouseEnter(Point p, dword keyflags);
	virtual void MouseLeave();
	virtual void MouseWheel(Point, int zdelta, dword);
	
	void LayoutDeep();
	void PaintDeep(Draw& d, int x, int y);
	bool KeyDeep(dword key, int count);
	void LeftDownDeep(Point p, dword keyflags, int x, int y);
	void RightDownDeep(Point p, dword keyflags, int x, int y);
	void GotFocusDeep();
	void LostFocusDeep();
	void MouseEnterDeep(Point p, dword keyflags);
	void MouseLeaveDeep();
	void MouseWheelDeep(Point pt, int zdelta, dword kf, int x, int y);
};


class ExprParentCtrl : public Ctrl {
	ExprCtrl* ec;
	
public:
	ExprParentCtrl();
	
	ExprCtrl* GetFocusExprCtrl();
	
	void Set(ExprCtrl& ec);
	
	virtual void Layout();
	virtual void Paint(Draw& d);
	
	virtual bool Key(dword key, int count);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);
	virtual void GotFocus();
	virtual void LostFocus();
	virtual void MouseEnter(Point p, dword keyflags);
	virtual void MouseLeave();
	virtual void MouseWheel(Point, int zdelta, dword);
};

}

#endif
