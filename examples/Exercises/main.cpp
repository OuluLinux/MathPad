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
	
	{
		mc.Inner("Get real and imaginary parts");
		
		Node n0 = ParseExpression("f(z)=z^3");
		mc.Print("Input", n0);
		
		n0[1].Replace(Node("z"), ParseExpression("x+i*y"));
		mc.Print("Input", n0);
		
		Node re = Real(n0[1]);
		Node im = Imag(n0[1]);
		mc.Print("Real part", re);
		mc.Print("Imaginary part", im);
		
		mc.Outer();
	}
	
	{
		mc.Inner("Define complex function f(z)");
		
		Node in0 = ParseExpression("u(x,y)=x^2+x-y^2");
		Node in1 = ParseExpression("v(x,y)=2*x*y-y");
		mc.Print("Input", in0);
		mc.Print("Input", in1);
		
		Node x = ParseExpression("x = 1/2*(z+zv)");
		Node y = ParseExpression("y = 1/(2*i)*(z+zv)");
		mc.Print("Real part", x);
		mc.Print("Imag part", y);
		
		Node n0 = in0;
		n0[1].Replace(x);
		n0[1].Replace(y);
		mc.Print("Simplify", n0);
		n0[1].Simplify();
		mc.Print("u", n0);
		
		Node n1 = in1;
		n1[0] = Node("i") * n1[0];
		n1[1] = Node("i") * n1[1];
		n1[1].Replace(x);
		n1[1].Replace(y);
		mc.Print("Simplify", n1);
		n1[1].Simplify();
		mc.Print("v", n1);
		
		Node n2 = ParseExpression("f(z)") == (n0[1] + n1[1]);
		mc.Print("Simplify", n2);
		n2[1].Simplify();
		mc.Print("Answer", n2);
		
		mc.Outer();
	}
	
	{
		mc.Inner("Form a picture of unit square with function f(z)=z^2");
		
		Node input = ParseExpression("f(z)=z^2");
		mc.Print("Input", input);
		
		Node z = ParseExpression("z=x+i*y");
		Node n0 = input;
		n0.Replace(z);
		mc.Print("Input", n0);
		
		Node n1 = ParseExpression("f(z)=u(x,y)+i*v(x,y)");
		mc.Print("Input", n1);
		
		for(int i = 0; i < 4; i++) {
			Node x, y, f = n0, u, v;
			switch(i) {
				case 0:
					x = BelongsTo(Node("x"), Range(0,1,1,1));
					y = Node("y") == 0;
					f.Replace(Node("y"), Node(0));
					break;
				case 1:
					y = BelongsTo(Node("y"), Range(0,1,1,1));
					x = Node("x") == 0;
					f.Replace(Node("x"), Node(0));
					break;
				case 2:
					x = BelongsTo(Node("x"), Range(0,1,1,1));
					y = Node("y") == 1;
					f.Replace(Node("y"), Node(1));
					break;
				case 3:
					y = BelongsTo(Node("y"), Range(0,1,1,1));
					x = Node("x") == 1;
					f.Replace(Node("x"), Node(1));
					break;
			}
			
			mc.Print("Side " + IntStr(i) + " x", x);
			mc.Print("Side " + IntStr(i) + " y", y);
			mc.Print("Side " + IntStr(i) + " f", f);
			f[1].Simplify();
			mc.Print("Side " + IntStr(i) + " f", f);
			
			u = ParseExpression("u(x,y)") == Real(f[1]);
			v = ParseExpression("v(x,y)") == Imag(f[1]);
			mc.Print("Side " + IntStr(i) + " v", v);
			mc.Print("Side " + IntStr(i) + " u", u);
				
		}
		mc.Outer();
	}
	
	{
		mc.Inner("Show parametric form of a line segment AB function");
		
		Node a = ParseExpression("A=1+i");
		Node b = ParseExpression("B=2+2*i");
		Node f = ParseExpression("f(z)=e^z");
		mc.Print("Input", a);
		mc.Print("Input", b);
		mc.Print("Input", f);
		
		Node n0 = Node("B") - Node("A") == b[1] - a[1];
		mc.Print("Calculate", n0);
		n0[1].Simplify();
		
		Node n1 = Node("AB") == Node("A") + Node("c") * n0[1];
		mc.Print("Equation for dots", Set().Add(n1).Add(BelongsTo(Node("c"), Range(0,1,1,1))));
		
		Node n2 = ParseExpression("c=t/(2*pi)");
		mc.Print("Polar conversion", n2);
		
		Node n3 = n1;
		n3.Replace(n2);
		n3.Replace(a);
		mc.Print("Conversed", n3);
		
		Node n4 = f;
		n4[0].Replace(Node("z"), n3[0]);
		n4[1].Replace(Node("z"), n3[1]);
		mc.Print("Answer", n4);
		
		mc.Outer();
	}
	
	{
		for(int i = 0; i < 2; i++) {
			mc.Inner("Is analytical polynome");
			
			Node in;
			if (i == 0) in = ParseExpression("P(x,y)=x^2-y^2+2*x*y*i");
			else in = ParseExpression("Q(x,y)=x^2+y^2-2*x*y*i");
			mc.Print("Input", in);
			
			Node x = ParseExpression("x = 1/2*(z+zv)");
			Node y = ParseExpression("y = 1/(2*i)*(z+zv)");
			mc.Print("Real part", x);
			mc.Print("Imag part", y);
			
			Node n0 = in;
			n0[1].Replace(x);
			n0[1].Replace(y);
			mc.Print("Simplify", n0);
			
			n0[1].Simplify();
			mc.Print("Simplify", n0);
			
			if (!n0[1].Compare(ParseExpression("(z+zv)^2")))
				mc.Print("Answer", Node("Yes"));
			else
				mc.Print("Answer", Node("No"));
			mc.Outer();
		}
	}
	
}

void Exercises::ComplexAnalysis3() {
	
	{
		for(int i = 0; i < 3; i++) {
			mc.Inner("Is analytical polynome");
			
			Node in;
			if      (i == 0) in = ParseExpression("f(z)=x^2-y^2+2*x+i*(2*x*y+2*y)");
			else if (i == 1) in = ParseExpression("f(z)=x^3-3*x*y^2+i*(3*(x^2)*y-y^3)");
			else if (i == 2) in = ParseExpression("f(z)=cos(x+i*y)");
			mc.Print("Input", in);
			
			Node n0 = Set().Add(FunctionDerive(Node("u"), Node("x")) == FunctionDerive(Node("v"), Node("y"))).Add(FunctionDerive(Node("u"), Node("y")) == -1 * FunctionDerive(Node("v"), Node("x")));
			mc.Print("Cauchy-Riemann rule for analytical polynomes", n0);
			
			Node u = Real(in[1]);
			Node v = Imag(in[1]);
			mc.Print("Real part: u", u);
			mc.Print("Imag part: v", v);
			
			Node n1a = Derive(u, Node("x"));
			Node n1b = Derive(v, Node("y"));
			mc.Print("u derived by x", n1a);
			mc.Print("y derived by y", n1b);
			bool a_part_true = TestEqual(n1a, n1b);
			
			if (a_part_true)
				mc.Print("Is Equal", Node("Yes"));
			else
				mc.Print("Is Equal", Node("No"));
			
			Node n2a = Derive(u, Node("y"));
			Node n2b = Derive(v, Node("x"));
			mc.Print("u derived by y", n2a);
			mc.Print("y derived by x", n2b);
			bool b_part_true = TestEqual(n2a, n2b * -1);
			
			if (b_part_true)
				mc.Print("Is (-1*) Equal", Node("Yes"));
			else
				mc.Print("Is Equal", Node("No"));
			
			bool is_analytical = a_part_true && b_part_true;
			if (is_analytical)
				mc.Print("Is analytical", Node("Yes"));
			else
				mc.Print("Is analytical", Node("No"));
			
			
			if (is_analytical) {
				
				Node n3 = Derive(u, Node("x")) + Node("i") * Derive(v, Node("x"));
				mc.Print("Derived. Simplify.", n3);
				
				n3.Simplify();
				mc.Print("Answer", n3);
				
			}
			
			mc.Outer();
		}
	}
	
	{
		mc.Inner("Is harmonic polynome");
		
		Node in = ParseExpression("u(x,y)=y^3-3*x^2*y");
		mc.Print("Input", in);
		
		Node n0 = FunctionDerive(Node("u"), Pow2(Node("y"))) == -1 * FunctionDerive(Node("u"), Pow2(Node("x")));
		mc.Print("Rule for harmonic function", n0);
	
		Node u = in[1];
		
		Node n2a = Derive(Derive(u, Node("y")), Node("y"));
		Node n2b = Derive(Derive(u, Node("x")), Node("x"));
		mc.Print("u derived by y^2", n2a);
		mc.Print("y derived by x^2", n2b);
		bool is_harmonic = TestEqual(n2a, n2b * -1);
		
		if (is_harmonic)
			mc.Print("Is harmonic", Node("Yes"));
		else
			mc.Print("Is harmonic", Node("No"));
		
		
		if (is_harmonic) {
			
			Node n3 = Integrate(Derive(u, Node("y")), Node("y")) - Node("i") * Integrate(Derive(u, Node("x")), Node("x")) + Node("C");
			mc.Print("Simplify", n3);
			
			n3.Simplify();
			mc.Print("Simplify", n3);
		}
		
		mc.Outer();
	}
	
	{
		mc.Inner("Conformity of function f(z)=z^2");
		
		Node f = ParseExpression("f(z)=z^2");
		Node z0 = ParseExpression("z_0=1+2*i");
		Node t0 = Real(z0[1]);
		Node t1 = Imag(z0[1]);
		Node c1 = ParseExpression("c_1=1+i*t");
		Node c2 = ParseExpression("c_2=t+2*t*i");
		
		mc.Print("f", f);
		mc.Print("intersection z0", z0);
		mc.Print("t1", t0);
		mc.Print("t2", t1);
		mc.Print("c1", c1);
		mc.Print("c2", c2);
		
		Node d1a = f;
		d1a[1].Replace(Node("z"), c1[1]);
		mc.Print("d1 Simplify", d1a);
		d1a[1].Simplify();
		mc.Print("d1", d1a);
		
		Node d2a = f;
		d2a[1].Replace(Node("z"), c2[1]);
		mc.Print("d2 Simplify", d2a);
		d2a[1].Simplify();
		mc.Print("d2", d2a);
		
		Node c1d = Derive(c1[1], Node("t"));
		mc.Print("c1 derived", c1d);
		Node c2d = Derive(c2[1], Node("t"));
		mc.Print("c2 derived", c2d);
		Node d1d = Derive(d1a[1], Node("t"));
		mc.Print("d1 derived", d1d);
		Node d2d = Derive(d2a[1], Node("t"));
		mc.Print("d2 derived", d2d);
		
		Node c1dt = c1d;
		c1dt.Replace(Node("t"), t1);
		mc.Print("c1 derived t2: simplify", c1dt);
		c1dt.Simplify();
		Node c1dt_abs = Abs(c1dt);
		mc.Print("c1 derived t2 abs", c1dt_abs);
		Node c1dt_arg = Arg(c1dt);
		mc.Print("c1 derived t2 arg", c1dt_arg);
		
		Node d1dt = d1d;
		d1dt.Replace(Node("t"), t1);
		mc.Print("d1 derived t2: simplify", d1dt);
		d1dt.Simplify();
		Node d1dt_abs = Abs(d1dt);
		mc.Print("d1 derived t2 abs", d1dt_abs);
		d1dt_abs.Simplify();
		mc.Print("d1 derived t2 abs", d1dt_abs);
		Node d1dt_arg = Arg(d1dt);
		mc.Print("d1 derived t2 arg", d1dt_arg);
		
		mc.Print("abs derived d1 t2 == r * abs derived c1 t2", Node());
		mc.Print("arg derived d1 t2 == r * arg derived c1 t2 + omega", Node());
		
		Node c2dt = c2d;
		c2dt.Replace(Node("t"), t0);
		mc.Print("c2 derived t1: simplify", c2dt);
		c2dt.Simplify();
		Node c2dt_abs = Abs(c2dt);
		mc.Print("c2 derived t1 abs", c2dt_abs);
		Node c2dt_arg = Arg(c2dt);
		mc.Print("c2 derived t1 arg", c2dt_arg);
		
		Node d2dt = d2d;
		d2dt.Replace(Node("t"), t0);
		mc.Print("d2 derived t1: simplify", d2dt);
		d2dt.Simplify();
		Node d2dt_abs = Abs(d2dt);
		mc.Print("d2 derived t1 abs", d2dt_abs);
		d2dt_abs.Simplify();
		mc.Print("d2 derived t1 abs", d2dt_abs);
		Node d2dt_arg = Arg(d2dt);
		mc.Print("d2 derived t1 arg", d2dt_arg);
		
		mc.Print("abs derived d2 t1 == r * abs derived c2 t1", Node());
		mc.Print("arg derived d2 t1 == r * arg derived c2 t1 + omega", Node());
				
		mc.Outer();
	}
	
	{
		mc.Inner("Conformity of function f(z)=z^2");
		
		Node in = ParseExpression("f(z)=z+1/z");
		mc.Print("Input", in);
		
		Node n0 = in;
		n0.Replace(Node("z"), ParseExpression("x+i*y"));
		
		Node u = Real(n0[1]);
		Node v = Imag(n0[1]);
		mc.Print("Real part: u", u);
		mc.Print("Imag part: v", v);
		
		Node n1 = Set().Add(FunctionDerive(Node("u"), Node("x")) == FunctionDerive(Node("v"), Node("y"))).Add(FunctionDerive(Node("u"), Node("y")) == -1 * FunctionDerive(Node("v"), Node("x")));
		mc.Print("Cauchy-Riemann rule for analytical polynomes", n1);
		
		Node udx = u;
		udx.Derive(Node("x"));
		mc.Print("u derived by x", udx);
		
		Node vdy = v;
		vdy.Derive(Node("y"));
		mc.Print("v derived by y", vdy);
		
		if (TestEqual(udx, vdy)) mc.Print("They are equal", Node(""));
		else mc.Print("They are not equal", Node(""));
		
		Node udy = u;
		udy.Derive(Node("y"));
		mc.Print("u derived by y", udy);
		
		Node vdx = v;
		vdx.Derive(Node("x"));
		mc.Print("v derived by x", vdx);
		
		if (TestEqual(udy, -1 * vdx)) mc.Print("They are -1* equal", Node(""));
		else mc.Print("They are not -1* equal", Node(""));
		
		Node n2 = DeriveFunction(in, Node("z"));
		mc.Print("Input derived", n2);
		Node n3 = n2[1] == 0;
		mc.Print("Solve z", n3);
		n3.Solve(Node("z"));
		mc.Print("Does conform everywhere except in points", n3);
		
		Node n4 = v == Node("d");
		mc.Print("Function of current is ", n4);
		
		mc.Outer();
	}
	
	
}

		
GUI_APP_MAIN {
	Exercises ue;
	
	//ue.HelloWorld();
	//ue.ComplexAnalysis1();
	//ue.ComplexAnalysis2();
	ue.ComplexAnalysis3();
	
	ue.mc.Refresh();
	
	ue.Run();
}

