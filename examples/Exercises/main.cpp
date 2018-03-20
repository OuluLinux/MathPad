#include "Exercises.h"

Exercises::Exercises() {
	Add(mc.SizePos());
	
	Title("University Exercises");
	Sizeable().MaximizeBox().MinimizeBox();
	
}

void Exercises::HelloWorld() {
	Node a = Node("A") * Node(5);
	Node b = Node("Y") == Pow2(Node("X"));
	
	Node c = b;
	c.Solve(Node("X"), true);
	
	Node d = b[0] == b[1] + Node("X") * Node("X");
	Node e = d;
	e[1].Simplify();
	
	mc.Inner("Sub test 1");
	mc.Print("A", a);
	mc.Print("B", b);
	mc.Print("C", c);
	mc.Outer();
	
	mc.Inner("Sub test 2");
	mc.Print("D", d);
	mc.Print("E", e);
	mc.Outer();
	
	
	LOG(b.AsString());
	LOG(b.AsDataInlineString());
	LOG(b.AsInlineString());
}


void Exercises::ComplexAnalysis1() {
	
	{
		Node n0 = Node("z") == (Pow2(Node(-1) + Node("i")) == Pow2(Node("w")));
		Node n1 = n0[1][0];
		n1.Simplify();
		Node real = Real(n1);
		Node imag = Imag(n1);
		Node abs = Abs(n1);
		Node arg = Arg(n1);
		abs.Simplify();
		
		{
			mc.Inner("Define real part, imaginary part and absolute value");
			mc.Print("Input", n0);
			mc.Print("Input", n1);
			mc.Print("Real part: Re z =", real);
			mc.Print("Imag part: Im z =", imag);
			mc.Print("Absolute: |z| =", abs);
			mc.Outer();
		}
		
		{
			mc.Inner("Exponent form");
			mc.Print("1", n0);
			mc.Print("Arg part: arg(z) =", arg);
			
			Node n2 = Node("z") == Abs(Node("z")) * (Node("e") ^ (Node("i") * Node("arg(z)")));
			mc.Print("Form", n2);
			
			Node n3 = Node("z") == abs * (Node("e") ^ (Node("i") * arg));
			mc.Print("Form", n3);
			
			n3[1].Simplify();
			mc.Print("Answer", n3);
			
			mc.Outer();
		}
	}
	
	{
		mc.Inner("Define absolute value and argument");
		Node n0 = ParseExpression("z = ((sqrt(3))-(i))/(1+(sqrt(3))*(i))");
		mc.Print("Input", n0);
		
		Node abs = Abs(n0[1]);
		Node arg = Arg(n0[1]);
		mc.Print("Absolute: |z| =", abs);
		mc.Print("Arg part: arg(z) =", arg);
		
		mc.Outer();
	}
	
	{
		mc.Inner("Define absolute value and argument");
		Node n0 = ParseExpression("z_1 = (-1+i)^7");
		mc.Print("Input", n0);
		Node n1 = ParseExpression("z_2 = (-1+i)");
		mc.Print("Use", n1);
		
		Node abs = Abs(n1[1]);
		Node arg = Arg(n1[1]);
		abs.Simplify();
		arg.Simplify();
		mc.Print("Absolute: |z_2| =", abs);
		mc.Print("Arg part: arg(z_2) =", arg);
		
		Node n3 = Node("z_2") == abs * (Node("e") ^ (Node("i") * arg));
		mc.Print("Form", n3);
		
		Node n4 = Node("z_1") == ((abs * (Node("e") ^ (Node("i") * arg))) ^ Node(7));
		mc.Print("Simplify", n4);
		
		n4[1].Simplify();
		mc.Print("Get abs and arg", n4);
		
		abs = Abs(n4[1]);
		arg = Arg(n4[1]);
		abs.Simplify();
		arg.Simplify();
		mc.Print("Absolute: |z_1| =", abs);
		mc.Print("Arg part: arg(z_1) =", arg);
		
		mc.Outer();
	}
	
	{
		mc.Inner("Find cube root");
		
		Node n0 = ParseExpression("w = -8*i");
		mc.Print("Input", n0);
		
		mc.Print("Find", ParseExpression("w^(1/3)"));
		mc.Print("Let", ParseExpression("w = z^3"));
		mc.Print("Let", ParseExpression("z = r_z*e^(i*omega_z)"));
		mc.Print("Let", ParseExpression("w = r_w*e^(i*omega_w)"));
		
		Node abs = Abs(n0[1]);
		Node arg = Arg(n0[1]);
		abs.Simplify();
		arg.Simplify();
		mc.Print("Absolute: |w| =", abs);
		mc.Print("Arg part: arg(w) =", arg);
		
		Node n1 = (Node("z") ^3) == (abs * (Node("e") ^ (Node("i") * arg)));
		mc.Print("Solve z", n1);
		
		Node n2 = n1;
		n2.SolveComplex(Node("z"), true);
		mc.Print("Cube root", n2);
		
		
		mc.Outer();
	}
	
	{
		mc.Inner("Define complex area A");
		
		Node n0 = ParseExpression("[abs(z) < 2, re(z)>0]");
		mc.Print("Input", n0);
		
		Node n1 = ParseExpression("[sqrt(x^2+y^2)<2, x>0]");
		mc.Print("Interpreted input", n1);
		
		Node n2 = Node("A") == Set();
		n2[1].Add(BelongsTo(ParseExpression("z=r*e^(i*omega)"), Node("C")));
		n2[1].Add(BelongsTo(Node("r"), Range(0,2,0,0)));
		n2[1].Add(BelongsTo(Node("omega"), Range(-Node("pi")/2,Node("pi")/2,0,0)));
		mc.Print("Area in polar coordinates", n2);
		
		mc.Outer();
	}
	
	{
		mc.Inner("Define complex area A");
		
		Node n0 = ParseExpression("abs(z-2) > abs(z-3)");
		mc.Print("Input", n0);
		
		Node n1 = n0;
		n1.Replace(Node("z"), Node("x")+Node("i")*Node("y"));
		mc.Print("z replaced", n1);
		
		Node n2 = Sqrt((Real(n1[0][0])^2) + (Imag(n1[0][0])^2)) > Sqrt((Real(n1[1][0])^2) + (Imag(n1[1][0])^2));
		mc.Print("Simplify", n2);
		
		Node n3 = n2;
		n3 = n3[0][0] > n3[1][0];
		n3.Simplify();
		mc.Print("Solve x", n3);
		
		Node n4 = n3;
		n4.Solve(Node("x"), false);
		mc.Print("X is Re z", n4);
		
		Node n5 = Node("A") == Set();
		n5[1].Add(BelongsTo(ParseExpression("z=r*e^(i*omega)"), Node("C")));
		n5[1].Add(n4);
		mc.Print("Area in polar coordinates", n5);
		
		mc.Outer();
	}
	
	{
		mc.Inner("Define complex area A");
		
		Node n0 = ParseExpression("abs(z^2-1) < 1");
		mc.Print("Input", n0);
		
		Node n1 = n0;
		n1.Replace(Node("z"), Node("x")+Node("i")*Node("y"));
		mc.Print("z replaced", n1);
		
		Node n2 = Sqrt((Real(n1[0][0])^2) + (Imag(n1[0][0])^2)) < Node(1);
		mc.Print("Replace x,y", n2);
		
		Node n3 = ParseExpression("[x=r*cos(omega),y=r*sin(omega)]");
		mc.Print("Use", n3);
		
		Node n4 = n2;
		n4.Replace(n3[0]);
		n4.Replace(n3[1]);
		mc.Print("Simplify", n4);
		n4.Simplify();
		mc.Print("Use abs(z)=1=r fact", n4);
		
		Node n5 = n4;
		n5.Replace(Node("r"), Node(1));
		mc.Print("Simplify", n5);
		n5.Simplify();
		mc.Print("Solve omega", n5);
		
		Node n6 = n5;
		n6.SolveComplex(Node("omega"));
		mc.Print("Answer", n6);
		
		mc.Outer();
	}
}

void Exercises::ComplexAnalysis2() {
	
	
	{
		mc.Inner("Solve");
		
		Node n0 = ParseExpression("e^(i*z)=-1+i*sqrt(3)");
		mc.Print("Input", n0);
		
		Node abs = Abs(n0[1]);
		Node arg = Arg(n0[1]);
		abs.Simplify();
		arg.Simplify();
		mc.Print("Absolute: |w| =", abs);
		mc.Print("Arg part: arg(w) =", arg);
		
		Node n1 = n0[0] == (abs * (Node("e") ^ (Node("i") * arg)));
		mc.Print("Solve z", n1);
		
		Node n2 = n1;
		n2.SolveComplex(Node("z"), true);
		mc.Print("Answer", n2);
		
		mc.Outer();
	}
	
	{
		mc.Inner("Solve");
		
		Node n0 = ParseExpression("cos(z)=2");
		mc.Print("Input", n0);
		
		Node n1 = ParseExpression("cos(z)=1/2*(e^(i*z)+e^(-i*z))");
		mc.Print("From Euler's forms", n1);
		
		Node n2 = n1[1] - n0[1] == 0;
		mc.Print("Divide by e^(i*z)", n2);
		
		Node n3 = n2[0] / ParseExpression("e^(i*z)") == n2[1];
		n3.Replace(Node("z"), Node("w"));
		mc.Print("Solve w", n3);
		n3.SolveComplex(Node("w"), true);
		mc.Print("", n3);
		
		Node abs = Abs(n3[1][0]);
		abs.Simplify();
		mc.Print("Absolute: |w| =", abs);
		
		Node n4 = ParseExpression("e^(i*z)") == (abs * (Node("e") ^ (Node("i") * Node("k") * 2 * Node("pi"))));
		mc.Print("Solve z", n4);
		n4.SolveComplex(Node("z"), true);
		mc.Print("Answer", n4);
		
		mc.Outer();
	}
	
	{
		mc.Inner("Solve");
		
		Node n0 = ParseExpression("i^(-2/3)");
		mc.Print("Input", n0);
		
		Node abs = Abs(n0);
		Node arg = Arg(n0);
		abs.Simplify();
		arg.Simplify();
		mc.Print("Absolute: |w| =", abs);
		mc.Print("Arg part: arg(w) =", arg);
		
		Node n1 = n0 == (abs * (Node("e") ^ (Node("i") * (arg + Node("k") * 2 * Node("pi") / 3))));
		mc.Print("Calculate for k: 0,1,2,..", n1);
		
		for(int k = 0; k < 3; k++) {
			Node nk1 = n1[1];
			nk1.Replace(Node("k"), Node(k));
			nk1.Evaluate();
			mc.Print("Answer for k=" + IntStr(k), nk1);
		}
		mc.Outer();
	}
	
	{
		mc.Inner("Solve");
		
		Node n0 = ParseExpression("log(2+2*sqrt(3)*i)");
		mc.Print("Input", n0);
		
		Node n1 = Node("z") == n0[0];
		mc.Print("Let", n1);
		
		Node abs = Abs(n1[1]);
		Node arg = Arg(n1[1]);
		abs.Simplify();
		arg.Simplify();
		mc.Print("Absolute: |w| =", abs);
		mc.Print("Arg part: arg(w) =", arg);
		
		Node n2 = n1[0] == Log(abs * (Node("e") ^ (Node("i") * (arg + Node("k") * 2 * Node("pi")))));
		mc.Print("Simplify", n2);
		n2[1].Simplify();
		mc.Print("Answer", n2);
		
		mc.Outer();
	}
	
	{
		mc.Inner("Solve");
		
		Node n0 = ParseExpression("z = (1-i)^i");
		mc.Print("Input", n0);
		
		mc.Print("Use", ParseExpression("w^i = (e^(log(w)))^i"));
		
		Node abs = Abs(n0[1][0]);
		Node arg = Arg(n0[1][0]);
		abs.Simplify();
		arg.Simplify();
		mc.Print("Absolute: |w| =", abs);
		mc.Print("Arg part: arg(w) =", arg);
		
		Node n1 = Node("w") == (abs * (Node("e") ^ (Node("i") * (arg + Node("k") * 2 * Node("pi")))));
		mc.Print("Place", n1);
		
		Node n2 = ParseExpression("(e^(log(w)))^i");
		n2.Replace(n1);
		mc.Print("Simplify", n2);
		n2.Simplify();
		mc.Print("Answer", n2);
		
		mc.Outer();
	}
	
}

GUI_APP_MAIN {
	Exercises ue;
	
	//ue.HelloWorld();
	//ue.ComplexAnalysis1();
	ue.ComplexAnalysis2();
	
	ue.mc.Refresh();
	
	ue.Run();
}

