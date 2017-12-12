/*
	U++ port of Xcas

	27.11.2017 - Implementation begins
		- create the frame of the application with U++ classes, including:
			- code editor with correct highlighting
			- help features
			- widget groups and dummy widgets for drawers
			- turtle
			- spreadsheets
		- import drawing functions
			- math
			- spreadsheets
			- 2D graph figures
			- 3D graphs
		The task is very challenging, because of the original code being very functional code.
		The objective is to make very object-oriented clone, which matches the orignal.
		The motivation is that I couldn't pass many university math courses without xcas...
		...and I still have few courses to pass.
		This work is away from my school work, but I consider it worth doing, and it takes
		probably only 2-4 weeks, while being useful forever for everyone.
*/

#ifndef _Xcas_Xcas_h
#define _Xcas_Xcas_h

#include <LibGiac/gen.h>
#include <LibGiac/prog.h>
#include <LibGiac/mathml.h>
#include <LibGiac/alg_ext.h>
#include <LibGiac/usual.h>
#include <LibGiac/rpn.h>
#include <LibGiac/symbolic.h>
#include <LibGiac/intg.h>
#include <LibGiac/series.h>
#include <LibGiac/quater.h>

#include <CtrlLib/CtrlLib.h>
#include <PdfDraw/PdfDraw.h>
#include <CodeEditor/CodeEditor.h>
using namespace Upp;

#include "Common.h"
#include "Input.h"
#include "Editor.h"
#include "Equation.h"
#include "Graph.h"
#include "Graph3D.h"
#include "CasEdit.h"

#define LAYOUTFILE <Xcas/Xcas.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS XcasImg
#define IMAGEFILE <Xcas/Xcas.iml>
#include <Draw/iml_header.h>

namespace Xcas {


FileSel& XCasFs();
FileSel& PdfFs();


class Xcas : public TopWindow {
	CasEdit    editor;
	MenuBar    menubar;
	ToolBar    toolbar;
	StatusBar  statusbar;
	String     filename;
	
	
	static LRUList& lrufile() { static LRUList l; return l; }
	
protected:
	
	void New();
	void Open();
	void Save();
	void Save0();
	void SaveAs();
	void SaveAll();
	void Print();
	void Pdf();
	void Load(const String& filename);
	void OpenFile(const String& fn);
	void Destroy();
	void NewEntry();
	void Execute();
	void ClearAnswers();
	void RemoveRow();
	void ClearDocument();
	
	void Index();
	void About();
	
	
	void SetBar();
	void MainBar(Bar& bar);
	
	
public:
	typedef Xcas CLASSNAME;
	Xcas();
	
	virtual void DragAndDrop(Point, PasteClip& d);
	virtual void FrameDragAndDrop(Point, PasteClip& d);

	static void SerializeApp(Stream& s);
	
};

}

#endif
