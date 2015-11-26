#include "solver.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

#ifdef PAPI
#include <stdio.h>
#include <papi.h>
#endif

using namespace std;


int main (int argc, char **argv) {
    try {
        if (argc < 3) {
            cerr << "Usage : " << argv[0] << " <originalFile> <patchedFile>"
                 << endl;

            return EXIT_FAILURE;
        }

        const time_t START = time(NULL);

        #ifdef PAPI
        long long counters[3];
        int PAPI_events[] = {
            PAPI_TOT_CYC,
            PAPI_L2_DCM,
            PAPI_L2_DCA };

        PAPI_library_init(PAPI_VER_CURRENT);
        const int i = PAPI_start_counters( PAPI_events, 3 );
        (void)i;
        #endif

        Solver solver(argv[1], argv[2]);

        cerr << "Calcul des couts..." << endl;

        solver.compute_costs(true);

        cerr << "Calcul termine !" << endl;
        cerr << "Cout total : " << solver.get_min_cost() << endl;

        // solver.display();

        cerr << endl << "Ecriture du patch..." << endl;

        solver.display_solution();

        cerr << endl << "Ecriture terminee !" << endl;
        cerr << "Duree totale : " << time(NULL) - START
        	 << " secondes !" << endl;


        #ifdef PAPI
        PAPI_read_counters( counters, 3 );

        printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n", 
            counters[1],
            (double)counters[1] / (double)counters[2],
            counters[0] );
        #endif


        return EXIT_SUCCESS;
    }
    catch (const std::exception &excep) {
        cerr << endl << "Erreur : " << excep.what() << endl << endl;
        return EXIT_FAILURE;
    }
}
