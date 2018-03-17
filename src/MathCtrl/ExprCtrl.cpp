#include "ExprCtrl.h"

#undef DLOG
//#define DLOG(x) LOG(x);
#define DLOG(x);

namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;
	
	
ExprCtrl::ExprCtrl() {
	want_focus = true;
	has_focus = false;
	visible = true;
	//changed = true;
	parent = 0;
	parent_id = PARENT_NONE;
}

/*bool ExprCtrl::IsChangedDeep() const {
	if (changed) return true;
	
	for(int i = 0; i < sub.GetCount(); i++) {
		if (sub[i]->IsChangedDeep())
			return true;
	}
	
	return false;
}

void ExprCtrl::SetChanged(bool b) {
	changed = b;
}*/
	
void ExprCtrl::Add(ExprCtrl& ec) {
	DLOG("ExprCtrl::Add " << Format("%X", (int64)&ec));
	for(int i = 0; i < sub.GetCount(); i++) {
		ASSERT(sub[i] != &ec);
	}
	sub.Add(&ec);
	ec.parent = (void*)this;
	ec.parent_id = PARENT_EXPR;
}

void ExprCtrl::RemoveChild(ExprCtrl* ec) {
	for(int i = 0; i < sub.GetCount(); i++) {
		ExprCtrl* e = sub[i];
		if (e == ec) {
			sub.Remove(i);
			i--;
			e->parent = 0;
		}
	}
}

String ExprCtrl::AsString(int indent) {
	String out;
	for(int i = 0; i < indent; i++)
		out.Cat(' ');
	out << "| " << Format("%X", (int64)this) << " " << rect.ToString() << " shown=" << (int)visible << " want_focus=" << (int)want_focus << " has_focus=" << (int)has_focus << "\n";
	for(int i = 0; i < sub.GetCount(); i++) {
		out << sub[i]->AsString(indent+1);
	}
	return out;
}

void ExprCtrl::WantFocus(bool b) {
	want_focus = b;
}

bool ExprCtrl::IsWantFocus() {
	return want_focus;
}


bool ExprCtrl::HasFocusDeep() const {
	if (has_focus)
		return true;
	for(int i = 0; i < sub.GetCount(); i++) {
		bool hf = sub[i]->HasFocusDeep();
		if (hf)
			return true;
	}
	return false;
}

bool ExprCtrl::HasFocus() const {
	return has_focus;
}



void ExprCtrl::SetFocus() {
	if (has_focus)
		return;
	
	//if (!want_focus) {
		/*ExprCtrl* ec = GetParent();
		while (ec) {
			if (ec->want_focus) {
				ec->SetFocus();
				break;
			}
			ec = ec->GetParent();
		}*/
		//return;
	//}
	
	ExprParentCtrl* e = GetParentCtrl();
	ASSERT(e); // if fail, "Set" ExprCtrl to ExprParentCtrl
	
	e->SetFocus();
	ExprCtrl* ec = e->GetFocusExprCtrl();
	if (ec) {
		ec->has_focus = false;
		ec->LostFocus();
		ec = ec->GetParent();
		while (ec) {
			ec->ChildLostFocus();
			ec = ec->GetParent();
		}
	}
	
	ec = this;
	ec->has_focus = true;
	ec->GotFocus();
	ec = ec->GetParent();
	while (ec) {
		ec->ChildGotFocus();
		ec = ec->GetParent();
	}
	e->Refresh();
}


void ExprCtrl::Refresh() {
	ExprParentCtrl* ec = GetParentCtrl();
	if (ec)
		ec->Refresh();
}



void ExprCtrl::SetRect(int x, int y, int w, int h) {
	rect = RectC(x, y, w, h);
}

void ExprCtrl::SetRect(const Rect& r) {
	rect = r;
}


void ExprCtrl::Show(bool b) {
	visible = b;
}

bool ExprCtrl::IsVisible() const {
	return visible;
}

Rect ExprCtrl::GetRect() const {
	return rect;
}

Rect ExprCtrl::GetScreenRect() const {
	Size sz = rect.GetSize();
	int x = rect.left, y = rect.top;
	
	void* p = parent;
	int id = parent_id;
	while (p && id == PARENT_EXPR) {
		ExprCtrl* ec = (ExprCtrl*)p;
		Rect r = ec->GetRect();
		x += r.left;
		y += r.top;
		p = ec->parent;
		id = ec->parent_id;
	}
	
	if (p && id == PARENT_PARENTEXPR) {
		ExprParentCtrl* ec = (ExprParentCtrl*)p;
		Rect r = ec->GetScreenRect();
		x += r.left;
		y += r.top;
	}
	
	return RectC(x, y, sz.cx, sz.cy);
}

Size ExprCtrl::GetSize() const {
	return rect.GetSize();
}


ExprCtrl* ExprCtrl::GetParent() {
	if (parent_id==PARENT_EXPR)
		return (ExprCtrl*)parent;
	return 0;
}

ExprParentCtrl* ExprCtrl::GetParentCtrl() {
	void* p = parent;
	int id = parent_id;
	while (p && id == PARENT_EXPR) {
		ExprCtrl* ec = (ExprCtrl*)p;
		p = ec->parent;
		id = ec->parent_id;
	}
	
	if (p && id == PARENT_PARENTEXPR) {
		ExprParentCtrl* ec = (ExprParentCtrl*)p;
		return ec;
	}
	return 0;
}

TopWindow* ExprCtrl::GetTopWindow() {
	ExprParentCtrl* ec = GetParentCtrl();
	if (ec) {
		return ec->GetTopWindow();
	}
	
	return 0;
}


/*Image ExprCtrl::OverrideCursor(const Image& img) {
	
}*/

void ExprCtrl::RefreshLayout() {
	Layout();
	Refresh();
}

void ExprCtrl::Layout() {}
void ExprCtrl::Paint(Draw& d) {}
bool ExprCtrl::Key(dword key, int count) {return false;}
void ExprCtrl::LeftDown(Point p, dword keyflags) {}
void ExprCtrl::RightDown(Point p, dword keyflags) {}
void ExprCtrl::GotFocus() {}
void ExprCtrl::LostFocus() {}
void ExprCtrl::ChildGotFocus() {}
void ExprCtrl::ChildLostFocus() {}
	
void ExprCtrl::MouseEnter(Point p, dword keyflags) {}
void ExprCtrl::MouseLeave() {}
void ExprCtrl::MouseWheel(Point pt, int zdelta, dword kf) {}

void ExprCtrl::LayoutDeep() {
	Layout();
	for(int i = 0; i < sub.GetCount(); i++) {
		sub[i]->LayoutDeep();
	}
}

void ExprCtrl::PaintDeep(Draw& d, int x, int y) {
	DLOG("ExprCtrl::PaintDeep " << rect.ToString());
	Paint(d);
	//changed = false;
	for(int i = 0; i < sub.GetCount(); i++) {
		ExprCtrl& ec = *sub[i];
		if (!ec.IsVisible())
			continue;
		Rect r = ec.GetRect();
		int x2 = x+r.left;
		int y2 = y+r.top;
		DLOG("   " << i << " x=" << x2 << " y=" << y2);
		//d.Offset(x2, y2);
		d.Offset(r.left, r.top);
		ec.PaintDeep(d, x2, y2);
		d.End();
		//d.Offset(x, y);
		//d.Offset(x, y);
		//d.End();
		//d.Offset(x,y);
	}
	
}

bool ExprCtrl::KeyDeep(dword key, int count) {
	//for(int i = 0; i < sub.GetCount(); i++) {
	for(int i = sub.GetCount()-1; i >= 0; i--) {
		ExprCtrl* e = sub[i];
		if (!e->IsVisible())
			continue;
		if (!e->HasFocusDeep())
			continue;
		if (e->KeyDeep(key, count))
			return true;
	}
	if (Key(key, count))
		return true;
	return false;
}

void ExprCtrl::LeftDownDeep(Point p, dword keyflags, int x, int y) {
	DLOG("ExprCtrl::LeftDownDeep " << p.ToString() << " " << rect.ToString());
	Size sz = rect.GetSize();
	for(int i = sub.GetCount()-1; i >= 0; i--) {
		ExprCtrl* e = sub[i];
		if (!e->IsVisible())
			continue;
		Rect r = e->GetRect();
		int left = x + r.left;
		int top = y + r.top;
		int right = x + r.right;
		int bottom = y + r.bottom;
		if ((p.x >= r.left && p.x < r.right) && (p.y >= r.top && p.y < r.bottom)) {
			Point p2(p.x - r.left, p.y - r.top);
			e->LeftDownDeep(p2, keyflags, left, top);
			return;
		}
	}
	LeftDown(p, keyflags);
}

void ExprCtrl::RightDownDeep(Point p, dword keyflags, int x, int y) {
	DLOG("ExprCtrl::RightDownDeep " << p.ToString() << " " << rect.ToString());
	Size sz = rect.GetSize();
	for(int i = sub.GetCount()-1; i >= 0; i--) {
		ExprCtrl* e = sub[i];
		if (!e->IsVisible())
			continue;
		Rect r = e->GetRect();
		int left = x + r.left;
		int top = y + r.top;
		int right = x + r.right;
		int bottom = y + r.bottom;
		if ((p.x >= r.left && p.x < r.right) && (p.y >= r.top && p.y < r.bottom)) {
			Point p2(p.x - r.left, p.y - r.top);
			e->RightDownDeep(p2, keyflags, left, top);
			return;
		}
	}
	RightDown(p, keyflags);
}

void ExprCtrl::GotFocusDeep() {
	
}

void ExprCtrl::LostFocusDeep() {
	
}

void ExprCtrl::MouseEnterDeep(Point p, dword keyflags) {
	
}

void ExprCtrl::MouseLeaveDeep() {
	
}



void ExprCtrl::MouseWheelDeep(Point p, int zdelta, dword kf, int x, int y) {
	LOG("ExprCtrl::MouseWheelDeep " << p.ToString() << " " << rect.ToString());
	Size sz = rect.GetSize();
	for(int i = sub.GetCount()-1; i >= 0; i--) {
		ExprCtrl* e = sub[i];
		if (!e->IsVisible())
			continue;
		Rect r = e->GetRect();
		int left = x + r.left;
		int top = y + r.top;
		int right = x + r.right;
		int bottom = y + r.bottom;
		if ((p.x >= r.left && p.x < r.right) && (p.y >= r.top && p.y < r.bottom)) {
			Point p2(p.x - r.left, p.y - r.top);
			e->MouseWheelDeep(p, zdelta, kf, left, top);
			return;
		}
	}
	MouseWheel(p, zdelta, kf);
}


ExprParentCtrl::ExprParentCtrl() {
	ec = 0;
	WantFocus();
}

ExprCtrl* ExprParentCtrl::GetFocusExprCtrl() {
	ExprCtrl* e = ec;
	while (e) {
		if (e->has_focus)
			return e;
		int count = e->sub.GetCount();
		ExprCtrl* e3 = 0;
		for(int i = 0; i < count; i++) {
			ExprCtrl* e2 = e->sub[i];
			if (e2->HasFocusDeep()) {
				e3 = e2;
				break;
			}
		}
		e = e3;
	}
	return e;
}

void ExprParentCtrl::Set(ExprCtrl& ec) {
	this->ec = &ec;
	ec.parent = (void*)this;
	ec.parent_id = ExprCtrl::PARENT_PARENTEXPR;
}

void ExprParentCtrl::Layout() {
	if (!ec) return;
	DLOG("ExprParentCtrl::Layout");
	DLOG(ec->AsString());
	Size sz = GetSize();
	ec->SetRect(sz);
	ec->LayoutDeep();
}

void ExprParentCtrl::Paint(Draw& d) {
	DLOG("ExprParentCtrl::Paint");
	DLOG(ec->AsString());
	
	//if (!ec->IsChangedDeep())
	//	return;
	//Size sz = GetSize();
	//d.DrawRect(sz, White);
	ec->PaintDeep(d, 0, 0);
}


bool ExprParentCtrl::Key(dword key, int count) {
	return ec->KeyDeep(key, count);
}

void ExprParentCtrl::LeftDown(Point p, dword keyflags) {
	SetFocus();
	DLOG("ExprParentCtrl::LeftDown BEFORE");
	DLOG(ec->AsString());
	ec->LeftDownDeep(p, keyflags, 0, 0);
	DLOG("ExprParentCtrl::LeftDown AFTER");
	DLOG(ec->AsString());
	
}

void ExprParentCtrl::RightDown(Point p, dword keyflags) {
	ec->RightDownDeep(p, keyflags, 0, 0);
}

void ExprParentCtrl::GotFocus() {
	ec->GotFocus();
}

void ExprParentCtrl::LostFocus() {
	//ec->LostDeep();
}

void ExprParentCtrl::MouseEnter(Point p, dword keyflags) {
	ec->MouseEnterDeep(p, keyflags);
}

void ExprParentCtrl::MouseLeave() {
	ec->MouseLeaveDeep();
}

void ExprParentCtrl::MouseWheel(Point pt, int zdelta, dword kf) {
	ec->MouseWheelDeep(pt, zdelta, kf, 0, 0);
}

}
