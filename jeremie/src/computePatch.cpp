#include "solver.hpp"
#include "mergelist.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <vector>

using namespace std;

int main (int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage : " << argv[0] << " <originalFile> <patchedFile> " << endl;
        return EXIT_SUCCESS;
    }
    /*
    Solver solver(argv[1], argv[2]);
    
    cerr << "Calcul des couts..." << endl;
    time_t beg =  time(NULL);
    solver.compute_costs(true);
    cerr << "Calcul termine en " << time(NULL) - beg << " secondes !" << endl;
    cerr << "Cout total : " << solver.get_min_cost() << endl;
    
    //solver.display();
    
    cerr << "\nEcriture du patch..." << endl;
    solver.display_solution();
    std::cerr << "Ecriture terminee !" << std::endl;
    
    while(1);*/
    
    MergeList<int> bob;
    bob.insert (3);
    
    vector<int> *vec = bob.get_elt_vect();
    
    for (unsigned int i = 0 ; i < vec->size() ; ++i)
        cerr << (*vec)[i] << endl;
    
    delete vec;
    
    bob.clear();
}
