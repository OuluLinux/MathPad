#ifndef _MathCtrl_MathCtrl_h_
#define _MathCtrl_MathCtrl_h_



#include "SubMathCtrl.h"


namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;

class MathCtrl;

class MathListItem {
	Node data;
	int depth;
	bool hidden_deeper;
public:
	MathListItem() {hidden_deeper = false; depth = -1;}
	
	MathListItem& Set(int depth, Node data) {this->data = data; this->depth = depth; return *this;}
	void SkipDeeper(bool b=true) {hidden_deeper = b;}
	
	Node& GetNode() {return data;}
	int GetDepth() {return depth;}
	bool IsSkipDeeper() {return hidden_deeper;}
	
};

struct CalcLine : public ExprCtrl {
	typedef CalcLine CLASSNAME;
	
	CalcLine();
	~CalcLine();
	Size GetPrefferedSize();
	
	void EditMode(bool b);
	
	int GetItemId() {return item_id;}
	
	void SetExpression(MathCore::Node n);
	void SetText(const String& str);
	void SetDebug(const String& str);
	void SetError(const String& str);
	void SetItemId(int i) {item_id = i;}
	
	void SetDepth(int i) {depth = i;}
	void Redraw();
	
	virtual void Layout();
	virtual void Paint(Draw& d);
	virtual bool Key(dword key, int count);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void GotFocus();
	virtual void LostFocus();
	virtual void ChildLostFocus();
	
	struct CursorPos : public Moveable<CursorPos> {
		Vector<int> nodepos;
		int x, y, w, h;
	};
	
	static bool edit_enabled;
	
	SubMathCtrl draw;
	
	Node node;
	Vector<CursorPos> curpos;
	int item_id;
	int depth;
	int x_offset, y_offset;
	
	int GetXOffset() const {return x_offset;}
	void SetXOffset(int i) {x_offset = i;}
	
	int GetYOffset() const {return y_offset;}
	void SetYOffset(int i) {y_offset = i;}
};

class CalcBackground : public ExprCtrl {
	int scroll;
public:
	typedef CalcBackground CLASSNAME;
	CalcBackground();
	
	virtual void Paint(Draw& d);
	void SetScroll(int i) {scroll = i;}
	
	Callback3<Point, int, dword> WhenMouseWheel;
	
	
	virtual void MouseWheel(Point pt, int zdelta, dword kf) {WhenMouseWheel(pt, zdelta, kf);}
	
};

class CalcDocument : public ExprCtrl {
	
protected:
	friend class MathCtrl;
	
	Upp::Array<CalcLine> lines;
	ScrollBar sb;
	CalcBackground bg;
	
	double total_zoom_factor;
	
	int root_depth;
	
	Upp::Array<MathCore::Node> tmp_nodes;
	Upp::Array<MathListItem> list;
	
	void DoRedrawSelected();
	void DoFullRefresh();
	void DoLogicalCursor(SubMathCtrl::Cursor cursor);
	void DoAbsoluteCursor(SubMathCtrl::Cursor cursor);
	void DoRefreshLayout() {RefreshLayout();}
	void DoCheckCursorVisibility();
	//void DoCheckDraw();
	
	int last_line;
	
	void AddNode(const MathCore::Node n);
public:
	typedef CalcDocument CLASSNAME;
	CalcDocument();
	
	void AddScrollBar();
	
	int GetCount() {return lines.GetCount();}
	
	int GetCursor();
	void SetCursor(int i);
	
	void GoLeft();
	
	virtual bool Key(dword key, int count);
	virtual void MouseWheel(Point, int zdelta, dword);
	virtual void Layout();
	virtual void ChildGotFocus();
	virtual void GotFocus();
	virtual void LostFocus();
	
	void Scroll();
	void ScrollToEnd();
	void PostRefreshLayout();
	void RefreshScrollBar();
	void RedrawSelected();
	void FullRefresh();
	void LogicalCursor(SubMathCtrl::Cursor cursor);
	void AbsoluteCursor(SubMathCtrl::Cursor cursor);
	void CursorLeft();
	void CursorRight();
	void GotAnswer(Node n);
	void CheckCursorVisibility();
	//void CheckDraw();
	void CollapseAll();
	
	void RefreshLastLineFocus();
	
	void Begin();
	void End();
	void RefreshLines();
	
	void Expand();
	void Collapse();
	
	//void InsertLines(int i, Node m);
	void RemoveLines(int i, int count);
	
	
	void AddLine() {AddNode(MathCore::Void()); lines.Top().SetFocus();}
	
	//ProgCalc* calc;
};

class MathCtrl : public ParentCtrl {
	
	CalcDocument doc;
	ExprParentCtrl docwrap;
	int output_depth;
	
public:
	typedef MathCtrl CLASSNAME;
	MathCtrl();
	
	//void AddLine() {doc.AddLine();}
	void FocusDocument();
	void Refresh();
	int  GetCursor() {return doc.GetCursor();}
	void SetCursor(int i) {doc.SetCursor(i);}
	
	void Print(String title, Node math);
	void Exercise(String title);
	void Inner(String title);
	void Outer();
	
	CalcDocument& GetDocument() {return doc;}
	
	virtual bool Key(dword key, int count);
	
};

}

#endif
