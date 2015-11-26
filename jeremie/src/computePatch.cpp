#include "solver.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;

int main (int argc, char **argv) {
    try {
        if (argc < 3)
            throw runtime_error(" usage : exec <originalFile> <patchedFile>");
        
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
    catch (const std::exception &excep) {
        cerr << "\nErreur : " << excep.what() << "\n" << endl;
        return EXIT_FAILURE;
    }
}
