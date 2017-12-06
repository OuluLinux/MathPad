#include "Xcas.h"

namespace Xcas {

void ItemLabel::Paint(Draw& w) {
	Size sz = GetSize();
	w.DrawRect(sz, is_selected ? SColorHighlight : SColorFace);
	w.DrawText(0, 0, label, Arial(sz.cy-4), is_selected ? SColorHighlightText : SColorText);
}








CasItem::CasItem() {
	SetFrame(NullFrame());
	NoWantFocus();
}

CasItem::~CasItem() {
	
}

void CasItem::Layout() {
	
}

void CasItem::MouseMove(Point p, dword) {
	if(HasCapture()) {
		/*SetPos(ClientToPos(p), mouseindex);
		Refresh();
		WhenAction();*/
	}
}

void CasItem::LeftDown(Point p, dword) {
	SetCapture();
	Refresh();
}

void CasItem::LeftUp(Point p, dword keyflags) {
	if(HasCapture())
		WhenSplitFinish();
	ReleaseCapture();
	Refresh();
}

Image CasItem::CursorImage(Point p, dword) {
	return p.y < GetHandleY() ? Image::Arrow() : Image::SizeVert();
}

void CasItem::Paint(Draw& w) {
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(sz, SColorFace);
	if (HasCapture())
		w.DrawRect(sz, SColorHighlight);
}















CasGroup::CasGroup() {
	AddFrame(sb);
	sb.WhenScroll = [=] { Layout(); };
	sb.SetLine(30);
}

CasItem* CasGroup::GetSelectedItem() {
	for(int i = 0; i < items.GetCount(); i++)
		if (items[i].IsSelected())
			return &items[i];
	return NULL;
}

CasGroup& CasGroup::GetSelectedGroup() {
	if (IsSelected())
		return *this;
	for(int i = 0; i < items.GetCount(); i++) {
		if (items[i].IsSelected()) {
			CasGroup* g = dynamic_cast<CasGroup*>(&items[i]);
			if (g) return *g;
		}
	}
	return *this;
}

void CasGroup::Layout() {
	Size sz = GetSize();
	int y = -sb;
	
	for(int i = 0; i < items.GetCount(); i++) {
		CasItem& ci = items[i];
		int h = ci.height;
		ci.SetRect(0, y, sz.cx, h);
		y += h;
	}
	
	//Refresh();
}

}
