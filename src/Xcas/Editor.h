#ifndef _Xcas_Editor_h_
#define _Xcas_Editor_h_

namespace Xcas {

class Editor : public CasItem {
	
	Input			input;
	CommentOutput	comment_out;
	One<GenOutput>	output;
	Splitter		vsplit;
	
public:
	typedef Editor CLASSNAME;
	Editor();
	
	
};

}

#endif
