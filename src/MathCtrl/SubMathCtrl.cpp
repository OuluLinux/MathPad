#include "SubMathCtrl.h"


namespace MathCtrl { using namespace MathCtrl; using namespace MathCore;


int SubMathCtrl::previous_key;

Callback SubMathCtrl::WhenNeedsRedraw;
Callback SubMathCtrl::WhenNeedsFullRefresh;
Callback SubMathCtrl::WhenCursorLeft;
Callback SubMathCtrl::WhenCursorRight;
Callback1<SubMathCtrl::Cursor> SubMathCtrl::WhenNeedsLogicalCursor, SubMathCtrl::WhenNeedsAbsoluteCursor, WhenCursorLeft, WhenCursorRight;
Callback1<Node> SubMathCtrl::WhenAnswer;

Image SubMathCtrl::cursor_before_override;
bool SubMathCtrl::cursor_is_overridden;



#define GRID_HEIGHT 20
#define FONT_MUL 19/20

#ifdef flagUSEGENTIUMFONT
	#define FONT_NAME "Gentium Basic"
	#define SET_FONT fnt.FaceName(FONT_NAME); fnt.Height(GRID_HEIGHT * FONT_MUL);
#else
	#ifdef flagPOSIX
		#undef FONT_MUL
		#define FONT_MUL 17/20
	#endif
	#define SET_FONT fnt = Serif(GRID_HEIGHT * FONT_MUL);
#endif



#ifndef MINIMIZE
void SnapGrid(int& width, int& height) {
	int woff = width % GRID_HEIGHT;
	if (woff) width += GRID_HEIGHT-woff;
	ASSERT((width % GRID_HEIGHT) == 0);
	int hoff = height % GRID_HEIGHT;
	if (hoff) height += GRID_HEIGHT-hoff;
}
#else
#	undef GRID_HEIGHT
#	define GRID_HEIGHT 18
#	define SnapGrid(x,y)
#endif


SubMathCtrl SubMathCtrl::SignMap(Node signmap) {
	if (signmap.GetInt() != FUNC_SIGNMAP)
		return Text("Error in signmap", TYPE_ERROR);
	
	int mul = 3;
	
	SubMathCtrl plus	= Text("+", TYPE_OP);
	SubMathCtrl minus	= Text("-", TYPE_OP);
	Size plus_sz	= plus.GetSize()*mul;
	
	SubMathCtrl fn		= Exp(signmap[0][0], true);
	SubMathCtrl fn_der	= Exp(signmap[1][0], true);
	Size fn_sz		= fn.GetSize()*mul;
	Size fn_der_sz	= fn_der.GetSize()*mul;
	
	int width, height;
	width = fn_sz.cx + signmap[3].GetCount() * plus_sz.cx;
	height = 3 * fn_sz.cy;
	
	
	SubMathCtrl dw(width, height);
	
	dw.DrawMath(0, fn_sz.cy + plus_sz.cy/mul, fn_der);
	dw.DrawMath(0, fn_sz.cy*2 + plus_sz.cy/mul, fn);
	
	dw.DrawLine(0, fn_sz.cy, width, fn_sz.cy, 1);
	dw.DrawLine(0, fn_sz.cy*2, width, fn_sz.cy*2, 1);
	
	for(int i = 0; i < signmap[2].GetCount(); i++) {
		Node mn = signmap[2][i];
		SubMathCtrl draw = Exp(mn, true);
		Size draw_sz = draw.GetSize();
		int x = fn_sz.cx + (i+1) * plus_sz.cx - draw_sz.cx / 2;
		if (draw_sz.cy > plus_sz.cy || draw_sz.cx > plus_sz.cx)
			dw.DrawMath(x, 0, plus_sz.cx, fn_sz.cy, draw);
		else
			dw.DrawMath(x, 0, draw);
	}
	
	for(int i = 0; i < signmap[3].GetCount(); i++) {
		Node mn = signmap[3][i];
		int line_x = fn_sz.cx + i * plus_sz.cx;
		int x = line_x + plus_sz.cx / mul;
		int y = fn_sz.cy + plus_sz.cy / mul;
		
		if (mn.Compare(MathCore::id("plus")) == 0) {
			dw.DrawMath(x, y, plus);
			dw.DrawLine(line_x, fn_sz.cy*3, line_x+plus_sz.cx, fn_sz.cy*2, 1);
		}
		else {
			dw.DrawMath(x, y, minus);
			dw.DrawLine(line_x, fn_sz.cy*2, line_x+plus_sz.cx, fn_sz.cy*3, 1);
		}
		
		dw.DrawLine(line_x, fn_sz.cy, line_x, height, 1);
	}
	
	return dw;
}




bool force_text_color;
int force_text_type;
		
SubMathCtrl SubMathCtrl::Text(String text, int type, bool italic, int offsetX, int offsetY, Node src)
{
	Font fnt;
	
	int _pos1, _pos2;
	if ((_pos1 = text.Find('_', 0)) >= 0 && type == TYPE_ID) {
		String sub = text.Right(text.GetCount()-_pos1-1);
		if ((_pos2 = sub.Find('_', 0)) >= 0)
			sub = sub.Left(_pos2) + ',' + sub.Right(sub.GetCount()-_pos2-1);
		return SubSup(text.Left(_pos1), sub, "", src); 
	}
	
	SET_FONT;
	fnt.Italic(italic);
	
	WString wtext = text.ToWString();
	
	Size sz = GetTextSize(wtext, fnt);
	
	if (sz.cy > GRID_HEIGHT) sz.cy = GRID_HEIGHT;
	
#ifndef MINIMIZE
	int width = ((int)(sz.cx / GRID_HEIGHT + 1)) * GRID_HEIGHT;
	
	// Align center
	if ((src.GetInner()!=0) && type != TYPE_LABEL && type != TYPE_ERROR && type != TYPE_IDSUB)
		offsetX += (width - sz.cx) / 2;
#endif
	
	// Make wide enough to show cursor
	if (sz.cx < 2) sz.cx = 2;
	
	SubMathCtrl dw(offsetX + sz.cx, sz.cy);
	Color clr = Black;
	
#ifndef MINIMIZE
	if (force_text_color)
		type = force_text_type;
	switch (type) {
		case TYPE_ID: clr = Color(206,92,0); break;
		case TYPE_IDSUB: clr = Color(206,166,134); break;
		case TYPE_OP: clr = Color(32,74,135); break;
		case TYPE_INT: clr = Color(164,0,0); break;
		case TYPE_DOUBLE: clr = Color(94,53,102); break;
		case TYPE_EXERCISE:
		case TYPE_FUNCTION: clr = Color(78,154,0); break;
		case TYPE_ERROR: clr = Color(239,41,41); break;
		case TYPE_PARENTHESIS: clr = Color(85, 87, 83); break;
		case TYPE_LABEL: clr = Color(196, 160, 0); break;
		case TYPE_TYPE:	clr = Color(143, 89, 2); break;
	}
#else
	if (type == TYPE_LABEL) {
		clr = GrayColor();
	}
#endif
	
	dw.DrawText(offsetX, offsetY, wtext, fnt, clr);
	dw.SetPtr(&src);
	return dw;
}

SubMathCtrl SubMathCtrl::ParenthesisBracket(int height, bool right) {
	int mwidth = GRID_HEIGHT / 2;
	int width = mwidth * 3 / 5;
	int xoff = mwidth * 1 / 5;
	SubMathCtrl out(mwidth, height, false);
	double hstep = height / 6.0;
	Color clr(85, 87, 83);
	if (!right) {
		out.DrawLine(xoff, hstep, xoff+width, 0, 1, clr);
		out.DrawLine(xoff, height-hstep, xoff+width, height-1, 1, clr);
		out.DrawLine(xoff, hstep, xoff, height-hstep, 1, clr);
	} else {
		out.DrawLine(xoff, 0, xoff+width, hstep, 1, clr);
		out.DrawLine(xoff, height-1, xoff+width, height-hstep, 1, clr);
		out.DrawLine(xoff+width, hstep, xoff+width, height-hstep, 1, clr);
	}
	out.SetDefaultWantFocus(false);
	return out;
}


SubMathCtrl SubMathCtrl::ArrayBracket(int height, bool right) {
	int mwidth = GRID_HEIGHT / 2;
	int width = mwidth * 3 / 5;
	int xoff = mwidth * 1 / 5;
	SubMathCtrl out(mwidth, height, false);
	out.DrawLine(xoff, 0, xoff+width, 0, 1);
	out.DrawLine(xoff, height-1, xoff+width, height-1, 1);
	Color clr(85, 87, 83);
	if (!right) {
		out.DrawLine(xoff, 0, xoff, height, 1, clr);
	} else {
		out.DrawLine(xoff+width, 0, xoff+width, height, 1, clr);
	}
	out.SetDefaultWantFocus(false);
	return out;
}

SubMathCtrl SubMathCtrl::SetBracket(int height, bool right) {
	int mwidth = GRID_HEIGHT / 2;
	int w = mwidth * 3 / 5;
	int xoff = mwidth * 1 / 5;
	int h = height;
	double hstep = h / 24.0;
	double wstep = w / 2.0;
	Color clr(85, 87, 83);
	SubMathCtrl bracket(mwidth, h, false);
	if (!right) {
		bracket.DrawLine(xoff+w, 0, 			xoff+wstep, hstep, 1, clr);
		bracket.DrawLine(xoff+wstep, hstep, 	xoff+wstep, 11*hstep, 1, clr);
		bracket.DrawLine(xoff+wstep, 11*hstep, 	xoff, 12*hstep, 1, clr);
		bracket.DrawLine(xoff, 12*hstep, 		xoff+wstep, 13*hstep, 1, clr);
		bracket.DrawLine(xoff+wstep, h-hstep, 	xoff+wstep, 13*hstep, 1, clr);
		bracket.DrawLine(xoff+w, h-1, 			xoff+wstep, h-hstep, 1, clr);
		bracket.SetDefaultWantFocus(false);
	} else {
		double half = w-wstep;
		bracket.DrawLine(xoff, 0, 				xoff+half, hstep, 1, clr);
		bracket.DrawLine(xoff+half, hstep, 		xoff+half, 11*hstep, 1, clr);
		bracket.DrawLine(xoff+half, 11*hstep, 	xoff+w, 12*hstep, 1, clr);
		bracket.DrawLine(xoff+w, 12*hstep, 		xoff+half, 13*hstep, 1, clr);
		bracket.DrawLine(xoff+half, h-hstep, 	xoff+half, 13*hstep, 1, clr);
		bracket.DrawLine(xoff, h-1, 			xoff+half, h-hstep, 1, clr);
		bracket.SetDefaultWantFocus(false);
	}
	return bracket;
}

SubMathCtrl SubMathCtrl::AbsBracket(int height, bool right) {
	int mwidth = GRID_HEIGHT / 2;
	int width = mwidth * 3 / 5;
	int xoff = mwidth * 1 / 5;
	SubMathCtrl out(mwidth, height, false);
	Color clr(85, 87, 83);
	if (!right) {
		out.DrawLine(xoff, 0, xoff, height, 1, clr);
	} else {
		out.DrawLine(xoff+width, 0, xoff+width, height, 1, clr);
	}
	out.SetDefaultWantFocus(false);
	return out;
}

SubMathCtrl SubMathCtrl::SubSup(SubMathCtrl &drwText, SubMathCtrl &drwSub, SubMathCtrl &drwSup) {
	int sub_grid = GRID_HEIGHT / 2;
	Size szText = drwText.GetSize();
	Size szSub  = drwSub.GetSize();
	Size szSup  = drwSup.GetSize();
	
	Node* n = drwText.GetPtr();
	Node* text_ptr = n;
	Node* prev = 0;
	int type = text_ptr ? text_ptr->GetType() : 0;
	
	int exp = 0;
	bool last_normal = false;
	bool all_normal = true;
	
	if (!n) {
		SubMathCtrl::IteratorDeep it = drwText.BeginDeep();
		for(; !it.IsEnd() ; it++) {
			Node* ptr = it->GetPtr();
			if (ptr) {
				n = ptr;
				break;
			}
		}
	}
	
	if (!n) 
		n = drwSup.GetPtr();
	if (!n) 
		n = drwSub.GetPtr();
	
	if (n) {
		prev = n;
		n = n->GetInner();
		Node* par = n;
		while (n) {
			if (n->IsOp() && n->IsOp(OP_POW)) {
				last_normal = n->GetCount() && (*n)[0].IsEqual(*prev);
				all_normal = last_normal && all_normal;
				exp++;
			}
			else if (n->IsFunction() && n->GetInt() == FUNC_EXP) {
				last_normal = true;
				all_normal = last_normal && all_normal;
				exp++;
			}
			prev = n;
			n = n->GetInner();
		}
		//if (par) {LOG(par->AsInlineString()); LOG(par->AsString());}
	} else {
		all_normal = false;
		exp = -1;
	}
	
	if (all_normal || (exp==1 && type==TYPE_OP) ) {
		szSub *= 1/2.;
		szSup *= 1/2.;
		
		int width = szText.cx + max(szSup.cx, szSub.cx);
		int height = szText.cy;
		int deltay = GRID_HEIGHT/2;
		
		height = max(height, deltay + szSup.cy);
		
		int w=0;
		SnapGrid(w, height);
		
		SubMathCtrl dw(width, height, false);
		
		
		dw.DrawMath(0, 		  	height-szText.cy,										szText.cx, szText.cy, drwText);
		dw.DrawMath(szText.cx, 	height-deltay-szSup.cy,									szSup.cx,  szSup.cy,  drwSup);
		dw.DrawMath(szText.cx, 	height-szSub.cy,										szSub.cx,  szSub.cy,  drwSub);
		
		dw.SetDefaultWantFocus(true);
		dw.SetRightHandFocus();
		
		return dw;
	} else {
		int width = szText.cx + szSup.cx + szSub.cx;
		int height = szText.cy;
		int deltay = GRID_HEIGHT/2;
		
		if (szSup.cx)
			height = max(height, deltay + szSup.cy);
		
		SubMathCtrl dw(width, height, false);
		
		
		dw.DrawMath(0, 		  				height-szText.cy,	  			 			szText.cx, szText.cy, drwText);
		dw.DrawMath(szText.cx + szSub.cx, 	0, 			 	  							szSup.cx,  szSup.cy,  drwSup);
		dw.DrawMath(szText.cx, 				height-szSub.cy, 							szSub.cx,  szSub.cy,  drwSub);
		
		dw.SetDefaultWantFocus(true);
		dw.SetRightHandFocus();
		
		return dw;
	}
	
	
}

SubMathCtrl SubMathCtrl::SubSup(String text, String sub, String sup, Node src)
{
	SubMathCtrl textdraw = Text(text, TYPE_ID);
	SubMathCtrl subdraw  = Text(sub, TYPE_IDSUB);
	SubMathCtrl supdraw  = Text(sup, TYPE_ID);
	
	textdraw.SetPtr(&src);
	subdraw.SetPtr(&src);
	
	bool text_with_sub = false;
	if (sup.GetCount() == 0) {
		text_with_sub = true;
		supdraw.SetDefaultWantFocus(false);
	}
	
	int count = text.GetCount();
	textdraw.SetCursorEnd(count);
	subdraw.SetCursorBegin(count+1);
	subdraw.SetCursorEnd(count+1+sub.GetCount());
	
	if (!text_with_sub)
		return SubSup(textdraw, subdraw, supdraw);
	else {
		SubMathCtrl out = SubSup(textdraw, subdraw, supdraw);
		out.SetDefaultWantFocus(false);
		out.SetPtr(0);
		return out;
	}
}

SubMathCtrl SubMathCtrl::SubSup(SubMathCtrl &drwText, String sub, String sup)
{
	SubMathCtrl subdraw  = Text(sub, TYPE_IDSUB);
	SubMathCtrl supdraw  = Text(sup, TYPE_ID);
	
	return SubSup(drwText, subdraw, supdraw);
}

SubMathCtrl SubMathCtrl::SubSupInv(SubMathCtrl &drwText, SubMathCtrl &drwSub, SubMathCtrl &drwSup)
{
	Size szText = drwText.GetSize();
	Size szSub  = (1/2.)*drwSub.GetSize();
	Size szSup  = (1/2.)*drwSup.GetSize();
	
	int width = szText.cx + max(szSup.cx, szSub.cx);
	int height = szText.cy + szSup.cy/2 + szSub.cy/2;
	int deltay = szSup.cy/2;
	
	SnapGrid(width, height);
	SubMathCtrl dw(width, height);
	
	dw.DrawMath(width-szText.cx, deltay, 			  			 szText.cx, szText.cy, drwText);
	dw.DrawMath(0, 		  		0, 			 	  			     szSup.cx,  szSup.cy,  drwSup);
	dw.DrawMath(0, 		  		deltay + szText.cy - szSub.cy/2, szSub.cx,  szSub.cy,  drwSub);
	
	dw.SetDefaultWantFocus(false);
	
	return dw;
}

SubMathCtrl SubMathCtrl::SubSupInv(String text, String sub, String sup)
{
	SubMathCtrl textdraw = Text(text, TYPE_ID);
	SubMathCtrl subdraw  = Text(sub, TYPE_IDSUB);
	SubMathCtrl supdraw  = Text(sup, TYPE_ID);
	
	return SubSupInv(textdraw, subdraw, supdraw);
}

SubMathCtrl SubMathCtrl::SubSupInv(SubMathCtrl &drwText, String sub, String sup)
{
	SubMathCtrl subdraw  = Text(sub, TYPE_IDSUB);
	SubMathCtrl supdraw  = Text(sup, TYPE_ID);
	
	return SubSupInv(drwText, subdraw, supdraw);
}


SubMathCtrl SubMathCtrl::JoinBottom(SubMathCtrl &left, SubMathCtrl &right)
{
	Size szLeft = left.GetSize();
	Size szRight = right.GetSize();
	
	int width = szLeft.cx + szRight.cx;
	int height = max(szLeft.cy, szRight.cy);
	SnapGrid(width, height);
	
	SubMathCtrl dw(width, height);
	
	int x, y;
	
	x = 0;
	y = height - szLeft.cy;
#ifndef MINIMIZE
	SnapGrid(x, y);
#endif
	dw.DrawMath(x, y,  szLeft.cx,  szLeft.cy, left);
	
	x = szLeft.cx; 
	y = height - szRight.cy;
#ifndef MINIMIZE
	SnapGrid(x, y);
#endif
	
	dw.DrawMath(x, y, szRight.cx, szRight.cy, right);
	
	dw.SetDefaultWantFocus(false);
	
	return dw;
}

SubMathCtrl SubMathCtrl::JoinCenter(SubMathCtrl &left, SubMathCtrl &right)
{
	Size szLeft = left.GetSize();
	Size szRight = right.GetSize();
	
	int width = szLeft.cx + szRight.cx;
	int height = max(szLeft.cy, szRight.cy);
	SnapGrid(width, height);
	
	SubMathCtrl dw(width, height);
	
	int x, y;
	
	x = 0;
	y = height == szLeft.cy?  0: (height-szLeft.cy)/2;
	SnapGrid(x, y);
	dw.DrawMath(x, y,  szLeft.cx,  szLeft.cy, left);
	
	x = szLeft.cx; 
	y = height == szRight.cy? 0: (height-szRight.cy)/2;
	SnapGrid(x, y);
	dw.DrawMath(x, y, szRight.cx, szRight.cy, right);
	
	dw.SetDefaultWantFocus(false);
	
	return dw;
}

SubMathCtrl SubMathCtrl::JoinFlex(SubMathCtrl &left, SubMathCtrl &right)
{
	Size szLeft = left.GetSize();
	Size szRight = right.GetSize();
	
	int width = szLeft.cx + szRight.cx;
	int height = max(szLeft.cy, szRight.cy);
	SnapGrid(width, height);
	
	
	SubMathCtrl dw(width, height);
	
	int x, y;
	
	dw.DrawMath(0, 		  0, szLeft.cx,  height, left);
	
	
	x = szLeft.cx;
	y = 0;
	SnapGrid(x, y);
	dw.DrawMath(x, y, szRight.cx, height, right);
	
	dw.SetDefaultWantFocus(false);
	
	return dw;
}

SubMathCtrl SubMathCtrl::JoinVert(SubMathCtrl &num, SubMathCtrl &denom, bool centered)
{
	Size szNum = num.GetSize();
	Size szDenom = denom.GetSize();
	
	int width = max(szNum.cx, szDenom.cx);
	int height = szNum.cy + szDenom.cy; 
	SnapGrid(width, height);
	
	SubMathCtrl dw(width, height);
	
	int x, y;
	
	x = !centered || width == szNum.cx?   0: (width-szNum.cx)/2;
	y = 0;
	SnapGrid(x,y);
	
	dw.DrawMath(x,	y,		szNum.cx,  szNum.cy, num);
	x = !centered || width == szDenom.cx? 0: (width-szDenom.cx)/2;
	y = szNum.cy;
	SnapGrid(x,y);
	
	dw.DrawMath(x, y, szDenom.cx, szDenom.cy, denom);
	
	dw.SetDefaultWantFocus(false);
	
	return dw;
}

SubMathCtrl SubMathCtrl::JoinTriple(SubMathCtrl& left, SubMathCtrl& middle, SubMathCtrl& right) {
	Size szLeft = left.GetSize();
	Size szMiddle = middle.GetSize();
	Size szRight = right.GetSize();
	
	Node* ptr = middle.GetPtr();
	
	int width = szLeft.cx + szMiddle.cx + szRight.cx;
	int height = max(max(szLeft.cy, szRight.cy), szMiddle.cy);
	//SnapGrid(width, height);
	
	SubMathCtrl dw(width, height, false);
	
	int x, y;
	
	x = 0;
	y = height - szLeft.cy;
	//SnapGrid(x, y);
	dw.DrawMath(x, y,  szLeft.cx,  szLeft.cy, left);
	
	x = szLeft.cx;
	y = height - szMiddle.cy;
	//SnapGrid(x, y);
	dw.DrawMath(x, y, szMiddle.cx, szMiddle.cy, middle);
	
	x = szLeft.cx + szMiddle.cx;
	y = height - szRight.cy;
	//SnapGrid(x, y);
	dw.DrawMath(x, y, szRight.cx, szRight.cy, right);
	
	dw.SetDefaultWantFocus(false);
	
	return dw;
}

SubMathCtrl SubMathCtrl::NumDenom(SubMathCtrl &num, SubMathCtrl &denom)
{
	Size szNum = num.GetSize();
	Size szDenom = denom.GetSize();
	
	int width = max(szNum.cx, szDenom.cx);
	int height = szNum.cy + szDenom.cy; 
	int extra_width = GRID_HEIGHT;
	SnapGrid(width, height);
	
#ifdef MINIMIZE
	extra_width = 2;
#endif
	
	SubMathCtrl dw(width + extra_width*2, height);
	
	dw.DrawMath(extra_width + (width == szNum.cx?   0: (width-szNum.cx)/2), 	 0, 		szNum.cx,  szNum.cy, num);
	dw.DrawMath(extra_width + (width == szDenom.cx? 0: (width-szDenom.cx)/2), szNum.cy, szDenom.cx, szDenom.cy, denom);
	dw.DrawLine(0, szNum.cy, width+2*extra_width, szNum.cy, 1);
	
	dw.SetDefaultWantFocus(true);
	dw.SetRightHandFocus();
	
	return dw;
}

SubMathCtrl SubMathCtrl::Bracket(SubMathCtrl &data)
{
	int h = data.GetSize().cy, w = 0;
	SubMathCtrl pizq = ParenthesisBracket(h, 0);
	SubMathCtrl pder = ParenthesisBracket(h, 1);
	return JoinTriple(pizq, data, pder);
}

SubMathCtrl SubMathCtrl::Abs(SubMathCtrl &data) {
	int h = data.GetSize().cy, w = 0;
	SubMathCtrl pizq = AbsBracket(h, 0);
	SubMathCtrl pder = AbsBracket(h, 1);
	return JoinTriple(pizq, data, pder);
}

SubMathCtrl SubMathCtrl::Sqrt(SubMathCtrl &right)
{
	Size szRight = right.GetSize();
	
	int sqWidth = 20;
	int extra = 3;
	int width = sqWidth + szRight.cx + extra;
	int height = szRight.cy;
	SnapGrid(width, height);
	
	SubMathCtrl dw(width, height);
	
	dw.DrawMath(sqWidth + extra, 0, szRight.cx, height, right);
	
	Color clr = Color(32,74,135);
	dw.DrawLine(sqWidth,    1, sqWidth * 80/100, height, 1, clr);
	dw.DrawLine(width-1,    1, width-1, height * 20/100, 1, clr);
	dw.DrawLine(sqWidth * 80/100, height, sqWidth * 60/100, height * 60/100+1, 2, clr);
	dw.DrawLine(sqWidth * 60/100, height * 60/100, sqWidth * 20/100, height * 60/100, 1, clr);
	dw.DrawLine(sqWidth,    1, width, 1, 1, clr);
	
	return dw;	
}

SubMathCtrl SubMathCtrl::Integral(SubMathCtrl &data, SubMathCtrl& key, SubMathCtrl &sub, SubMathCtrl &sup)
{
	SubMathCtrl left = Text("∫", TYPE_OP);
	Size leftSz = left.GetSize();
	
	Size subSz = sub.GetSize() * 0.5;
	Size supSz = sup.GetSize() * 0.5;
	
	int width = max(leftSz.cx, subSz.cx, supSz.cx);
	int height = leftSz.cy + subSz.cy + supSz.cy;
	SnapGrid(width, height);
	SubMathCtrl in(width, height);
	
	in.DrawMath(0, supSz.cy, leftSz.cx, leftSz.cy, left);
	in.DrawMath(width-supSz.cx, 0, supSz.cx, supSz.cy, sup);
	in.DrawMath(0, supSz.cy+leftSz.cy, subSz.cx, subSz.cy, sub);
	
	
	SubMathCtrl bdata = Bracket(data);
	bdata = JoinBottom(bdata, key);
	
	return JoinBottom(in, bdata);
}

SubMathCtrl SubMathCtrl::IntegratedUnplaced(SubMathCtrl &data, SubMathCtrl &sub, SubMathCtrl &sup) {
	SubMathCtrl left = Text("/", TYPE_OP);
	Size leftSz = left.GetSize();
	
	Size subSz = sub.GetSize() * 0.5;
	Size supSz = sup.GetSize() * 0.5;
	
	int width = max(leftSz.cx, subSz.cx, supSz.cx);
	int height = leftSz.cy + subSz.cy + supSz.cy;
	SnapGrid(width, height);
	SubMathCtrl in(width, height);
	
	in.DrawMath(0, supSz.cy, leftSz.cx, leftSz.cy, left);
	in.DrawMath(width-supSz.cx, 0, supSz.cx, supSz.cy, sup);
	in.DrawMath(0, supSz.cy+leftSz.cy, subSz.cx, subSz.cy, sub);
	
	return JoinBottom(in, data);
}

SubMathCtrl SubMathCtrl::Sumat(SubMathCtrl &data, SubMathCtrl &sub, SubMathCtrl &sup)
{
	SubMathCtrl left = Text("∑", TYPE_FUNCTION, true, 0, -20);
	SubMathCtrl right = SubSupInv(data, sub, sup);
	
	Size szLeft = left.GetSize();
	Size szRight = right.GetSize();
	
	int sqWidth = szLeft.cx/3;
	int width = sqWidth + szRight.cx;
	int height = szRight.cy;
	
	SubMathCtrl dw(width, height);
	
	dw.DrawMath(0, 0, (int)(szLeft.cx*1.1),  height, left);
	dw.DrawMath(sqWidth, 0, szRight.cx, height, right);
	
	return dw;
}


SubMathCtrl SubMathCtrl::Sumat(Node expr, Node key, Node begin, Node end) {
	SubMathCtrl left = Text("∑", TYPE_FUNCTION, true, 0, -20);
	Size szLeft = left.GetSize();
	int width = szLeft.cx * 4.0;
	int height = szLeft.cy * 4.0;
	
	SubMathCtrl end_draw = Exp(end, true);
	
	SubMathCtrl key_draw = Exp(key, true);
	SubMathCtrl begin_draw   = OpTextBetween(key_draw, "=", "=", begin, false);
	
	Size begin_draw_sz = begin_draw.GetSize();
	Size end_draw_sz = end_draw.GetSize();
	
	SubMathCtrl dw(width, height);
	
	int x, y, w, h;
	
	x = width/2-end_draw_sz.cx/2;
	y = 0;
	w = end_draw_sz.cx;
	h = height/4;
	dw.DrawMath(x, y, w, h, end_draw);
	
	int sum_y = y+h;
	
	x = 0;
	y = height*3/4;
	w = width;
	h = height/4;
	dw.DrawMath(x, y, w, h, begin_draw);
	
	int sum_h = y - sum_y;
	int sum_w = sum_h;
	int sum_x = (width - sum_w) / 2;
	int sum_y0 = sum_y+2;
	int sum_y1 = sum_y+sum_h-2;
	int sum_y2 = sum_y0 + (sum_y1 - sum_y0) / 2;
	int sum_x0 = sum_x + sum_w/2;
	dw.DrawLine(sum_x, sum_y0, 		sum_x+sum_w, 	sum_y0, 1, Black);
	dw.DrawLine(sum_x, sum_y1,		sum_x+sum_w, 	sum_y1, 1, Black);
	dw.DrawLine(sum_x, sum_y0,		sum_x0, 		sum_y2, 1, Black);
	dw.DrawLine(sum_x, sum_y1,		sum_x0, 		sum_y2, 1, Black);
	
	SubMathCtrl ex = Exp(expr, true);
	return JoinCenter(dw, ex);
}

SubMathCtrl SubMathCtrl::Limit(Node expr, Node key, Node limit) {
	SubMathCtrl top = Text("lim", TYPE_FUNCTION, true, 0, 0);
	SubMathCtrl bottom_left = Exp(key, true);
	SubMathCtrl bottom_middle = Text("\342\206\222", TYPE_OP);
	SubMathCtrl bottom_right = Exp(limit, true);
	SubMathCtrl bottom = JoinTriple(bottom_left, bottom_middle, bottom_right);
	SubMathCtrl left = JoinVert(top, bottom, true);
	SubMathCtrl right = Exp(expr, false);
	return JoinCenter(left, right);
}

SubMathCtrl SubMathCtrl::Exp(SubMathCtrl &data, SubMathCtrl &exp)
{
	SubMathCtrl foo = Text("", TYPE_OTHER);
	
	return SubSup(data, foo, exp);
}

SubMathCtrl SubMathCtrl::Function(String function, SubMathCtrl &content)
{
	SubMathCtrl fundraw = Text(function, TYPE_FUNCTION);
	SubMathCtrl pardraw = Bracket(content);
	
	fundraw.SetDefaultWantFocus(false);
	
	SubMathCtrl out = JoinBottom(fundraw, pardraw);
	out.SetDefaultWantFocus(true);
	out.SetRightHandFocus();
	
	return out;
}

SubMathCtrl SubMathCtrl::Equal(SubMathCtrl &left, SubMathCtrl &right)
{
	SubMathCtrl equal = Text("=", TYPE_OP);
	equal.SetDefaultWantFocus(false);
	SubMathCtrl leftdraw = JoinBottom(left, equal);
	
	return JoinBottom(leftdraw, right);
}

String SubMathCtrl::Replace(String str, String find, String replace)
{
	String ret;
	int i = 0, j;
	int lenStr = str.GetCount();
	int lenFind = find.GetCount();
	int lenReplace = replace.GetCount();
	while ((j = str.Find(find, i)) >= i) {
		bool partial_match = false;
		int next_pos;
		if (j > 0 && !IsSpace(str[j])) {
			partial_match = true;
		}
		else if ((next_pos = j + find.GetCount()) < str.GetCount() && !IsSpace(str[next_pos])) {
			partial_match = true;
		}
		ret += str.Mid(i, j-i);
		if (!partial_match) {
			ret += replace;
		} else {
			ret += find;
		}
		i = j + lenFind;
		if (i >= lenStr)
			break;
	}
	ret += str.Mid(i);
	return ret;
}

String SubMathCtrl::ReplaceSymbols(String var)
{
	for (int i = 0; i < symbols.GetCount(); ++i) {
		const String& key = symbols.GetKey(i);
		String letter = ToLower(key);
		const WString& sym = symbols[i];
		int count = sym.GetCount();
		int split = count / 2;
		if (var.Find(letter) >= 0)
			var = Replace(var, letter, sym.Mid(split).ToString());
		letter = ToUpper(key);
		if (var.Find(letter) >= 0)
			var = Replace(var, letter, sym.Mid(0, split).ToString());
	}
	return var;	    
}

//#define REQUIRE_ARGS(x, y) if (x.GetCount() != y) {raise(SIGTERM); return Text("<wrong args>", TYPE_ERROR);}
#define REQUIRE_ARGS(x, y) if (x.GetCount() != y) {return Text("<wrong args>", TYPE_ERROR);}

SubMathCtrl SubMathCtrl::OpTextBetween(SubMathCtrl& x, const String& chr, const String& size_str, Node b, bool brackets) {
	SubMathCtrl mult = Text(chr, TYPE_OP);
	mult.SetDefaultWantFocus(false);
	SubMathCtrl left = JoinBottom(x, mult);
	SubMathCtrl y = Exp(b, !brackets);
	return JoinBottom(left, y);	
}

SubMathCtrl SubMathCtrl::OpTextBetween(SubMathCtrl& x, const String& chr, SubMathCtrl& y) {
	SubMathCtrl mult = Text(chr, TYPE_OP);
	mult.SetDefaultWantFocus(false);
	SubMathCtrl left = JoinBottom(x, mult);
	return JoinBottom(left, y);	
}

int count = 0;
SubMathCtrl SubMathCtrl::Exp(Node& n, bool nobrackets)
{
	if (!nobrackets) {
		LOG(n.AsString());
		LOG(n.AsInlineString());
		SubMathCtrl mid   = Exp(n, true);
		mid.node = &n;
		if (n.IsFunction()) {
			if (n.IsFuncAbs())
				return mid;
		}
		return Bracket(mid);
	}
	
	
	int type = n.GetType();
	int op = n.GetInt();
	if (type == TYPE_UNARY && n.GetCount() == 1) {
		SubMathCtrl x = Exp(n[0], true);
		if (op == UNARY_NEG) {
			SubMathCtrl min = Text("-", TYPE_OP);
			return JoinBottom(min, x).SetSource(n);
		}
		else if (op == UNARY_NEGSET) {
			Size sz = x.GetSize();
			SubMathCtrl mc (sz.cx, sz.cy);
			mc.DrawMath(0,0,x);
			mc.DrawLine(0,0,sz.cx-1,0, 1, Black);
			return mc;
		}
		else return x;
	}
	else if (type == TYPE_EXERCISE) {
		return Text(n.GetString(), TYPE_EXERCISE);
	}
	else if (type == TYPE_OP) {
		if (!n.GetCount())
			return Text("Error", TYPE_ERROR);
		
		SubMathCtrl x = Exp(n[0], true);
		
		if (op == OP_POW) {
			if ((n[0].IsOp() && n[0].GetCount() && n[0].GetInt() != OP_POW) || n[0].IsType() || n[0].IsUnary())
				x = Bracket(x);
			bool no_brackets = false;
			if (n[1].IsNumber() || n[1].IsId())
				no_brackets = true;
			SubMathCtrl y = Exp(n[1], no_brackets);
			return Exp(x, y).SetSource(n);
		}
		else if (op == OP_DIV) {
			SubMathCtrl y = Exp(n[1], true);
			return NumDenom(x, y).SetSource(n);
		}
		else if (op == OP_INCRATIO) {
			SubMathCtrl y = Exp(n[1], true);
			SubMathCtrl l = Text("d", TYPE_FUNCTION);
			SubMathCtrl a = JoinBottom(l, x);
			SubMathCtrl b = JoinBottom(l, y);
			return NumDenom(a, b).SetSource(n);
		}
		else if (op == OP_MUL) {
			Node n0 = n[0];
			Node n1 = n[1];
			int n0_type = n0.GetType();
			int n1_type = n1.GetType();
			
			// Remove * char between 2*x, x^2*y^2, 2*(x+y), 4*cos(x), etc.
			if ((n0_type==TYPE_ID || n0_type==TYPE_INT ||
					(n0_type==TYPE_OP && n0.GetCount() && n0[0].GetType()==TYPE_ID ) ) &&
				(n1_type==TYPE_ID || n1_type==TYPE_PARENTHESIS || n1_type==TYPE_FUNCTION ||
					(n1_type==TYPE_OP && n1.GetCount() && n1[0].GetType()==TYPE_ID ) ) ) {
				bool brackets = false;
				if (n[1].GetOpLevel() == 1)
					brackets = true;
				SubMathCtrl y = Exp(n[1], !brackets);
				return JoinBottom(x, y).SetSource(n);
			}
			
			if (n0.GetOpLevel() == 1)
				x = Bracket(x);
			bool brackets = false;
			if (n[1].GetOpLevel() == 1)
				brackets = true;
			return OpTextBetween(x, "\302\267", ".", n[1], brackets).SetSource(n);
		}
		else if (op == OP_EQ) {
			SubMathCtrl y = Exp(n[1], true);
			return Equal(x, y).SetSource(n);
		}
		else if (op == OP_BELONGS) {
			REQUIRE_ARGS(n, 2);
			SubMathCtrl x = Exp(n[0], true);
			return OpTextBetween(x, "\342\210\210", ".", n[1]).SetSource(n);
		}
		else if (op == OP_FOLLOWS) {
			REQUIRE_ARGS(n, 2);
			SubMathCtrl x = Exp(n[0], true);
			return OpTextBetween(x, "~", "~", n[1]).SetSource(n);
		}
		else if (op == OP_LABEL) {
			REQUIRE_ARGS(n, 2);
#ifndef MINIMIZE
			SubMathCtrl x = Text(n[0].GetString(), TYPE_LABEL);
			return OpTextBetween(x, ":", ":", n[1]).SetSource(n);
#else
			SubMathCtrl x = Text(":" + n[0].GetString(), TYPE_LABEL);
			SubMathCtrl y = Exp(n[1], true);
			return JoinBottom(y, x).SetSource(n);
#endif
		}
		else if (op == OP_ASSIGN) {
			REQUIRE_ARGS(n, 2);
			SubMathCtrl x = Exp(n[0], true);
			return OpTextBetween(x, "\342\207\220", "-", n[1]).SetSource(n);
		}
		else if (op == OP_LIST) {
			if (n.GetCount() == 0)
				return Text(",", TYPE_OTHER);
			SubMathCtrl comma = Text(",", TYPE_OTHER);
			SubMathCtrl left = Exp(n[0], true);
			for(int i = 1; i < n.GetCount(); i++) {
				left = JoinBottom(left, comma);
				SubMathCtrl y = Exp(n[i], true);
				left = JoinBottom(left, y);
			}
			return left.SetSource(n);
		}
		else if (op == OP_SETAND) {
			REQUIRE_ARGS(n, 2);
			SubMathCtrl x = Exp(n[0], true);
			Node n0 = n[0];
			Node n1 = n[1];
			if (!n0.IsSetAnd() && !n0.IsId() && !n0.IsNumber() && !n0.IsFunction()) x = Bracket(x);
			SubMathCtrl y = Exp(n[1], true);
			if (!n1.IsSetAnd() && !n1.IsId() && !n1.IsNumber() && !n1.IsFunction()) y = Bracket(y);
			return OpTextBetween(x, "\342\210\251", y).SetSource(n);
		}
		else if (op == OP_SETOR) {
			REQUIRE_ARGS(n, 2);
			SubMathCtrl x = Exp(n[0], true);
			Node n0 = n[0];
			Node n1 = n[1];
			if (!n0.IsSetAnd() && !n0.IsId() && !n0.IsNumber() && !n0.IsFunction()) x = Bracket(x);
			SubMathCtrl y = Exp(n[1], true);
			if (!n1.IsSetAnd() && !n1.IsId() && !n1.IsNumber() && !n1.IsFunction()) y = Bracket(y);
			return OpTextBetween(x, "\342\210\252", y).SetSource(n);
		}
		else if (op == OP_SETORDER) {
			REQUIRE_ARGS(n, 2);
			SubMathCtrl x = Exp(n[0], true);
			Node n0 = n[0];
			Node n1 = n[1];
			if (!n0.IsSetOrder() && !n0.IsId() && !n0.IsNumber() && !n0.IsFunction()) x = Bracket(x);
			SubMathCtrl y = Exp(n[1], true);
			if (!n1.IsSetOrder() && !n1.IsId() && !n1.IsNumber() && !n1.IsFunction()) y = Bracket(y);
			return OpTextBetween(y, "|", x).SetSource(n);
		}
		else if (op == OP_SETCOMPL) {
			REQUIRE_ARGS(n, 2);
			SubMathCtrl x = Exp(n[0], true);
			Node n0 = n[0];
			Node n1 = n[1];
			if (!n0.IsSetOrder() && !n0.IsId() && !n0.IsNumber() && !n0.IsFunction()) x = Bracket(x);
			SubMathCtrl y = Exp(n[1], true);
			if (!n1.IsSetOrder() && !n1.IsId() && !n1.IsNumber() && !n1.IsFunction()) y = Bracket(y);
			return OpTextBetween(x, "\\", y).SetSource(n);
		}
		
		else {
			REQUIRE_ARGS(n, 2);
			SubMathCtrl y = Exp(n[1], true);
			SubMathCtrl plus = Text(n.GetOpString(), TYPE_OP);
			plus.SetDefaultWantFocus(false);
			SubMathCtrl left = JoinBottom(x, plus);
			return JoinBottom(left, y).SetSource(n);
		}
		
	}
	else if (type == TYPE_FUNCTION) {
		int func = n.GetInt();
		String value;
		int count = n.GetCount();
		if(func == FUNC_INTEGRATE && (count == 2 || count == 4)) {
			if (count == 4) {
				SubMathCtrl data	= Exp(n[0], true);
				SubMathCtrl key		= Exp(n[1], true);
				SubMathCtrl keyd 	= Text("d", TYPE_FUNCTION, 0,0,0, n);
				key = JoinBottom(keyd, key);
				SubMathCtrl sub		= Exp(n[2], true);
				SubMathCtrl sup		= Exp(n[3], true);
				return Integral(data, key, sub, sup).SetSource(n);
			} else {
				SubMathCtrl data	= Exp(n[0], true);
				SubMathCtrl key		= Exp(n[1], true);
				SubMathCtrl keyd 	= Text("d", TYPE_FUNCTION, 0,0,0, n);
				key = JoinBottom(keyd, key);
				data = Bracket(data);
				SubMathCtrl right = JoinBottom(data, key);
				SubMathCtrl left = Text("∫", TYPE_OP);
				return JoinBottom(left, right).SetSource(n);
			}
		}
		else if (func == FUNC_INTEGRATED_UNPLACED) {
			REQUIRE_ARGS(n, 3);
			SubMathCtrl data	= Exp(n[0], true);
			SubMathCtrl sub		= Exp(n[1], true);
			SubMathCtrl sup		= Exp(n[2], true);
			return IntegratedUnplaced(data, sub, sup).SetSource(n);
		}
		else if (func == FUNC_SQRT) {
			REQUIRE_ARGS(n, 1);
			SubMathCtrl x 		= Exp(n[0], true);
			return Sqrt(x).SetSource(n);
		}
		else if (func == FUNC_FACTORIAL) {
			REQUIRE_ARGS(n, 1);
			SubMathCtrl x		= Exp(n[0], true);
			SubMathCtrl excl	= Text("!", TYPE_OP);
			return JoinBottom(x, excl).SetSource(n);
		}
		else if (func == FUNC_BINCOEFF) {
			REQUIRE_ARGS(n, 2);
			SubMathCtrl up			= Exp(n[0], true);
			SubMathCtrl down		= Exp(n[1], true);
			SubMathCtrl x = JoinVert(up, down, true);
			return Bracket(x).SetSource(n);
		}
		else if (func == FUNC_SIGNMAP) {
			return SignMap(n).SetSource(n);
		}
		else if (func == FUNC_SUM) {
			REQUIRE_ARGS(n, 4);
			return Sumat(n[0], n[1], n[2], n[3]).SetSource(n);
		}
		else if (func == FUNC_LIMIT) {
			REQUIRE_ARGS(n, 3);
			return Limit(n[0], n[1], n[2]).SetSource(n);
		}
		else if (func == FUNC_CROSS) {
			REQUIRE_ARGS(n, 2);
			SubMathCtrl left = Exp(n[0], true);
			return OpTextBetween(left, "\303\227", "x", n[1]).SetSource(n);
		}
		else if (func == FUNC_PLUSMIN) {
			REQUIRE_ARGS(n, 1);
			SubMathCtrl left = Text("\302\261", TYPE_OP);
			SubMathCtrl right = Exp(n[0], false);
			return JoinBottom(left, right).SetSource(n);
		}
		else if (func == FUNC_ABS) {
			REQUIRE_ARGS(n, 1);
			SubMathCtrl expr = Exp(n[0], true);
			return Abs(expr).SetSource(n);
		}
		else if (func == FUNC_EXP) {
			REQUIRE_ARGS(n, 1);
			SubMathCtrl e = Text("e", TYPE_ID);
			bool no_brackets = false;
			if (n[0].IsNumber() || n[0].IsId())
				no_brackets = true;
			SubMathCtrl y = Exp(n[0], no_brackets);
			y.SetSource(n[0]);
			return Exp(e, y).SetSource(n);
		}
		else if (func == FUNC_DERIVE) {
			REQUIRE_ARGS(n, 1);
			Node arg = n[0];
			if (arg.IsFunction()) {
				String value = arg.GetFunctionName() + " '";
				if (arg.GetCount() == 0) {
					SubMathCtrl x = Text("", TYPE_OTHER);
					return Function(value, x).SetSource(n);
				} else {
					SubMathCtrl x = Exp(arg[0], true);
					SubMathCtrl comma = Text(",", TYPE_OTHER);
					SubMathCtrl left = x;
					for(int i = 1; i < arg.GetCount(); i++) {
						left = JoinBottom(left, comma);
						SubMathCtrl y = Exp(arg[i], true);
						left = JoinBottom(left, y);
					}
					return Function(value, left).SetSource(n);
				}
			}
			else if (arg.IsId()) {
				String value = arg.GetString() + " '";
				return Text(value, TYPE_ID);
			}
			SubMathCtrl x = Exp(arg, false);
			SubMathCtrl der_sign =  Text("'", TYPE_OTHER);
			return JoinFlex(x, der_sign);
		}
		else if ((value = n.GetFunctionName()) == "range" && (n.GetCount() == 4 || n.GetCount() == 5)) {
			if (n.GetCount() == 4) {
				return Range(n[0],n[1],n[2],n[3]).SetSource(n);
			}
			else {
				SubMathCtrl left = Exp(n[0], true);
				SubMathCtrl op = Text("=", TYPE_OP);
				SubMathCtrl right = Range(n[1],n[2],n[3],n[4]);
				return JoinTriple(left, op, right).SetSource(n);
			}
		}
		else {
			if (n.GetCount() == 0) {
				SubMathCtrl x = Text("", TYPE_OTHER);
				return Function(value, x).SetSource(n);
			} else {
				SubMathCtrl x = Exp(n[0], true);
				SubMathCtrl comma = Text(",", TYPE_OTHER);
				SubMathCtrl left = x;
				for(int i = 1; i < n.GetCount(); i++) {
					left = JoinBottom(left, comma);
					SubMathCtrl y = Exp(n[i], true);
					left = JoinBottom(left, y);
				}
				return Function(value, left).SetSource(n);
			}
		}
	}
	else if (type == TYPE_TEXT || type == TYPE_ID || type == TYPE_VECTORID) {
		String value = n.GetString();
		String tmp = TrimQuotes(value);
		int sid = symbols.Find(tmp);
		if (sid != -1) {
			const WString& sym = symbols[sid];
			tmp = sym.Mid(sym.GetCount()/2).ToString();
			SubMathCtrl symbol = Text(tmp, TYPE_ID, 0,0,0, n);
			return symbol.SetSource(n);
		}
		tmp = ReplaceSymbols(tmp);
		if (type != TYPE_VECTORID) {
			return Text( tmp, TYPE_ID, 0,0,0, n);
		} else {
			SubMathCtrl text = Text( tmp, TYPE_ID, 0,0,0, n).SetSource(n);
			Size sz = text.GetSize();
			int height = 6;
			int width = sz.cx;
			SubMathCtrl arrow(width, height, false);
			double hstep = height / 6.0;
			arrow.DrawLine(0, hstep*3, width, hstep*3, 1, Color(206,92,0));
			Vector<Point> poly;
			poly << Point(width, hstep*3) << Point(width-height/2, hstep) << Point(width-height/2, hstep*5);
			arrow.DrawPolygon(poly, Color(206,92,0));
			SubMathCtrl arrow_draw = arrow;
			arrow_draw.SetDefaultWantFocus(false);
			text.DrawMath(0,0,arrow);
			return text;
		}
	}
	else if (type == TYPE_INT) {
		return Text(IntStr(n.GetInt()), TYPE_INT).SetSource(n);
	}
	else if (type == TYPE_DOUBLE) {
		return Text(DblStr(n.GetDouble()), TYPE_DOUBLE).SetSource(n);
	}
	else if (type == TYPE_PARENTHESIS) {
		if (n.GetCount() == 1) {
			SubMathCtrl d = Exp(n[0], true);
			return Bracket(d).SetSource(n).SetRHand();
		} else {
			if (n.GetCount() > 1) {
				SubMathCtrl left = Exp(n[0]);
				SubMathCtrl comma = Text(",", TYPE_OTHER);
				for(int i = 1; i < n.GetCount(); i++) {
					left = JoinBottom(left, comma);
					SubMathCtrl y = Exp(n[i], true);
					left = JoinBottom(left, y);
				}
				return Function(n.GetString(), left).SetSource(n).SetRHand(); 
			}
			SubMathCtrl x = Text("()", TYPE_PARENTHESIS);
			return Function(n.GetString(), x).SetSource(n).SetRHand(); 
		}
	}
	else if (type == TYPE_ARRAY) {
		if (n.GetCount() == 0)
			return Text("[]", TYPE_PARENTHESIS).SetSource(n).SetRHand();
		SubMathCtrl x = Exp(n[0], true);
		SubMathCtrl comma = Text(",", TYPE_OTHER);
		SubMathCtrl left = x;
		for(int i = 1; i < n.GetCount(); i++) {
			left = JoinBottom(left, comma);
			SubMathCtrl y = Exp(n[i], true);
			left = JoinBottom(left, y);
		}
		int height = left.GetSize().cy;
		SubMathCtrl array_left  = ArrayBracket(height, 0);
		SubMathCtrl array_right = ArrayBracket(height, 1);
		return JoinTriple(array_left, left, array_right).SetSource(n).SetRHand();
	}
	else if (type == TYPE_MATRIX) {
		if (n.GetCount() == 0)
			return Text("[]", TYPE_PARENTHESIS).SetSource(n);
		if (n.GetCount() == 1 && n[0].GetCount() == 1)
			return Exp(n[0][0], true).SetSource(n);
		int max_rows, max_cols;
		max_rows = n.GetCount();
		max_cols = 0;
		Upp::Array<Upp::Array<SubMathCtrl> > SubMathCtrls;
		Vector<int> max_col_widths, max_row_heights;
		for(int i = 0; i < n.GetCount(); i++) {
			SubMathCtrls.Add();
			int& max_row_height = max_row_heights.Add(0);
			int col_count = n[i].GetCount();
			if (col_count > max_cols) max_cols = col_count;
			for(int j = 0; j < col_count; j++) {
				int& max_col_width = (j < max_col_widths.GetCount() ? max_col_widths[j] : max_col_widths.Add(0));
				SubMathCtrl& d = SubMathCtrls[i].Add(Exp(n[i][j], true));
				Size sz = d.GetSize();
				if (sz.cy > max_row_height) max_row_height = sz.cy;
				if (sz.cx > max_col_width) max_col_width = sz.cx;
			}
		}
		int width = 0, height = 0;
		for(int i = 0; i < max_col_widths.GetCount(); i++)
			width += max_col_widths[i];
		for(int i = 0; i < max_row_heights.GetCount(); i++)
			height += max_row_heights[i];
		SubMathCtrl grid(width, height);
		
		int y = 0;
		for(int i = 0; i < n.GetCount(); i++) {
			int col_count = n[i].GetCount();
			int x = 0;
			for(int j = 0; j < col_count; j++) {
				grid.DrawMath(x, y, SubMathCtrls[i][j]);
				x += max_col_widths[j];
			}
			y += max_row_heights[i];
		}
		SubMathCtrl grid_draw = grid;
		SubMathCtrl array_left  = ArrayBracket(height, 0);
		SubMathCtrl array_right = ArrayBracket(height, 1);
		return JoinTriple(array_left, grid_draw, array_right).SetSource(n);
	}
	else if (type == TYPE_SET) {
		if (n.GetCount() == 0)
			return Text( "{}", TYPE_PARENTHESIS ).SetSource(n);
		SubMathCtrl x = Exp(n[0], true);
		SubMathCtrl comma = Text(",", TYPE_OTHER);
		SubMathCtrl left = x;
		for(int i = 1; i < n.GetCount(); i++) {
			left = JoinBottom(left, comma);
			SubMathCtrl y = Exp(n[i], true);
			left = JoinBottom(left, y);
		}
		int height = left.GetSize().cy;
		SubMathCtrl array_left  = SetBracket(height, 0);
		SubMathCtrl array_right = SetBracket(height, 1);
		return JoinTriple(array_left, left, array_right).SetSource(n);
	}
	else if (type == TYPE_LINEARSET) {
		if (n.GetCount() == 0)
			return Text( "{}", TYPE_PARENTHESIS ).SetSource(n);
		SubMathCtrl x = Exp(n[0], true);
		SubMathCtrl left = x;
		for(int i = 1; i < n.GetCount(); i++) {
			SubMathCtrl y = Exp(n[i], true);
			left = JoinVert(left, y);
		}
		Size sz = left.GetSize();
		int h = sz.cy;
		SubMathCtrl bleft = SetBracket(h, false);
		left = JoinBottom(bleft, left);
		return left.SetSource(n);
	}
	else if (type == TYPE_RANGE) {
		REQUIRE_ARGS(n, 4);
		return Range(n[0],n[1],n[2],n[3]).SetSource(n);
	}
	else if (type == TYPE_SPACE) {
		int op = n.GetInt();
		if (op == SPACE_REAL) {
			return Text("ℝ", TYPE_ID).SetSource(n);
		}
		else return Text("<unknown space>", TYPE_ID).SetSource(n);
	}
	else if (type == TYPE_NULL) {
		SubMathCtrl out(GRID_HEIGHT, GRID_HEIGHT);
		Color clr = GrayColor();
		out.DrawLine(0,0, GRID_HEIGHT-1,0, 1, clr);
		out.DrawLine(GRID_HEIGHT-1,0, GRID_HEIGHT-1,GRID_HEIGHT-1, 1, clr);
		out.DrawLine(0,GRID_HEIGHT-1, GRID_HEIGHT-1,GRID_HEIGHT-1, 1, clr);
		out.DrawLine(0,0, 0, GRID_HEIGHT-1, 1, clr);
		return out.SetSource(n);
	}
	else if (type == TYPE_TYPE) {
		REQUIRE_ARGS(n, 2);
		SubMathCtrl x = Exp(n[0], true);
		force_text_color = true;
		force_text_type = TYPE_TYPE;
		SubMathCtrl y = Exp(n[1], true);
		force_text_color = false;
		
		// Resize to half
		Size y_sz = y.GetSize();
		if (y_sz.cy > GRID_HEIGHT) {
			y_sz /= 2;
			SubMathCtrl y_half(y_sz.cx, y_sz.cy);
			Size yh_sz = y_half.GetSize();
			y_half.DrawMath(0, yh_sz.cy - y_sz.cy, y_sz.cx, y_sz.cy, y);
			return JoinBottom(x, y_half);
		} else {
			return JoinBottom(x, y);
		}
	}
	else {
		return Text(n.GetString(), TYPE_ID).SetSource(n);
	}
	return Text("Error", TYPE_ERROR).SetSource(n);
}

SubMathCtrl SubMathCtrl::Range(Node min, Node max, Node mineq, Node maxeq) {
	int min_eq = mineq.GetInt();
	int max_eq = maxeq.GetInt();
	String array_left_str  = (min_eq ? "[" : "]");
	String array_right_str = (max_eq ? "]" : "[");
	SubMathCtrl x = Exp(min, true);
	SubMathCtrl comma = Text(",", TYPE_OTHER);
	SubMathCtrl left = x;
	left = JoinBottom(left, comma);
	SubMathCtrl y = Exp(max, true);
	left = JoinBottom(left, y);
	SubMathCtrl array_left  = Text(array_left_str, TYPE_PARENTHESIS);
	left = JoinBottom(array_left, left);
	SubMathCtrl array_right = Text(array_right_str, TYPE_PARENTHESIS);
	left = JoinBottom(left, array_right);
	return left;
}

SubMathCtrl::SubMathCtrl()
{
	if (symbols.GetCount() == 0) {
		int i = 0;
		symbols.GetAdd("alpha") = "Αα"; 	
		symbols.GetAdd("nu")    = "Νν";
		symbols.GetAdd("beta")  = "Ββ"; 	
		symbols.GetAdd("xi")    = "Ξξ"; 	
		symbols.GetAdd("gamma") = "Γγ"; 	 	
		symbols.GetAdd("omicron") = "Οο"; 	
		symbols.GetAdd("delta") = "Δδ"; 	 	
		symbols.GetAdd("pi") 	= "Ππ";
		symbols.GetAdd("epsilon") = "Εε"; 	
		symbols.GetAdd("rho")   = "Ρρ"; 	
		symbols.GetAdd("zeta") 	= "Ζζ"; 	 	
		symbols.GetAdd("sigma") = "Σσ"; 	
		symbols.GetAdd("eta") 	= "Ηη"; 		
		symbols.GetAdd("tau") 	= "Ττ"; 	
		symbols.GetAdd("theta") = "Θθ"; 	 	
		symbols.GetAdd("upsilon") = "Υυ"; 	
		symbols.GetAdd("iota") 	= "Ιι"; 	 	
		symbols.GetAdd("phi") 	= "Φφ"; 	
		symbols.GetAdd("kappa") = "Κκ"; 	 	
		symbols.GetAdd("chi") 	= "Χχ"; 	 	
		symbols.GetAdd("lambda") = "Λλ"; 	
		symbols.GetAdd("psi") 	= "Ψψ"; 	
		symbols.GetAdd("mu") 	= "Μμ"; 	 	
		symbols.GetAdd("omega") = "Ωω";
		symbols.GetAdd("real")  = "ℝℝ";
		symbols.GetAdd("partdiff") = "∂∂";
		symbols.GetAdd("inf") = "∞∞";
		symbols.GetAdd("rinf") = "∞∞";
		symbols.GetAdd("nabla") = "∇∇";
		symbols.GetAdd("laplace") = "ℒℒ";
		
		
		for(; i < symbols.GetCount(); i++) {
			ASSERT(symbols[i].GetCount() == 2);
		}
		
		symbols.GetAdd("ninf") = "-∞-∞";
		symbols.GetAdd("rninf") = "-∞-∞";
		
		for(; i < symbols.GetCount(); i++) {
			ASSERT(symbols[i].GetCount() == 4);
		}
	}
	
	idraw = 0;
	cursor = 0;
	cursor_begin = 0;
	cursor_end = 0;
	node = 0;
	default_wantfocus = true;
	right_hand_focus = false;
	cursor_off = 0;
}


SubMathCtrl::SubMathCtrl(int width, int height, bool snap) {
	if (snap) SnapGrid(width, height);
	
	idraw = new ImageDraw(width, height);
	
	SetRect(0, 0, width, height);
	
	idraw->DrawRect(0, 0, width, height, GrayColor(128));
	
	idraw->Alpha().DrawRect(0, 0, width, height, GrayColor(0));
	
	cursor = 0;
	cursor_begin = 0;
	cursor_end = 0;
	node = 0;
	default_wantfocus = true;
	right_hand_focus = false;
	cursor_off = 0;
	
	
}

SubMathCtrl::SubMathCtrl(const SubMathCtrl& mc) {
	idraw = 0;
	cursor = 0;
	cursor_begin = 0;
	cursor_end = 0;
	node = 0;
	default_wantfocus = true;
	right_hand_focus = false;
	cursor_off = 0;
	
	*this = mc;
	
}

SubMathCtrl::~SubMathCtrl() {
	if (idraw) delete idraw;
	idraw = 0;
	
}

String SubMathCtrl::AsString(int indent) {
	String out;
	for(int i = 0; i < indent; i++) {
		out += "  ";
	}
	
	out +=
		"| " + IntStr(nodes.GetCount()) +
		" this=" + Format("%X", (int64)this) +
		" node=" + Format("%X", (int64)&*node) +
		" def_wantfocus=" + String(default_wantfocus?"1 ":"0 ") +
		" righthand=" + String(right_hand_focus?"1 ":"0 ") +
		(node ? node->AsDataInlineString() : "") + "\n";
	
	for(int i = 0; i < nodes.GetCount(); i++) {
		out += nodes[i].AsString(indent+1);
	}
	return out;
}

Node* SubMathCtrl::GetPtr() const {
	return node;
}

void SubMathCtrl::SetPtr(Node* ptr) {
	node = ptr;
	if (node) {
		cursor_end = node->GetString().GetCount();
	}
}

void SubMathCtrl::SetCursorBegin(int i) {
	cursor_begin = i;
}

void SubMathCtrl::SetCursorEnd(int i) {
	cursor_end = i;
}

SubMathCtrl& SubMathCtrl::operator = (const SubMathCtrl& node) {
	for(int i = 0; i < nodes.GetCount(); i++)
		RemoveChild(&nodes[i]);
	nodes.Clear();
	
	SetRect(node.GetRect());
	
	if (node.idraw) {
		img = *node.idraw;
	} else {
		img = node.img;
	}
	
	for(int i = 0; i < node.nodes.GetCount(); i++) {
		const SubMathCtrl& src  = node.nodes[i];
		SubMathCtrl& dest = nodes.Add(src);
		Add(dest);
		dest.SetRect(src.GetRect());
	}
	
	this->node = node.node;
	this->default_wantfocus = node.default_wantfocus;
	this->right_hand_focus = node.right_hand_focus;
	this->cursor_off = node.cursor_off;
	this->cursor_begin = node.cursor_begin;
	this->cursor_end = node.cursor_end;
	
	return *this;
}

void SubMathCtrl::ConvertImage() {
	if (idraw) {
		img = *idraw;
		delete idraw;
		idraw = 0;
	}
}

void SubMathCtrl::ClearImageDraw() {
	if (idraw) delete idraw;
	idraw = 0;
}

SubMathCtrl& SubMathCtrl::SetSource(Node& node) {
	SetPtr(&node);
	return *this;
}

SubMathCtrl& SubMathCtrl::SetRHand(bool b) {
	SetDefaultWantFocus(true);
	right_hand_focus = b;
	return *this;
}

void SubMathCtrl::SetNode(Node& node) {
	SubMathCtrl mc = Exp(node, true);
	
	*this = mc;
	
	EnableSubFocus(false);
	
}

void SubMathCtrl::Redraw() {
	if (node)
		SetNode(*node);
}

void SubMathCtrl::Render() {
	
}

Size SubMathCtrl::GetRenderSize() {
	
	Size sz = GetSize();
	return sz;
}

void SubMathCtrl::Layout() {
	
}

void SubMathCtrl::Paint(Draw& d) {
	Size sz = GetSize();
	
	ConvertImage();
	
	Size img_sz = img.GetSize();
	bool resize_smaller;
	if (img_sz.cx > sz.cx || img_sz.cy > sz.cy) {
		d.DrawImage(0, 0, sz.cx, sz.cy, img);
		resize_smaller = true;
	} else {
		d.DrawImage(0, 0, img_sz.cx, img_sz.cy, img);
		resize_smaller = false;
	}
	
	if (HasFocus()) {
		bool rhand = IsRightHandFocus();
		if (HasStringCursor()) {
			int type = node->GetType();
			String value = node->GetString();
			Font fnt;
			SET_FONT;
			Size xy = GetTextSize(value.Left(cursor - cursor_begin), fnt);
			int x = cursor_off + xy.cx;
			if (resize_smaller) {
				x = x * sz.cx / img_sz.cx;
			}
			if (!rhand) {
				if (x >= sz.cx)
					d.DrawRect(x-2, 0, 2, sz.cy, Black());
				else
					d.DrawRect(x, 0, 2, sz.cy, Black());
			} else {
				d.DrawRect(cursor_off + sz.cx - 2, 0, 2, sz.cy, Black());
			}
		} else {
			if (!rhand) {
				d.DrawRect(cursor_off + 0, 0, 2, sz.cy, Black());
			} else {
				d.DrawRect(cursor_off + sz.cx - 2, 0, 2, sz.cy, Black());
			}
		}
	}
}

int SubMathCtrl::GetCursorX() {
	if (!HasStringCursor())
		return 0;
	int type = node->GetType();
	String value = node->GetString();
	Font fnt;
	SET_FONT;
	Size xy = GetTextSize(value.Left(cursor - cursor_begin), fnt);
	int x = cursor_off + xy.cx;
	return x;
}

void SubMathCtrl::ResizeContent(Size new_sz) {
	Size sz = GetSize();
	double xfac = 1.0 * new_sz.cx / sz.cx;
	double yfac = 1.0 * new_sz.cy / sz.cy;
	for(int i = 0; i < nodes.GetCount(); i++) {
		SubMathCtrl& mc = nodes[i];
		Rect r = mc.GetRect();
		Size mc_size = r.GetSize();
		int x = r.left * xfac;
		int y = r.top * xfac;
		int w = mc_size.cx * xfac;
		int h = mc_size.cy * yfac;
		mc.ResizeContent(Size(w, h));
		mc.SetRect(x, y, w, h);
	}
}

void SubMathCtrl::DrawMath(int x, int y, SubMathCtrl& mc) {
	SubMathCtrl& c = nodes.Add(mc);
	Add(c);
	Size sz = mc.GetSize();
	c.ResizeContent(sz);
	c.SetRect(x, y, sz.cx, sz.cy);
}

void SubMathCtrl::DrawMath(int x, int y, int w, int h, SubMathCtrl& mc) {
	SubMathCtrl& c = nodes.Add(mc);
	Add(c);
	c.ResizeContent(Size(w, h));
	c.SetRect(x, y, w, h);
}

void SubMathCtrl::DrawLine(int x0, int y0, int x1, int y1, int line_width, Color clr) {
	if (!idraw) return;
	idraw->DrawRect(GetSize(), clr);
	idraw->Alpha().DrawLine(x0, y0, x1, y1, line_width, GrayColor(255));
}

void SubMathCtrl::DrawText(int x, int y, WString str, Font& fnt, Color clr) {
	if (!idraw) return;
	idraw->DrawRect(GetSize(), clr);
	idraw->Alpha().DrawText(x, y, str, fnt, GrayColor(255));
	cursor_off = x;
}

void SubMathCtrl::DrawPolygon(Vector<Point>& pts, Color clr) {
	if (!idraw) return;
	idraw->DrawRect(GetSize(), clr);
	idraw->Alpha().DrawPolygon(pts, GrayColor(255));
}


void SubMathCtrl::LeftDown(Point p, dword keyflags) {
	if (IsDefaultWantFocus()) {
		if (node && (node->IsTypeUsingString() || node->IsInt() || node->IsDouble())) {
			String value = node->GetString();
			int x = cursor_off;
			Font fnt;
			SET_FONT;
			bool cursor_is_set = false;
			for(int i = 0; i < value.GetCount(); i++) {
				int chr = value[i];
				int w = fnt.GetWidth(chr);
				x += w;
				if (p.x < x) {
					cursor = i;
					cursor_is_set = true;
					break;
				}
			}
			if (!cursor_is_set)
				cursor = value.GetCount();
		}
		WantFocus();
		SetFocus();
		WhenNeedsRedraw();
	}
}


void SubMathCtrl::LostFocus() {
	Refresh();
	if (cursor_is_overridden) {
		GetTopWindow()->OverrideCursor(cursor_before_override);
		cursor_is_overridden = false;
	}
}

void SubMathCtrl::MouseEnter(Point p, dword keyflags) {
	cursor_before_override = GetTopWindow()->OverrideCursor(CtrlImg::ibeam0());
	cursor_is_overridden = true;
}

void SubMathCtrl::MouseLeave() {
	GetTopWindow()->OverrideCursor(cursor_before_override);
	cursor_is_overridden = false;
}



SubMathCtrl::IteratorDeep 		SubMathCtrl::BeginDeep() { return IteratorDeep(this);}




}
