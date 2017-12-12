#include "Xcas.h"

namespace Xcas {

CasEdit::CasEdit() {
	ctx = new giac::context;
	init_context(ctx);
	
	doc.edit = this;
	
	Add(doc);
	AddFrame(sb);

	sb.WhenScroll = [=] { Refresh(); };
	sb.SetLine(50);
	
}

CasEdit::~CasEdit() {
	cleanup_context(ctx);
	delete ctx;
}

void CasEdit::Layout() {
	doc.RefreshLabel();
	
	int page_height = doc.GetContentHeight();
	Size sz(GetSize());
	
	sb.SetPage(sz.cy);
	sb.SetTotal(page_height);
	
	doc.SetRect(0, -sb, sz.cx, page_height);
}

void CasEdit::MouseWheel(Point, int zdelta, dword) {
	sb.Wheel(zdelta);
}

bool CasEdit::Key(dword key, int) {
	return sb.VertKey(key);
}

void CasEdit::Print() {
	
}

Editor& CasEdit::NewExpression() {
	Editor& e = GetSelectedGroup().Add<Editor>();
	Layout();
	return e;
}

CasItem* CasEdit::GetSelectedItem() {
	return GetSelectedGroup().GetSelectedItem();
}

RichText CasEdit::Get() {
	
	return RichText();
}

String CasEdit::GetXml() {
	
	return "";
}

CasGroup& CasEdit::GetSelectedGroup() {
	return doc.GetSelectedGroup();
}

void CasEdit::SetXml(const String& xml) {
	
}

}
