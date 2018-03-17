#include "MathCtrl.h"


namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;
	
MathCtrl::MathCtrl() {
	Add(docwrap.SizePos());
	docwrap.Set(doc);
	doc.SetFocus();
	output_depth = 0;
}

void MathCtrl::Print(String title, Node n) {
	Node math;
	math.SetOp(OP_LABEL);
	math.Add(id(title));
	math.Add(n);
	MathListItem& item = doc.list.Add();
	item.Set(output_depth, math);
}

void MathCtrl::Exercise(String title) {
	output_depth = 1;
	Node math;
	math.SetType(TYPE_EXERCISE);
	math.SetDataString(title);
	MathListItem& item = doc.list.Add();
	item.Set(0, math);
}

void MathCtrl::Inner(String title) {
	output_depth++;
	Node math;
	math.SetType(TYPE_EXERCISE);
	math.SetDataString(title);
	MathListItem& item = doc.list.Add();
	item.Set(0, math);
}

void MathCtrl::Outer() {
	output_depth--;
	
}


void MathCtrl::FocusDocument() {
	doc.SetFocus();
	doc.SetCursor(0);
}

void MathCtrl::Refresh() {
	doc.End();
}

bool MathCtrl::Key(dword key, int count) {
	if (key == K_RIGHT) {
		doc.Expand();
		return true;
	}
	else if (key == K_LEFT) {
		doc.Collapse();
		return true;
	}
	return ParentCtrl::Key(key, count);
}

}
