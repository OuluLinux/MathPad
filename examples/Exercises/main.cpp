#include "Exercises.h"

Exercises::Exercises() {
	Add(mc.SizePos());
	
	Title("University Exercises");
	Sizeable().MaximizeBox().MinimizeBox();
	
}

GUI_APP_MAIN {
	
	Node a = Node("A") * Node(5);
	Node b = Node("Y") == Pow2(Node("X"));
	Node c = b.Solve(Node("X"), true);
	
	
	Exercises ue;
	
	ue.mc.Exercise("Test");
	ue.mc.Print("A", a);
	ue.mc.Print("B", b);
	ue.mc.Print("C", c);
	
	ue.mc.Refresh();
	
	ue.Run();
}

