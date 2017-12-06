#ifdef flagMAIN
#include "Xcas.h"
using namespace Xcas;
typedef ::Xcas::Xcas XcasMain;

GUI_APP_MAIN
{
	SetLanguage(LNG_ENGLISH);
	SetDefaultCharset(CHARSET_UTF8);
	
	XCasFs().Type("XCas files", "*.xwd")
	         .AllFilesType()
	         .DefaultExt("xwd");
	PdfFs().Type("PDF files", "*.pdf")
	       .AllFilesType()
	       .DefaultExt("pdf");
	
	LoadFromFile(callback(XcasMain::SerializeApp));
	new XcasMain;
	Ctrl::EventLoop();
	StoreToFile(callback(XcasMain::SerializeApp));
}

#endif
