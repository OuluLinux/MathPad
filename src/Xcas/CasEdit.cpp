#include "Xcas.h"

namespace Xcas {

CasEdit::CasEdit() {
	Add(doc.SizePos());
	
	
}

void CasEdit::Print() {
	
}

Editor& CasEdit::NewExpression() {
	return GetSelectedGroup().Add<Editor>();
}

CasItem* CasEdit::GetSelectedItem() {
	return GetSelectedGroup().GetSelectedItem();
}

RichText CasEdit::Get() {
	
}

String CasEdit::GetXml() {
	
}

CasGroup& CasEdit::GetSelectedGroup() {
	return doc.GetSelectedGroup();
}

void CasEdit::SetXml(const String& xml) {
	
}

}
