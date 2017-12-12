#include "Xcas.h"

namespace Xcas {

void ItemLabel::Paint(Draw& w) {
	Size sz = GetSize();
	w.DrawRect(sz, is_selected ? SColorHighlight : White);
	w.DrawLine(0, sz.cy-1, sz.cx-1, sz.cy-1, 1, SColorShadow);
	w.DrawText(5, 0, label, Arial(sz.cy-4), is_selected ? SColorHighlightText : SColorText);
}








CasItem::CasItem() {
	Add(label);
	
	SetFrame(NullFrame());
	NoWantFocus();
}

CasItem::~CasItem() {
	
}

void CasItem::Layout() {
	
	label.SetRect(0, 0, label_width, 15);
	
}

void CasItem::MouseMove(Point p, dword) {
	if(HasCapture()) {
		if (p.y >= min_height) {
			height = p.y;
			edit->Layout();
		}
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
	else
		w.DrawLine(0, sz.cy-1, sz.cx-1, sz.cy-1, 1, SColorShadow);
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

int CasGroup::GetContentHeight() const {
	int h = 0;
	for (const auto& i : items) h += i.GetContentHeight();
	return h;
}

void CasGroup::Layout() {
	Size sz = GetSize();
	int y = -sb;
	
	int left_pad = group ? label_width : 0;
	if (group) label.SetRect(0, 0, label_width, 15);
	
	for(int i = 0; i < items.GetCount(); i++) {
		CasItem& ci = items[i];
		int h = ci.height;
		ci.SetRect(left_pad, y, sz.cx - left_pad, h);
		y += h;
	}
	
}

void CasGroup::RefreshLabel() {
	for(int i = 0; i < items.GetCount(); i++) {
		CasItem& ci = items[i];
		ci.label.label = IntStr(i+1);
		ci.RefreshLabel();
	}
}






CommentOutput::CommentOutput() {
	ParentCtrl::Add(lbl.SizePos());
	lbl.Label::SetInk(Green);
}

void CommentOutput::Add(String comment) {
	String s = lbl.GetData();
	if (s != "") s += "\n";
	s += comment;
	lbl.SetLabel(s);
}

void CommentOutput::Clear() {
	lbl.SetLabel("");
}

}
