#include "solver.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;


int main (int argc, char **argv) {
    try {
        if (argc < 3) {
            cerr << "Usage : " << argv[0] << " <originalFile> <patchedFile>"
                 << endl;

            return EXIT_FAILURE;
        }

        const time_t START = time(NULL);
        Solver solver(argv[1], argv[2]);

        cerr << "Calcul des couts..." << endl;

        solver.compute_costs(true);

        cerr << "Calcul termine !" << endl;
        cerr << "Cout total : " << solver.get_min_cost() << endl;

        // solver.display();

        cerr << endl << "Ecriture du patch..." << endl;

        solver.display_solution();

        cerr << "Ecriture terminee !" << endl << endl;
        cerr << "Duree totale : " << time(NULL) - START
        	 << " secondes !" << endl << endl;

        return EXIT_SUCCESS;
    }
    catch (const std::exception &excep) {
        cerr << endl << "Erreur : " << excep.what() << endl << endl;
        return EXIT_FAILURE;
    }
}
