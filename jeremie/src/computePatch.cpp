#include "solver.hpp"

#include <iostream>
#include <cstdlib>

using namespace std;

int main (int argc, char **argv) {
    if (argc < 3) {
        cout << "Usage : " << argv[0] << " <originalFile> <patchedFile> " << endl;
        return EXIT_SUCCESS;
    }
    Solver solver(argv[1], argv[2]);
    
    //solver.display();
    solver.display_solution();
}
