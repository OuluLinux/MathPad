#include "Xcas.h"

namespace Xcas {

Editor::Editor() {
	Add(vsplit.HSizePos(label_width).VSizePos(0, handleh));
	
	vsplit.Vert();
	vsplit << input << comment_out;
	vsplit.SetPos(95000);
	
	input.SetData("simplify(2x+5x)");
	input.SetData("simplify(2x+5x/sqrt(2));");
	
	input.WhenExecute << THISBACK(Execute);
	input.WhenNewline << THISBACK(Newline);
	
	height = min_height + handleh;
}

void Editor::Execute() {
	giac::context0 = edit->ctx;
	
	String s = input.GetData();
	if (s == "") return;
	
	giac::gen g(s, edit->ctx);
	
	comment_out.Clear();
	if (is_zero(g)) {
		comment_out.Add("ERROR");
		return;
	}
	
	g = giac::protecteval(g, 1, edit->ctx);
	
	int content_height = 0;
	int pos0_prev = vsplit.GetPos();
	int pos1_prev = vsplit.GetPos();
	if (!output.IsEmpty()) {
		content_height = output->GetContentHeight();
	} else {
		output = new Equation();
		output->ctx = edit->ctx;
		vsplit << * output;
		pos1_prev = 10000;
	}
	
	output->Set(g);
	
	int content_height_diff = output->GetContentHeight() - content_height;
	int new_height = height + content_height_diff;
	int pos0 = (height * pos0_prev) / new_height;
	int pos1 = (height * pos1_prev) / new_height;
	height += content_height_diff;
	edit->Layout();
	
	vsplit.SetPos(pos0, 0);
	vsplit.SetPos(pos1, 1);
	
	output->Refresh();
}

void Editor::Newline() {
	height += input.GetLineScreenRect(0).GetHeight(); edit->Layout();
}

}

