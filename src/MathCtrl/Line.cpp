#include "MathCtrl.h"
#include <MathCore/MathCore.h>

namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;
	
bool __calc_line_refresh;


bool CalcLine::edit_enabled = false;

CalcLine::CalcLine() {
	depth = 0;
	x_offset = 0;
	y_offset = 0;
	
	Add(draw);
}

CalcLine::~CalcLine() {
	
}

Size CalcLine::GetPrefferedSize() {
	Size sz = draw.GetRenderSize();
	return sz;
}

void CalcLine::SetExpression(MathCore::Node n) {
	node = n;
	draw.SetNode(node);
}

void CalcLine::SetText(const String& str) {
	node = MathCore::Text(str);
	draw.SetNode(node);
}

void CalcLine::SetDebug(const String& str) {
	node = MathCore::Text("Debug: " + str);
	draw.SetNode(node);
}

void CalcLine::SetError(const String& str) {
	node = MathCore::Text("Error: " + str);
	draw.SetNode(node);
}

void CalcLine::Redraw() {
	SubMathCtrl::Cursor cursor = draw.GetCursor();
	draw.SetNode(node);
	draw.SetCursor(cursor);
}

bool CalcLine::Key(dword key, int count) {
	if (key == K_ENTER) {
		edit_enabled = true;
		if (!draw.HasFocusDeep()) {
			EditMode(true);
			SubMathCtrl::Cursor cur;
			draw.SetCursor(cur);
			Refresh();
			return true;
		}
	}
	else if (key == K_ESCAPE) {
		edit_enabled = false;
		if (draw.HasFocusDeep()) {
			EditMode(false);
			SetFocus();
			GetParent()->Refresh();
			return true;
		}
	}
	return false;
}

void CalcLine::Layout() {
	Size sz = GetSize();
	draw.SetRect(sz);
}

void CalcLine::Paint(Draw& d) {
	//LOG("CalcLine::Paint this=" << Format("%X", (int64)this));
	
	__calc_line_refresh = true;
	Size sz = GetRect().GetSize();
	d.DrawRect(sz, White);
	
#ifndef MINIMIZE
	{
		int x = GetRect().left;
		int y = GetRect().top;
		int box = 20;
		int sc = (box-y) % box;
		int xx = (box-x) % box; // x+box
		Rect r(GetSize());
		
		int w = r.GetWidth();
		int h = r.GetHeight();
		
		Color c = Color(235, 253, 253);
		
		for(int i = xx; i < w; i += box) {
			d.DrawLine(i, 0, i, h, 1, c);
		}
		
		for(int i = sc; i < h; i += box) {
			d.DrawLine(0, i, w, i, 1, c);
		}
	}
#endif
	
	if (HasFocus()) {
		ImageBuffer ib(sz.cx, sz.cy);
		for(RGBA* ptr = ib.Begin(); ptr != ib.End(); ptr++) {
			ptr->r = 0;
			ptr->g = 0;
			ptr->b = 64;
			ptr->a = 64;
		}
		d.DrawImage(0, 0, ib);
	}
	
}

void CalcLine::LeftDown(Point p, dword keyflags) {
	SetFocus();
	GetParent()->Refresh();
}

void CalcLine::GotFocus() {
	GetParent()->ChildGotFocus();
	if (edit_enabled)
		EditMode(true);
	else
		EditMode(false);
	Refresh();
}

void CalcLine::LostFocus() {
	Refresh();
}


void CalcLine::ChildLostFocus() {
	GetParent()->Refresh();
}

void CalcLine::EditMode(bool b) {
	//LOG("CalcLine::EditMode " << (int)b);
	if (!b) {
		draw.EnableSubFocus(false);
	} else {
		draw.EnableSubFocus(true);
	}
	GetParent()->Refresh();
}

}
