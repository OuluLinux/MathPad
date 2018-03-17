#include "UniExercises.h"

UniExercises::UniExercises() {
	Add(mc.SizePos());
	
	Title("University Exercises");
	Sizeable().MaximizeBox().MinimizeBox();
	
}

GUI_APP_MAIN {
	
	Node a = Node("A") * Node(5);
	Node b = Node("Y") == Pow2(Node("X"));
	
	UniExercises ue;
	
	ue.mc.Exercise("Test");
	ue.mc.Print("A", a);
	ue.mc.Print("B", b);
	
	ue.mc.Refresh();
	
	ue.Run();
}
