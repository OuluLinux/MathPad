#include "SubMathCtrl.h"

namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;



SubMathCtrl::IteratorDeep::IteratorDeep(SubMathCtrl* cur) {
	this->cur.Add(cur);
	pos.Add(0);
	if (cur->IsRightHandFocus()) operator++(9999);
	begin_ptr = this->cur[this->cur.GetCount()-1];
}

SubMathCtrl::IteratorDeep::IteratorDeep(const SubMathCtrl* cur) {
	this->cur.Add((SubMathCtrl*)cur);
	pos.Add(0);
	if (cur->IsRightHandFocus()) operator++(9999);
	begin_ptr = this->cur[this->cur.GetCount()-1];
}

bool SubMathCtrl::IteratorDeep::operator == (IteratorDeep& it) {
	return GetCurrentAddr() == it.GetCurrentAddr();
}




void SubMathCtrl::IteratorDeep::operator ++(int i) {
	#define LASTPOS pos[pos.GetCount()-1]
	#define LASTCUR cur[cur.GetCount()-1]
	#define SECLASTCUR cur[cur.GetCount()-2]
	#define ADDPOS pos[pos.GetCount()-1]++
	#define ADDCUR LASTCUR = &SECLASTCUR->At(LASTPOS)
	#define REMLASTPOS pos.Remove(pos.GetCount()-1)
	#define REMLASTCUR cur.Remove(cur.GetCount()-1)
	
	if (pos.GetCount() == 1 && pos[0] < 0) {pos[0]++; return;}
	if (pos.GetCount() == 1 && pos[0] == 1) return; // at end
	
	bool rhand = LASTCUR->IsRightHandFocus();
	if (!rhand && LASTCUR->GetCount()) {
		while (LASTCUR->GetCount()) {
			pos.Add(0);
			cur.Add(&LASTCUR->At(0));
			rhand = LASTCUR->IsRightHandFocus();
			if (!rhand) break;
		}
	} 
	else if (pos.GetCount() == 1) {
		if (i != 9999)
			pos[0] = 1; // at end
		else {
			if (!rhand)
				pos[0] = 1; // at end
			else {
				if (LASTCUR->GetCount()) {
					while (rhand && LASTCUR->GetCount()) {
						//pos[0] = 1;
						pos.Add(0);
						cur.Add(&LASTCUR->At(0));
						rhand = LASTCUR->IsRightHandFocus();
					}
				} else {
					pos[0] = 1; // at end
				}
			}
		}
	}
	else {
		
		while (1) {
			if (LASTPOS + 1 < SECLASTCUR->GetCount()) {
				ADDPOS;
				ADDCUR;
				
				bool rhand = LASTCUR->IsRightHandFocus();
				while (rhand && LASTCUR->GetCount()) {
					pos.Add(0);
  					cur.Add(&LASTCUR->At(0));
  					rhand = LASTCUR->IsRightHandFocus();
				} 
				
				break;
			} else {
				REMLASTPOS;
				REMLASTCUR;
				bool rhand = LASTCUR->IsRightHandFocus();
				if (rhand)
					break;
				if (pos.GetCount() <= 1) {
					pos.SetCount(1);
					pos[0] = 1;
					break;
				}
			}
		}
	}
}

void SubMathCtrl::IteratorDeep::operator --(int i) {
	#define LASTPOS pos[pos.GetCount()-1]
	#define LASTCUR cur[cur.GetCount()-1]
	#define SECLASTCUR cur[cur.GetCount()-2]
	#define SUBPOS pos[pos.GetCount()-1]--
	#define SUBCUR LASTCUR = &SECLASTCUR->At(LASTPOS)
	#define REMLASTPOS pos.Remove(pos.GetCount()-1)
	#define REMLASTCUR cur.Remove(cur.GetCount()-1)
	
	// Go in if right hand node, and subnodes exists
	bool rhand = LASTCUR->IsRightHandFocus();
	if (rhand) {
		if (pos.GetCount() == 1 && LASTPOS == 1) {
			pos[0] = 0;
			return;
		}
		else {
			int count = LASTCUR->GetCount();
			while (count) {
				pos.Add(count-1);
				cur.Add(&LASTCUR->At(count-1));
				rhand = LASTCUR->IsRightHandFocus();
				count = LASTCUR->GetCount();
				if (rhand) break;
			}
			return;
		}
	}
	else if (pos.GetCount() == 1 && LASTPOS == 1) {
		pos[0] = 0;
		int count = LASTCUR->GetCount();
		while (count) {
			pos.Add(count-1);
			cur.Add(&LASTCUR->At(count-1));
			count = LASTCUR->GetCount();
			rhand = LASTCUR->IsRightHandFocus();
			if (rhand) break;
		}
		return;
	}
	
	if (pos.GetCount() == 1 && LASTPOS == 0) {
		
	}
	else {
		
		while (1) {
			// Go previous
			if (LASTPOS - 1 >= 0) {
				SUBPOS;
				SUBCUR;
				
				// Stay here if right hand node
				bool rhand = LASTCUR->IsRightHandFocus();
				if (rhand)
					break;
				
				// Else go in
				
				int count = LASTCUR->GetCount();
				while (count) {
					pos.Add(count-1);
					cur.Add(&LASTCUR->At(count-1));
					count = LASTCUR->GetCount();
					rhand = LASTCUR->IsRightHandFocus();
					if (rhand) break;
				} 
				break;
			} 
			// Go out
			else {
				REMLASTPOS;
				REMLASTCUR;
				if (pos.GetCount() <= 1) {
					pos.SetCount(1);
					pos[0] = 0;
					break;
				}
				if (!LASTCUR->IsRightHandFocus())
					break;
			}
		}
	}
}

void SubMathCtrl::IteratorDeep::IncNotDeep() {
	if (LASTCUR->GetCount()) {
		while (1) {
			if (cur.GetCount() >= 2 && LASTPOS + 1 < SECLASTCUR->GetCount()) {
				ADDPOS;
				ADDCUR;
				break;
			} else {
				REMLASTPOS;
				REMLASTCUR;
				if (pos.GetCount() <= 1) {
					pos.SetCount(1);
					pos[0] = 1;
					break;
				}
			}
		}
	} 
	else operator++(1);
}

SubMathCtrl* SubMathCtrl::IteratorDeep::operator->() {
	if (pos.GetCount() && pos[0] == 1) return 0;
	return LASTCUR;
}

SubMathCtrl& SubMathCtrl::IteratorDeep::operator*() {
	if (pos.GetCount() && pos[0] == 1) throw Error("Iterator at end");
	return *LASTCUR;
}

SubMathCtrl* SubMathCtrl::IteratorDeep::Higher() {
	if (cur.GetCount() <= 1) return 0;
	return cur[cur.GetCount()-2];
}


}
