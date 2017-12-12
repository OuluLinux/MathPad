#include <LibGiac/gen.h>
#include <LibGiac/prog.h>
#include <LibGiac/mathml.h>

#ifdef flagMAIN

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

// At some point Visual C compiler did not include these.
#ifdef flagMSC
extern "C" {
__declspec(noalias) void __cdecl __std_reverse_trivially_swappable_4(void * _First, void * _Last) _NOEXCEPT {
	int* f = (int*)_First;
	int* l = (int*)_Last;
	while (f != l) {
		int i = *f;
		*f = *l;
		*l = i;
		f++;
		if (f == l) break;
		l--;
	}
}
__declspec(noalias) void __cdecl __std_reverse_trivially_swappable_8(void * _First, void * _Last) _NOEXCEPT {
	typedef __int64 I64;
	I64* f = (I64*)_First;
	I64* l = (I64*)_Last;
	while (f != l) {
		I64 i = *f;
		*f = *l;
		*l = i;
		f++;
		if (f == l) break;
		l--;
	}
}
}
#endif
