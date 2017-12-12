#ifndef _Xcas_Common_h_
#define _Xcas_Common_h_

namespace Xcas {


class CasEdit;
class CasGroup;


struct ItemLabel : public Ctrl {
	String label;
	bool is_selected = false;
	virtual void Paint(Draw& w);
};


class CasItem : public ParentCtrl {
	
protected:
	friend class CasGroup;
	friend class CasEdit;
	friend class Editor;
	
	ItemLabel label;
	int indent = 0;
	int height = 5;
	bool is_selected = false;
	CasGroup* group = NULL;
	CasEdit* edit = NULL;
	
	static const int handleh = 5;
	static const int min_height = 50;
	static const int label_width = 15;
	
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
	virtual int GetContentHeight() const {return height;}
	virtual void RefreshLabel() {}
	
	Callback WhenSplitFinish;
};


class CasGroup : public CasItem {
	
protected:
	Array<CasItem>	items;
	ScrollBar		sb;
	Button			expand;
	Label			title;
	
	
public:
	typedef CasGroup CLASSNAME;
	CasGroup();
	
	CasItem*		GetSelectedItem();
	CasGroup&		GetSelectedGroup();
	virtual int		GetContentHeight() const;
	
	virtual void	Layout();
	virtual void	RefreshLabel();
	
	template <class T> T& Add() {
		T* o = new T();
		o->group = this;
		o->edit = edit;
		items.Add(o);
		ParentCtrl::Add(*o);
		Layout();
		return *o;
	}
	
};


class GenOutput : public ParentCtrl {
	
public:
	//friend class Editor;
	giac::context* ctx = NULL;
	giac::gen value;
	
public:
	virtual void Set(giac::gen& g) {value = g;}
	virtual int GetContentHeight() {return 0;}
};


class CommentOutput : public ParentCtrl {
	Label lbl;
	
public:
	typedef CommentOutput CLASSNAME;
	CommentOutput();
	
	void Add(String comment);
	void Clear();
	
};


}

#endif
