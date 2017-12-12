#ifndef _Xcas_Input_h_
#define _Xcas_Input_h_

namespace Xcas {

class XcasSyntax : public EditorSyntax {
private:
	struct Identation {
		enum Type {
			Tab = 0,
			Space,
			Unknown
		};
	};

public:
	XcasSyntax();

	virtual void Highlight(const wchar *start, const wchar *end, HighlightOutput& hls,
	                       CodeEditor *editor, int line, int pos);
	virtual void IndentInsert(CodeEditor& e, int chr, int count);
	
private:
	bool IsSeparator(const wchar* c);

	bool IsKeyword(const WString& w);
	bool IsSpecialVar(const WString& w);
	bool IsNumber(const WString& w);
	
	void InitKeywords();
	void LoadSyntax(const char* keywordsArray[], const char* specialVarsArray[]);
	
	bool             LineHasColon(const WString& line);
	int              CalculateLineIndetations(const WString& line, Identation::Type type);
	int              CalculateSpaceIndetationSize(CodeEditor& editor);
	Identation::Type FindIdentationType(CodeEditor& editor, const WString& line);
	char             GetIdentationByType(Identation::Type type);
	
private:
	Index<String> keywords;
	Index<String> specialVars;
};

void CreateXcasSyntax(One<EditorSyntax>& e);


class Input : public CodeEditor {
	
	
public:
	typedef Input CLASSNAME;
	Input();
	
	virtual bool Key(dword key, int count);
	
	Callback WhenExecute, WhenNewline;
};

}

#endif
