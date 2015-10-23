#include <iostream>
#include <string>

using namespace std;

enum field {Ex, Ey, Ez};

string select(field E) {
    switch(E) {
        case Ex: return "Ex";
        case Ey: return "Ey";
        case Ez: return "Ez";
    }
}


int main() {
    cout << select(Ey) << endl;
}

