#include "Xcas.h"

#define IMAGECLASS XcasImg
#define IMAGEFILE <Xcas/Xcas.iml>
#include <Draw/iml_source.h>

namespace Xcas {

FileSel& XCasFs()
{
	static FileSel fs;
	return fs;
}

FileSel& PdfFs()
{
	static FileSel fs;
	return fs;
}



Xcas::Xcas()
{
	Title("Xcas Ultimate");
	Icon(XcasImg::icon());
	Sizeable().MaximizeBox().MinimizeBox();
	
	AddFrame(menubar);
	AddFrame(TopSeparatorFrame());
	AddFrame(toolbar);
	AddFrame(statusbar);
	Add(editor.SizePos());
	
	
	menubar.Set(THISFN(MainBar));
	WhenClose = THISBACK(Destroy);
	menubar.WhenHelp = toolbar.WhenHelp = statusbar;
	
	editor.ClearModify();
	
	SetBar();
	editor.WhenRefreshBar = THISBACK(SetBar);
	
	OpenMain();
	ActiveFocus(editor);
}

void Xcas::SerializeApp(Stream& s) {
	int version = 1;
	s / version;
	s % XCasFs()
	  % PdfFs();
	if (version >= 1)
		s % lrufile();
}

void Xcas::DragAndDrop(Point, PasteClip& d) {
	if(IsAvailableFiles(d)) {
		Vector<String> fn = GetFiles(d);
		for(int open = 0; open < 2; open++) {
			for(int i = 0; i < fn.GetCount(); i++) {
				String ext = GetFileExt(fn[i]);
				if(FileExists(fn[i]) && (ext == ".rtf" || ext == ".qtf")) {
					if(open)
						OpenFile(fn[i]);
					else {
						if(d.Accept())
							break;
						return;
					}
				}
			}
			if(!d.IsAccepted())
				return;
		}
	}
}

void Xcas::FrameDragAndDrop(Point p, PasteClip& d) {
	DragAndDrop(p, d);
}

void Xcas::SetBar() {
	toolbar.Set(THISBACK(MainBar));
}

void Xcas::MainBar(Bar& bar)
{
	bar.Sub("File", [=](Bar& bar) {
		bar.Add("New Session", CtrlImg::new_doc(), THISBACK(New))
			.Key(K_CTRL_N)
			.Help("Open new session");
		bar.Add("Open..", CtrlImg::open(), THISBACK(Open))
		   .Key(K_CTRL_O)
		   .Help("Open existing document");
		bar.Sub("Insert from file", [=](Bar& bar) {
			bar.Add("Xcas session", [&] {});
			bar.Add("Figure", [&] {});
			bar.Add("Spreadsheet", [&] {});
			bar.Add("Program", [&] {});
		});
		bar.Add("Save", CtrlImg::save(), THISBACK(Save))
			.Key(K_CTRL_S)
			.Help("Save current document");
		bar.Add("Save As", CtrlImg::save_as(), THISBACK(SaveAs))
			.Help("Save current document with a new name");
		bar.Add("Print", CtrlImg::print(), THISBACK(Print))
			.Key(K_CTRL_P)
			.Help("Print document");
		bar.Add("Export to PDF..", XcasImg::pdf(), THISBACK(Pdf))
			.Help("Export document to PDF file");
		if(bar.IsMenuBar()) {
			if(lrufile().GetCount())
				lrufile()(bar, THISBACK(OpenFile));
			bar.Separator();
			bar.Add("Exit", THISBACK(Destroy));
		}
	});
	
	bar.Sub("Edit", [=](Bar& bar) {
		bar.Add("Execute", CtrlImg::go_forward(), THISBACK(Execute))
			.Key(K_F5)
			.Help("Execute the worksheet of the currently open tab.");
		bar.Add("Clear answers", THISBACK(ClearAnswers))
			.Key(K_CTRL | K_SHIFT | K_C)
			.Help("Clear all what was printed during execution.");
		bar.Add("New Parameter", THISBACK(NewParameter))
			.Key(K_CTRL_P)
			.Help("");
		bar.Separator();
		bar.Add("Merge selected levels", THISBACK(MergeLevels));
		bar.Add("New group", THISBACK(NewGroup));
		bar.Add("Group selected levels", THISBACK(GroupSelectedLevels));
		bar.Add("Degroup selected levels", THISBACK(DegroupSelectedLevels));
	});
	
	bar.Sub("Toolbox", [=](Bar& bar) {
		bar.Add("New Entry", THISBACK(NewEntry))
			.Key(K_ALT_N)
			.Help("");
		bar.Add("New Comment", [&] {});
		//TODO: fill functions automatically from index
	});
	
	bar.Sub("Expression", [=](Bar& bar) {
		bar.Add("New Expression", [&] {editor.NewExpression();})
			.Key(K_ALT_E);
		//TODO: fill functions automatically from index
	});
	
	bar.Sub("Commands", [=](Bar& bar) {
		//TODO: fill functions automatically from index
	});
	
	bar.Sub("Program", [=](Bar& bar) {
		bar.Add("New Program", [&] {})
			.Key(K_ALT_P);
		bar.Add("debug: debug a program", [&] {});
		//TODO: fill functions automatically from index
	});
	
	bar.Sub("Graphic", [=](Bar& bar) {
		//TODO: fill functions automatically from index
	});
	
	bar.Sub("Geography", [=](Bar& bar) {
		//TODO: fill functions automatically from index
	});
	
	bar.Sub("Spreadsheet", [=](Bar& bar) {
		bar.Add("New Spreadsheet", [&] {})
			.Key(K_ALT_T);
		//TODO:
	});
	
	bar.Sub("Physics", [=](Bar& bar) {
		//TODO: fill units automatically from index
	});
	
	bar.Sub("Turtle", [=](Bar& bar) {
		bar.Add("New Turtle", [&] {})
			.Key(K_ALT_D);
		//TODO: turtle moves
	});
	
	bar.Sub("Examples", [=](Bar& bar) {
		//TODO: fill units automatically from index
	});
	
	
	bar.Sub("Help", [=](Bar& bar) {
		bar.Add("Index", THISBACK(Index));
		
		bar.Add("Help topics", [&] {
			HelpWindow help;
			help.GoTo("topic://Topic/app/main$en-us");
			help.Execute();
		})
			.Key(K_F1)
			.Help("Opens help window");
		
		bar.Add("About..", THISBACK(About));
	});
}

void Xcas::New() {
	new Xcas;
}

void Xcas::Open() {
	FileSel& fs = XCasFs();
	if(fs.ExecuteOpen())
		OpenFile(fs);
	else
		statusbar.Temporary("Loading aborted.");
}

void Xcas::Save0()
{
	lrufile().NewEntry(filename);
	if(filename.IsEmpty())
		SaveAs();
	else {
		if (SaveFile(filename, editor.GetXml())) {
			statusbar.Temporary("File " + filename + " was saved.");
			ClearModify();
		}
		else
			Exclamation("Error saving the file [* " + DeQtf(filename) + "]!");
	}
}

void Xcas::Save() {
	if(!editor.IsModified()) return;
	Save0();
}

void Xcas::SaveAs() {
	FileSel& fs = XCasFs();
	if(fs.ExecuteSaveAs()) {
		filename = fs;
		Title(filename);
		Save0();
	}
}

void Xcas::Print() {
	editor.Print();
}

void Xcas::Pdf() {
	FileSel& fs = PdfFs();
	if(!fs.ExecuteSaveAs("Output PDF file"))
		return;
	Size page = Size(3968, 6074);
	PdfDraw pdf;
	UPP::Print(pdf, editor.Get(), page);
	SaveFile(~fs, pdf.Finish());
}

void Xcas::Load(const String& name) {
	lrufile().NewEntry(name);
	editor.SetXml(LoadFile(name));
	filename = name;
	editor.ClearModify();
	Title(filename);
}

void Xcas::OpenFile(const String& fn) {
	if(filename.IsEmpty() && !editor.IsModified())
		Load(fn);
	else
		(new Xcas)->Load(fn);
}

void Xcas::Destroy() {
	if(editor.IsModified()) {
		switch(PromptYesNoCancel("Do you want to save the changes to the document?")) {
		case 1:
			Save();
			break;
		case -1:
			return;
		}
	}
	delete this;
}

void Xcas::Execute() {
	
}

void Xcas::ClearAnswers() {
	
}

void Xcas::NewParameter() {
	
}

void Xcas::MergeLevels() {
	
}

void Xcas::NewGroup() {
	
}

void Xcas::GroupSelectedLevels() {
	
}

void Xcas::DegroupSelectedLevels() {
	
}

void Xcas::NewEntry() {
	CasItem* item = editor.GetSelectedItem();
	if (item)
		item->NewEntry();
}

void Xcas::Index() {
	
}

void Xcas::About() {
	PromptOK("[A5 XCas]&Using [*^www://upp.sf.net^ Ultimate`+`+] technology.");
}

}


