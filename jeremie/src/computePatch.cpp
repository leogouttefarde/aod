#include "solver.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main (int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage : " << argv[0] << " <originalFile> <patchedFile> " << endl;
        return EXIT_SUCCESS;
    }
    
    Solver solver(argv[1], argv[2]);
    
    time_t beg =  time(NULL);
    cerr << "Calcul des couts..." << endl;
    solver.compute_costs();
    cerr << "Calcul termine en " << time(NULL) - beg << " secondes !" << endl;
    cerr << "Cout total : " << solver.get_min_cost() << endl;
    
    //solver.display();
    
    cerr << "\nEcriture du patch..." << endl;
    solver.display_solution();
    std::cerr << "Ecriture terminee !" << std::endl;
}
