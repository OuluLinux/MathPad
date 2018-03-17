#include "SubMathCtrl.h"



namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;


Navigator::Navigator() {
	
	
	vert.Vert();
	vert.Set(horz, description);
	vert.SetPos(8000);
	
	horz.Horz();
	horz.Set(classes, list);
	horz.SetPos(2000);
	
	classes.NoHeader();
	classes.AddColumn("");
	classes.WhenAction = THISBACK(RefreshData);
	
	list.NoHeader();
	list.AddColumn("");
	list.AddIndex(list_id);
	list.WhenAction = THISBACK(RefreshCursor);
	
	lsearch.SetLabel("Search:");
	cancel.SetLabel("Cancel");
	ok.SetLabel("OK");
	
	ptr = 0;
	wndptr = 0;
	tmp_loses_focus = 0;
	
	
	
	
	RefreshData();
	RefreshCursor();
}

void Navigator::RefreshData() {
	Panic("TODO");
	/*int classes_cursor = classes.GetCursor();
	int class_id = classes_cursor;
	bool classes_has_focus = classes.HasFocus();
	bool list_has_focus = list.HasFocus();
	
	String value = search.GetData();
	String value_incase;
	for(int i = 0; i < value.GetCount(); i++)
		value_incase.Cat(ToLower(value[i]));
	
	ScriptRunner& proc = GetScriptRunner();
	
	classes.Clear();
	classes.Add("All");
	for(int i = 0; i < proc.GetClassCount(); i++) {
		classes.Add(proc.GetClassName(i));
	}
	
	list.Clear();
	
	class_id--;
	if (class_id < 0 || class_id >= proc.GetClassCount()) {
		for(int i = 0; i < proc.GetProblemCount(); i++) {
			ProblemCallback& cb = proc.GetProblem(i);
			CompareAdd(cb.name, value, value_incase, i);
		}
	} else {
		
		int count = proc.GetClassProblemCount(class_id);
		for(int i = 0; i < count; i++) {
			int prob_id = proc.GetClassProblem(class_id, i);
			ProblemCallback& cb = proc.GetProblem(prob_id);
			CompareAdd(cb.name, value, value_incase, prob_id);
		}
	}
	
	list.SetSortColumn(0);
	
	RefreshCursor();
	
	if (list_has_focus && list.GetCount())
		list.SetCursor(0);
	if (classes_has_focus && classes_cursor >= 0 && classes_cursor < classes.GetCount()) {
		classes.WhenAction.Clear(); // TODO: fix this workaround
		classes.SetCursor(classes_cursor);
		classes.WhenAction = THISBACK(RefreshData);
	}
	
	*/
}

void Navigator::CompareAdd(const String& cb, const String& value, const String& value_incase, int id) {
	bool same=true;
	int count = min(cb.GetCount(), value_incase.GetCount());
	for(int i = 0; i < count; i++) {
		int chr = ToLower(cb[i]);
		if (chr != value_incase[i]) {
			same=false;
			break;
		}
	}
	if (same)
		list.Add(cb, id);
}

void Navigator::RefreshCursor() {
	Panic("TODO");
	/*int cursor = list.GetCursor();
	description.Clear();
	if (cursor < 0 || cursor >= list.GetCount()) return;
	
	bool list_has_focus = list.HasFocus();
	int id = list.Get(cursor, 1);
	
	String out;
	
	ScriptRunner& proc = GetScriptRunner();
	
	
	if (id >= 0 && id < proc.GetProblemCount()) {
		ProblemCallback& cb = proc.GetProblem(id);
		out << "Args: " << cb.args;
		//if (cb.optargs.GetCount()) out << " [" << cb.optargs << "]";
		out << "\n" << "Ex: " << cb.examples << "\n" << "Desc: " << cb.desc;
	}
	
	
	description.SetData(out);
	
	if (list_has_focus && cursor >= 0 && cursor < list.GetCount())
		list.SetCursor(cursor);*/
}

void Navigator::SetDefault() {
	Add(vert.VSizePos(30).HSizePos());
	Add(lsearch.LeftPos(5,95).TopPos(0,30));
	Add(search.LeftPos(100,100).TopPos(2,26));
	Add(cancel.RightPos(110,100).TopPos(2,26));
	Add(ok.RightPos(5,100).TopPos(2,26));
	search.SetFocus();
	ok.WhenAction = THISBACK(CloseAndAccept);
	cancel.WhenAction = THISBACK(CloseThis);
	search.WhenEnter = THISBACK(CloseAndAccept);
	search.WhenAction = THISBACK(RefreshData);
	PostCallback(THISBACK(DoFocusEdit));
}

		
void Navigator::Run() {
	TopWindow tw;
	wndptr = &tw;
	tw.Title("Navigator");
	tw.Sizeable();
	tw.SetRect(0,0,640,480);
	tw.Add((*this).VSizePos().HSizePos());
	tw.Run();
}

void Navigator::SetAssist() {
	Add(vert.VSizePos().HSizePos());
	PostCallback(THISBACK(DoSetFocus));
}

void Navigator::SetSubMathCtrl(SubMathCtrl* ptr) {
	this->ptr = ptr;
	Node* node = ptr->GetPtr();
	if (node && node->IsId()) {
		String value = node->GetString();
		search.SetData(value);
		search.Key(K_END, 1);
		RefreshData();
	}
}

void Navigator::LostFocus() {
	if (HasFocusDeep())
		return;
	if (tmp_loses_focus)
		return;
	CloseThis();
}

void Navigator::CloseThis() {
	if (!wndptr) {
		Close();
		Close();
	} else {
		wndptr->Close();
		wndptr->Close();
	}
	PostCallback(callback1(RemoveNavigator, this));
	if (ptr) {
		Node* node = ptr->GetPtr();
		if (node) {
			SubMathCtrl::Cursor cur;
			Node* node0 = node->GetCount() ? &(*node)[0] : node;
			cur.SetPtr(node0);
			cur.SetTextPos(999);
			ptr->WhenNeedsAbsoluteCursor(cur);
		}
	}
}

void Navigator::CloseAndAccept() {
	CloseAndAcceptExample(-1);
}

void Navigator::CloseAndAcceptExample(int ex) {
	Panic("TODO");
	/*int cursor = list.GetCursor();
	description.Clear();
	if (cursor < 0 || cursor >= list.GetCount()) {CloseThis(); return;}
	
	int id = list.Get(cursor, 1);
	
	ScriptRunner& proc = GetScriptRunner();
	
	int arg_count = 0;
	if (id >= 0 && id < proc.GetProblemCount()) {
		ProblemCallback& cb = proc.GetProblem(id);
		arg_count = Split(cb.args, ",").GetCount();
	}
	
	
	if (ptr) {
		Node node = ptr->GetPtr();
		String value;
		int i = list.GetCursor();
		if (i >= 0 && i < list.GetCount())
			value = list.Get(i, 0);
		else
			value = search.GetData();
		
		if (node && value.GetCount()) {
			i = proc.FindProblem(value);
			
			if (i != -1) {
				Vector<String> ex_list = Split(proc.GetProblem(i).examples, ";");
				if (ex >= 0 && ex < ex_list.GetCount()) {
					node = MathCore::ParseExpression(value + "(" + ex_list[ex] + ")");
					ptr->WhenNeedsRedraw();
					CloseThis();
					return;
				}
			} 
			
			node = MathCore::Function(value);
			for(int i = 0; i < arg_count; i++) {
				node->Add(Void());
			}
			
			ptr->WhenNeedsRedraw();
		}
	}
	CloseThis();*/
}

bool Navigator::Key(dword key, int count) {
	
	if (key == K_ESCAPE) {
		CloseThis();
		return true;
	}
	else if (key == K_RETURN) {
		CloseAndAccept();
		return true;
	}
	else if (key == (K_CTRL|K_RETURN)) {
		CloseAndAcceptExample(0);
		return true;
	}
	else if (key >= (K_CTRL|K_0) && key <= (K_CTRL|K_9)) {
		CloseAndAcceptExample(key - (K_CTRL|K_0));
		return true;
	}
	else if (key == K_DOWN || key == K_UP || key == K_PAGEUP || key == K_PAGEDOWN) {
		return list.Key(key,  count);
	}
	
	bool ret = search.Key(key, count);
	
	if (ret && ptr) {
		Node* node = ptr->GetPtr();
		String value = search.GetData();
		if (node) {
			if (value.GetCount()) {
				if (node->IsId()) {
					tmp_loses_focus = true;
					node->SetDataString(value);
					ptr->WhenNeedsRedraw();
					PostCallback(THISBACK(RecoverFocus));
				}
				else if (node->IsNull()) {
					tmp_loses_focus = true;
					*node = MathCore::id(value);
					ptr->WhenNeedsRedraw();
					PostCallback(THISBACK(RecoverFocus));
				}
			} else {
				tmp_loses_focus = true;
				*node = MathCore::Void();
				ptr->WhenNeedsRedraw();
				PostCallback(THISBACK(RecoverFocus));
			}
		}
		RefreshData();
	}
	
	return ret;
}

void Navigator::DoFocusEdit() {
	search.SetFocus();
}

void Navigator::DoSetFocus() {
	SetFocus();
}

void Navigator::RecoverFocus() {
	tmp_loses_focus = false;
	list.SetFocus();
}








Upp::Array<Navigator> navs;

Navigator& NewNavigator() {
	return navs.Add();
}

void RemoveNavigator(Navigator* nav) {
	for(int i = 0; i < navs.GetCount(); i++) {
		if (nav == &navs[i]) {
			navs.Remove(i);
			return;
		}
	}
}

int GetNavigatorCount() {
	return navs.GetCount();
}

}
