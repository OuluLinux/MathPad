#include <iostream>
#define HAVE_CONFIG_H
#include <giac/giac.h>
using namespace std;
using namespace giac;

int main()
{
    while (true) {
        context ct;

        string in;
        cin >> in;
        if (in == "quit")
            break;

        stringstream ss;
        ss << in;
        ss.seekg(0);

        gen g;
        ss >> g;
        cout << protecteval(g,1,&ct) << endl;
    }
    return 0;
}

