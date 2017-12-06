#ifdef flagMAIN

#include <LibGiac/gen.h>
#include <LibGiac/prog.h>
#include <LibGiac/mathml.h>


int main() {
	std::string s;
	
	std::cout << "libgiac " VERSION << std::endl;
	std::cout << "Copyright (C) 2001,2014 B. Parisse, Institut Fourier, 38402 St Martin d'Heres" << std::endl;
	std::cout << "License GNU GPLv3" << std::endl;
	std::cout << "Command 'f' to evaluate real number answer always" << std::endl;
	
	giac::context* ctx = new giac::context;
	giac::context0 = ctx;
	init_context(ctx);
	
	bool always_float = false;
	
	while (!std::cin.fail()) {
		std::cout << ">>> ";
		std::getline (std::cin, s);
		
		if (s == "q" || s == "quit" || std::cin.fail()) break;
		if (s == "f") {
			always_float = !always_float;
			std::cout << "always_float = " << (always_float ? "true" : "false") << std::endl;
			continue;
		}
		
		if (always_float)
			s = "evalf(" + s + ")";
		
		giac::gen g(s, ctx);
		
		if (is_zero(g))
			return 0;
		
		g = giac::protecteval(g, 1, ctx);
		
		std::cout << g.print(ctx) << std::endl;
	}
	
	cleanup_context(ctx);
	delete ctx;
	
	return 0;
}

#endif
