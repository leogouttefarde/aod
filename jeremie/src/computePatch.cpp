#include "solver.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main (int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage : " << argv[0] << " <originalFile> <patchedFile> " << endl;
        return EXIT_FAILURE;
    }
    
    time_t beg =  time(NULL);
    Solver solver(argv[1], argv[2]);
    
    cerr << "Calcul des couts..." << endl;
    
    solver.compute_costs(true);
    cerr << "Calcul termine !\n";
    cerr << "Cout total : " << solver.get_min_cost() << endl;
    
    //solver.display();
    
    cerr << "\nEcriture du patch..." << endl;
    solver.display_solution();
    cerr << "Ecriture terminee !\n\n";
    cerr << "Duree totale : moins de " << time(NULL) - beg + 1
                                       << " secondes !\n" << endl;
}
