#include <LibGiac/gen.h>
#include <LibGiac/prog.h>
#include <LibGiac/mathml.h>

bool is_quit;
giac::context* ctx;

int main() {
	using namespace std;
	
	ctx = new giac::context;
	
	giac::context0 = ctx;
	init_context(ctx);
	
	std::cout << "libgiac " VERSION << "\n\r";
	std::cout << "Copyright (C) 2001,2014 B. Parisse, Institut Fourier, 38402 St Martin d'Heres" << std::endl;
	std::cout << "License GNU GPLv3" << std::endl;
	
	std::string s;
	
	vector<string> history;
	int his_cur = 0;
	
	while (!is_quit) {
		std::cout << ">>> ";
		
		#if 0
		// Easy way to read command line
		std::getline (std::cin, s);
		#else
		{
			enum {
				KEY_STRING,
				KEY_ENTER,
				KEY_UP,
				KEY_DOWN,
			};
			system("stty raw");
			//while (1) {
			s.clear();
			if (his_cur) {
				s = history[history.size()-his_cur];
				cout << s;
			}
			int key = KEY_STRING;
			while (!is_quit) {
				int cmd[3] = {0,0,0};
				int len = 0;
				for (len = 0; len < 3; len++) {
					int c = cmd[len] = getchar();
					#if 0
					std::cout << c << " ";
					#elif 0
					std::cout.put(c);
					#else
					#endif
					if (len == 0) {
						if (c == 3) {
							is_quit = true;
							break;
						}
						// Escape sequence
						else if (c == 27) {
							continue;
						}
						// Enter
						else if (c == 13) {
							len = 1;
							break;
						}
						else {
							std::cout.put(c);
							len = 1;
							break;
						}
					}
				}
				if (len >= 3 && cmd[0] == 27) {
					if (cmd[1] == 91 && cmd[2] == 65)
						{key = KEY_UP; break; } // UP
					if (cmd[1] == 91 && cmd[2] == 66)
						{key = KEY_DOWN; break; } // DOWN
					if (cmd[1] == 91 && cmd[2] == 68)
						{ } // LEFT
					if (cmd[1] == 91 && cmd[2] == 67)
						{ } // RIGHT
				}
				else if (len == 1 && cmd[0] == 13) {
					key = KEY_ENTER;
					break;
				}
				else {
					for(int i = 0; i < 3 && i < len; i++)
						s.push_back(cmd[i]);
				}
			}
			system("stty cooked");
			if (is_quit) {
				std::cout << "\n";
				break;
			}
			if (key == KEY_ENTER) {
				std::cout << "\n";
			}
			if (key == KEY_UP) {
				his_cur = min<int>(his_cur+1, history.size());
				if (his_cur) {
					for(int i = 0; i < 80; i++) std::cout << " "; // clear line
					std::cout << "\r";
					continue;
				}
			}
			if (key == KEY_DOWN) {
				his_cur = max<int>(his_cur-1, 0);
				for(int i = 0; i < 80; i++) std::cout << " "; // clear line
				std::cout << "\r";
				continue;
			}
			if (0) {std::cout << "GOT: " << s << "\n";}
		}
		#endif
		
		if (s == "q" || s == "quit") break;
		if (s == "") continue;
		
		// Add to history, if this was new
		if (his_cur == 0) history.push_back(s);
		// otherwise reset to head of histry
		else {
			s = history[history.size()-his_cur];
			his_cur = 0;
		}
		
		// Call giac
		giac::gen g(s, ctx);
		
		if (is_zero(g))
			return 0;
	
		g = giac::protecteval(g, 1, ctx);
		
		// Print giac's output
		std::cout << g.print(ctx) << std::endl;
	}
	
	cleanup_context(ctx);
	delete ctx;
	
}

