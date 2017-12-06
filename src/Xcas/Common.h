#ifndef _Xcas_Common_h_
#define _Xcas_Common_h_

namespace Xcas {


struct ItemLabel : public Ctrl {
	String label;
	bool is_selected = false;
	virtual void Paint(Draw& w);
};


class CasItem : public ParentCtrl {
	
protected:
	friend class CasGroup;
	
	ItemLabel label;
	int indent = 0;
	int height = 5;
	bool is_selected = false;
	
	
	const int handleh = 5;
	
	
public:
	typedef CasItem CLASSNAME;
	CasItem();
	virtual ~CasItem();
	
	virtual void NewEntry() {}
	
	int  GetHandleY() const {return height - handleh;}
	bool IsSelected() const {return is_selected;}
	
	
	virtual void Layout();
	virtual void MouseMove(Point p, dword keyflags);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);
	virtual void Paint(Draw& w);
	
	
	
	Callback WhenSplitFinish;
};


class CasGroup : public CasItem {
	
protected:
	Array<CasItem> items;
	ScrollBar sb;
	Button expand;
	Label title;
	
	
public:
	typedef CasGroup CLASSNAME;
	CasGroup();
	
	CasItem* GetSelectedItem();
	CasGroup& GetSelectedGroup();
	
	virtual void Layout();
	
	template <class T> T& Add() {T* o = new T(); items.Add(o); ParentCtrl::Add(*o); Layout(); return *o;}
	
};


class GenOutput : public ParentCtrl {
	
	
};


class CommentOutput : public ParentCtrl {
	
	
};


}

#endif
