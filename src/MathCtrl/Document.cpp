#include "MathCtrl.h"


namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;
	

CalcBackground::CalcBackground() {
	scroll = 0;
}


void CalcBackground::Paint(Draw& d) {
	LOG("CalcBackground::Paint this=" << Format("%X", (int64)this));
	
	int box = 20;
	int sc = 0;
	Rect r(GetSize());
	
	d.DrawRect(r, White);
	
#ifndef MINIMIZE
	int w = r.GetWidth();
	int h = r.GetHeight();
	
	Color c = Color(235, 253, 253);
	
	for(int i = box; i < w; i += box) {
		d.DrawLine(i, 0, i, h, 1, c);
	}
	
	for(int i = sc; i < h; i += box) {
		d.DrawLine(0, i, w, i, 1, c);
	}
#endif
}




CalcDocument::CalcDocument() {
	last_line = -1;
	//calc = 0;
	
	sb.WhenScroll = THISBACK(Scroll);
	
	Add(bg);
	
	bg.WhenMouseWheel = THISBACK(MouseWheel);
	
	total_zoom_factor = 0.2;
	
	
	WantFocus();
	
	SubMathCtrl::WhenNeedsRedraw = THISBACK(RedrawSelected);
	SubMathCtrl::WhenNeedsFullRefresh = THISBACK(DoFullRefresh);
	SubMathCtrl::WhenNeedsLogicalCursor = THISBACK(LogicalCursor);
	SubMathCtrl::WhenNeedsAbsoluteCursor = THISBACK(AbsoluteCursor);
	SubMathCtrl::WhenCursorLeft = THISBACK(CursorLeft);
	SubMathCtrl::WhenCursorRight = THISBACK(CursorRight);
	SubMathCtrl::WhenAnswer = THISBACK(GotAnswer);
	
	PostCallback(THISBACK(AddScrollBar));
}

void CalcDocument::AddScrollBar() {
	GetParentCtrl()->AddFrame(sb);
}

void CalcDocument::GotFocus() {
	//LOG("CalcDocument::GotFocus");
}

void CalcDocument::LostFocus() {
	//LOG("CalcDocument::LostFocus");
}

	
int CalcDocument::GetCursor() {
	for(int i = 0; i < lines.GetCount(); i++) {
		if (lines[i].HasFocusDeep()) return i;
	}
	return -1;
}

void CalcDocument::SetCursor(int i) {
	if (i >= 0 && i < lines.GetCount())
		lines[i].SetFocus();
	else 
		SetFocus();
}

void CalcDocument::GoLeft() {
	int cursor = GetCursor();
	if (cursor < 0) return;
	//Upp::Array<MathListItem>& list = GetMainMathListItems();
	int item_id = lines[cursor].GetItemId();
	int depth = list[item_id].GetDepth() - 1;
	int higher_id = -1;
	for(int i = item_id - 1; i >= 0; i--) {
		if (list[i].GetDepth() == depth) {
			higher_id = i;
			break;
		}
	}
	for(int i = 0; i < lines.GetCount(); i++) {
		if (lines[i].GetItemId() == higher_id) {
			lines[i].SetFocus();
			break;
		}
	}
}


	
bool CalcDocument::Key(dword key, int count) {
	if (key == K_DOWN) {
		last_line++;
		if (last_line >= lines.GetCount()) last_line = lines.GetCount() -1;
		if (last_line >= 0) lines[last_line].SetFocus();
		Refresh();
		return true;
	}
	else if (key == K_UP) {
		last_line--;
		if (last_line < 0) last_line = 0;
		if (last_line < lines.GetCount()) lines[last_line].SetFocus();
		Refresh();
		return true;
	}
	else if (key == (K_DELTA|K_LEFT|K_CTRL)) {
		if (last_line >= 0 && last_line < lines.GetCount()) {
			CalcLine& l = lines[last_line];
			int xoff = l.GetXOffset() - 20;
			l.SetXOffset(xoff);
			Layout();
			Refresh();
		}
	}
	else if (key == (K_DELTA|K_RIGHT|K_CTRL)) {
		if (last_line >= 0 && last_line < lines.GetCount()) {
			CalcLine& l = lines[last_line];
			int xoff = l.GetXOffset() + 20;
			l.SetXOffset(xoff);
			Layout();
			Refresh();
		}
	}
	else if (key == (K_DELTA|K_CTRL|K_F) || key == (K_ALT|K_ENTER)) {
		AddNode(MathCore::Void());
		return true;
	}
	/*else if (key == K_ENTER) {
		if (last_line >= lines.GetCount()) last_line = lines.GetCount() -1;
		if (last_line >= 0) {
			CalcLine& l = lines[last_line];
			Node n = l.node;
			if (n && calc) {
				LOG(n.AsInlineString());
				Node ans = *n;
				calc->Execute(ans);
				AddNode(ans);
				return true;
			}
		}
	}
	else if (key == (K_CTRL|K_D)) {
		if (last_line >= lines.GetCount()) last_line = lines.GetCount() -1;
		if (last_line >= 0) {
			CalcLine& l = lines[last_line];
			Node n = l.node;
			if (n && calc) {
				LOG(n.AsInlineString());
				Node ans = *n;
				AddNode(ans.Floating());
				return true;
			}
		}
	}*/
	
	return false;
}

void CalcDocument::GotAnswer(Node n) {
	AddNode(n);
};

void CalcDocument::AddNode(const MathCore::Node n) {
	SetFocus();
	//Upp::Array<MathListItem>& list = GetMainMathListItems();
	MathListItem& item = list.Add();
	item.Set(0, n);
	RefreshLines();
	RefreshScrollBar();
	RefreshLayout();
	CalcLine& l = lines[lines.GetCount()-1];
	l.EditMode(true);
	SubMathCtrl::Cursor cur;
	cur.SetPtr(&item.GetNode());
	l.draw.SetFocus();
	l.draw.SetCursor(cur);
	l.Refresh();
	ScrollToEnd();
	Layout();
}

void CalcDocument::MouseWheel(Point, int zdelta, dword) {
	sb.Wheel(zdelta);
}

bool calc_doc_layout;

void CalcDocument::Layout() {
	int pos = sb;
	pos -= pos % 20; // Snap to grid. TODO: get custom gridsize
	bg.SetRect(GetSize());
	bg.SetScroll(-pos);
	
	calc_doc_layout = true;
	DoCheckCursorVisibility();
	calc_doc_layout = false;
	
	Size sz = GetSize();
	
	int y = -pos;
	
	for(int i = 0; i < lines.GetCount(); i++) {
		CalcLine& l = lines[i];
		Size dsz = l.GetPrefferedSize();
		int x = 20 * l.depth;
		int x_off = l.GetXOffset();
		x -= x_off;
		Rect r_old = l.GetRect();
		Rect r_new = RectC(x, y, dsz.cx, dsz.cy);
		if (r_old != r_new) {
			l.SetRect(r_new);
		}
		
		bool visible = (y + dsz.cy) > 0 && y < (sz.cy);
		l.Show(visible);
		
		y += dsz.cy;
	}
	
	Refresh();
}

void CalcDocument::ChildGotFocus() {
	RefreshLastLineFocus();
}

void CalcDocument::DoCheckCursorVisibility() {
	//LOG("CalcDocument::DoCheckCursorVisibility");
	if (last_line < 0 || last_line >= lines.GetCount())
		return;
	CalcLine& l = lines[last_line];
	SubMathCtrl::Cursor cur = l.draw.GetCursor();
	SubMathCtrl* mc = cur.GetSubMathCtrl();
	Rect rect = GetScreenRect();
	Size sz = GetSize();
	int grid = 20;
	if (mc) {
		Rect cur_rect = mc->GetScreenRect();
		int x = 10 * l.depth;
		int w = sz.cx - x;
		int prev_x_off = l.GetXOffset();
		int prev_y_off = l.GetYOffset();
		int cur_x = cur_rect.left - rect.left + mc->GetCursorX();
		int x_max = sz.cx - 4*grid;
		int x_min = 4* grid;
		
		int x_off_left  = prev_x_off - (x_min - cur_x);
		int x_off_right = prev_x_off + (cur_x - x_max);
		
		x_off_left  -= x_off_left % grid;
		x_off_right -= x_off_right % grid;
		
		if (cur_x >= x_max) {
			l.SetXOffset(x_off_right);
			if (!calc_doc_layout) {
				PostCallback(THISBACK(DoRefreshLayout));
			}
		}
		else if (cur_x < x_min) {
			if (x_off_left <= 4*grid)
				x_off_left = 0;
			l.SetXOffset(x_off_left);
			if (!calc_doc_layout) {
				PostCallback(THISBACK(DoRefreshLayout));
			}
		}
	}
}

extern bool __calc_line_refresh;


void CalcDocument::Scroll() {
	Layout();
	
}

void CalcDocument::ScrollToEnd() {
	sb.ScrollInto(sb.GetTotal());
}

void CalcDocument::RefreshScrollBar() {
	Size sz = GetSize();
	int y = 0;
	for(int i = 0; i < lines.GetCount(); i++) {
		CalcLine& l = lines[i];
		Size sz = l.GetPrefferedSize();
		y += sz.cy;
	}
	y += 20 - (y % 20); 
	int line = lines.GetCount() ? y / lines.GetCount() : 0;
	sb.SetLine(line);
	sb.SetPage(sz.cy);
	sb.SetTotal(y);
}


void CalcDocument::RedrawSelected() {
	PostCallback(THISBACK(DoRedrawSelected));
}

void CalcDocument::DoFullRefresh() {
	PostCallback(THISBACK(FullRefresh));
}

void CalcDocument::LogicalCursor(SubMathCtrl::Cursor cur) {
	PostCallback(THISBACK1(DoLogicalCursor, cur));
}

void CalcDocument::AbsoluteCursor(SubMathCtrl::Cursor cur) {
	PostCallback(THISBACK1(DoAbsoluteCursor, cur));
}

void CalcDocument::CheckCursorVisibility() {
	PostCallback(THISBACK(DoCheckCursorVisibility));
}

/*void CalcDocument::CheckDraw() {
	PostCallback(THISBACK(DoCheckDraw));
}*/

void CalcDocument::FullRefresh() {
	//LOG("CalcDocument::FullRefresh");
	CheckCursorVisibility();
	Refresh();
}

void CalcDocument::DoRedrawSelected() {
	LOG("CalcDocument::DoRedrawSelected");
	int cursor = last_line;
	if (cursor < 0 || cursor >= lines.GetCount()) 
		return;
	
	
	
	int pos = sb.Get();
	pos -= pos % 20; // Snap to grid. TODO: get custom gridsize
	int total = sb.GetTotal();
	int page = sb.GetPage();
	bool scroll_is_at_bottom = (pos+page) == total;
	
	CalcLine& cl = lines[cursor];
	Size prev_size = cl.GetPrefferedSize();
	cl.Redraw();
	Size next_size = cl.GetPrefferedSize();
	
	if (prev_size == next_size) {
		CheckCursorVisibility();
		Refresh();
	} else {
		RefreshScrollBar();
		Layout();
		if (scroll_is_at_bottom) 
			sb.Set(sb.GetTotal());
		CheckCursorVisibility();
		Refresh();
	}
}

void CalcDocument::DoLogicalCursor(SubMathCtrl::Cursor cur) {
	//PRINT("CalcDocument::LogicalCursor " << cur.AsString());
	int cursor = GetCursor();
	if (cursor == -1) 
		return;
	CalcLine& cl = lines[cursor];
	SubMathCtrl& draw = cl.draw;
	SubMathCtrl::Cursor current = draw.GetCursor();
	//PRINT("Before: " << current.AsString());
	while (cur.GetCount() && cur[0] == -1) {
		if (current.GetCount())
			current.Remove(current.GetCount()-1);
		cur.Remove(0);
	}
	if (!cur.GetPtr())
		current.Append(cur);
	else
		current.SetPtr(cur.GetPtr());
	//PRINT("After:  " << current.AsString());
	//PRINT("Dest: \n" << draw.AsString());
	draw.SetCursor(current);
	
	/*if (1) {
		current = draw.GetCursor();
		PRINT("Final cursor: " << current.AsString());
	}*/
	
	CheckCursorVisibility();
}

void CalcDocument::DoAbsoluteCursor(SubMathCtrl::Cursor cur) {
	//PRINT("CalcDocument::AbsoluteCursor " << cur.AsString());
	int cursor = last_line;
	if (cursor < 0 || cursor >= lines.GetCount()) 
		return;
	//RefreshLines();
	CalcLine& cl = lines[cursor];
	SubMathCtrl& draw = cl.draw;
	//PRINT("Cursor:  " << cur.AsString());
	//PRINT("Dest: \n" << draw.AsString());
	draw.SetCursor(cur);
	CheckCursorVisibility();
}

void CalcDocument::Begin() {
	list.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		RemoveChild(&lines[i]);
	}
	lines.Clear();
}

void CalcDocument::End() {
	RefreshLines();
	RefreshScrollBar();
	RefreshLayout();
	if (lines.GetCount() == 0)
		SetFocus();
}

int GetDepth(const String& path) {
	return Split(path, DIR_SEPS).GetCount();
}

void CalcDocument::RefreshLines() {
	
	for(int i = 0; i < lines.GetCount(); i++)
		RemoveChild(&lines[i]);
	lines.Clear();
	
	//Upp::Array<MathListItem>& list = GetMainMathListItems();
	int skip_deeper_depth = -1;
	for(int i = 0; i < list.GetCount(); i++) {
		MathListItem& item = list[i];
		if (skip_deeper_depth != -1) {
			if (item.GetDepth() > skip_deeper_depth)
				continue;
			skip_deeper_depth = -1;
		}
		if (item.IsSkipDeeper())
			skip_deeper_depth = item.GetDepth();
		CalcLine& cl = lines.Add();
		Add(cl);
		cl.SetExpression(item.GetNode());
		cl.SetItemId(i);
		cl.SetDepth(item.GetDepth());
	}
}


void CalcDocument::CollapseAll() {
	//Upp::Array<MathListItem>& list = GetMainMathListItems();
	for(int i = 0; i < list.GetCount(); i++) {
		MathListItem& item = list[i];
		if (item.GetDepth() == 0)
			item.SkipDeeper(true);
		else
			item.SkipDeeper(false);
	}
	RefreshLines();
}

void CalcDocument::Expand() {
	//Upp::Array<MathListItem>& list = GetMainMathListItems();
	int cursor = GetCursor();
	for(int i = 0; i < lines.GetCount(); i++) {
		if (lines[i].HasFocus()) {
			int id = lines[i].GetItemId();
			MathListItem& item = list[id];
			item.SkipDeeper(false);
			RefreshLines();
			RefreshLayout();
			break;
		}
	}
	SetCursor(cursor);
	RefreshScrollBar();
}

void CalcDocument::Collapse() {
	//Upp::Array<MathListItem>& list = GetMainMathListItems();
	int cursor = GetCursor();
	for(int i = 0; i < lines.GetCount(); i++) {
		if (lines[i].HasFocus()) {
			int id = lines[i].GetItemId();
			MathListItem& item = list[id];
			bool no_deeper = id < list.GetCount() -2 && list[id+1].GetDepth() <= item.GetDepth();
			if (no_deeper || item.IsSkipDeeper()) {
				GoLeft();
				return;
			} else {
				item.SkipDeeper();
				//f.Hidden(false);
				RefreshLines();
				RefreshLayout();
				break;
			}
		}
	}
	SetCursor(cursor);
	RefreshScrollBar();
}


/*void CalcDocument::InsertLines(int pos, Node n) {
	Vector<MathListItem>& list = GetMainMathListItems();
	for(int i = 0; i < list.GetCount(); i++) {
		MathListItem& item = list[i];
		CalcLine& cl = lines.Insert(pos + i);
		Add(cl);
		cl.SetExpression(item.GetNode());
		cl.SetItemId(i);
		cl.SetDepth(item.GetDepth());
	}
	RefreshLayout();
}*/

void CalcDocument::RemoveLines(int pos, int count) {
	for(int i = 0; i < count; i++) {
		RemoveChild(&lines[pos]);
		lines.Remove(pos);
	}
	RefreshLayout();
}

void CalcDocument::RefreshLastLineFocus() {
	for(int i = 0; i < lines.GetCount(); i++) {
		if (lines[i].HasFocusDeep()) {
			last_line = i;
			return;
		}
	}
	last_line = -1;
}

void CalcDocument::CursorLeft() {
	if (last_line == -1)
		return;
	//PRINT("CalcDocument::CursorLeft");
	CalcLine& cl = lines[last_line];
	SubMathCtrl::Cursor cur = cl.draw.GetCursor();
	SubMathCtrl::IteratorDeep it = cl.draw.BeginDeep();
	Node* orig_ptr = cur.GetPtr();
	SubMathCtrl* begin = &cl.draw;
	SubMathCtrl* last = cur.GetSubMathCtrl();
	
	//PRINT("Before: " << cur.AsString());
	
	if (!orig_ptr)
		return;
	if (orig_ptr->IsEqual(*begin->GetPtr()) && !begin->IsRightHandFocus())
		return;
	
	//PRINT(begin.AsString());
	
	for (;!it.IsEnd();it++) {
		Node* ptr = it->GetPtr();
		SubMathCtrl* cur = &*it;
		//if (ptr == orig_ptr && it->IsDefaultWantFocus()) {
		if (cur == last && cur->IsDefaultWantFocus()) {
			SubMathCtrl* ptr_cur = &*it;
			//PRINT(Format("Found %X at %X", (int64)orig_ptr, (int64)ptr_cur));
			if (!it.IsBegin()) for (;;) {
				it--;
				SubMathCtrl* cur = &*it;
				if (cur != last) {
					ptr = cur->GetPtr();
					if (cur->IsDefaultWantFocus() && cur->GetPtr()) {
						cur->SetFocus();
						break;
					} else {
						if (cur == begin)
							break;
						
					}
				}
				if (it.IsBegin())
					break;
			}
			break;
		}
	}
	
	/*if (1) {
		SubMathCtrl::Cursor current = cl.draw.GetCursor();
		PRINT("Final cursor: " << current.AsString());
	}*/
	
}

void CalcDocument::CursorRight() {
	if (last_line == -1)
		return;
	//PRINT("CalcDocument::CursorRight");
	
	CalcLine& cl = lines[last_line];
	
	{
		SubMathCtrl::IteratorDeep it = cl.draw.BeginDeep();
		int i = 0;
		for (;!it.IsEnd();it++) {
			SubMathCtrl* ptr_cur = &*it;
			//PRINT(i << Format(" At %X ", (int64)ptr_cur) << (it->GetPtr()?it->GetPtr()->AsDataInlineString():""));
			i++;
		}
		for (;;) {
			i--;
			it--;
			SubMathCtrl* ptr_cur = &*it;
			//PRINT(i << Format(" At %X ", (int64)ptr_cur) << (it->GetPtr()?it->GetPtr()->AsDataInlineString():""));
			if (it.IsBegin()) 
				break;
			
		}
		//PRINT(cl.draw.AsString());
	}
	
	SubMathCtrl::Cursor cur = cl.draw.GetCursor();
	SubMathCtrl::IteratorDeep it = cl.draw.BeginDeep();
	Node* orig_ptr = cur.GetPtr();
	SubMathCtrl* begin = &cl.draw;
	SubMathCtrl* last = cur.GetSubMathCtrl();
	
	//PRINT("Before: " << cur.AsString());
	
	if (!orig_ptr || (orig_ptr->IsEqual(*begin->GetPtr()) && begin->IsRightHandFocus()))
		return;
	
	//LOG("CalcDocument::CursorRight current " << orig_ptr->AsDataInlineString());
	//PRINT(begin.AsString());
	
	bool found = false;
	for (;!it.IsEnd();it++) {
		Node* ptr = it->GetPtr();
		if (ptr->IsEqual(*orig_ptr) && it->IsDefaultWantFocus()) {
			found = true;
			SubMathCtrl* ptr_cur = &*it;
			//PRINT(Format("Found %X at %X", (int64)orig_ptr, (int64)ptr_cur));
			for (;;) {
				it++;
				if (it.IsEnd())
					break;
				SubMathCtrl* cur = &*it;
				if (cur == last)
					continue;
				ptr = cur->GetPtr();
				if (cur->IsDefaultWantFocus() && cur->GetPtr()) {
					//LOG("CalcDocument::CursorRight current " << cur->AsString());
					cur->SetFocus();
					break;
				}
			}
			break;
		}
	}
	
	//if (!found) {PRINT(Format("Not found %X", (int64)orig_ptr));}
	
	/*if (1) {
		SubMathCtrl::Cursor current = cl.draw.GetCursor();
		PRINT("Final cursor: " << current.AsString());
	}*/
}

}
