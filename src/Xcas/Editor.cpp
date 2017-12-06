#include "Xcas.h"

namespace Xcas {

Editor::Editor() {
	Add(vsplit.HSizePos().VSizePos(0, handleh));
	
	vsplit.Vert();
	vsplit << input << comment_out;
	vsplit.SetPos(10000);
	
	height = 30 + handleh;
}

}

