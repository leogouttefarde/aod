#include "solver.hpp"
#include "mergelist.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <vector>

using namespace std;

#ifdef TESTS
template <typename T>
void printlist (const MergeList<T> &list) {
    vector<T> *vec = list.get_elt_vect();
    for (vector<int>::reverse_iterator it = vec->rbegin() ; it != vec->rend() ; ++it)
        cerr << *it << endl;
    delete vec;
}
#endif

int main (int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage : " << argv[0] << " <originalFile> <patchedFile> " << endl;
        return EXIT_SUCCESS;
    }
    
    time_t beg =  time(NULL);
    Solver solver(argv[1], argv[2]);
    
    cerr << "Calcul des couts..." << endl;
    
    solver.compute_costs(true);
    cerr << "Calcul termine !" << endl;
    cerr << "Cout total : " << solver.get_min_cost() << endl;
    
    //solver.display();
    
    cerr << "\nEcriture du patch..." << endl;
    solver.display_solution();
    std::cerr << "Ecriture terminee !" << std::endl;
    std::cerr << "Duree totale : moins de " << time(NULL) - beg + 1 << " secondes !" << std::endl;
#ifdef TESTS
    MergeList<int> bob, albert;
    bob.push_front (1);
    bob.push_front (2);
    bob.push_front (3);
    albert.push_front (4);
    
    cerr << "bob : ";
    printlist(bob);

    cerr << "albert : ";
    printlist(albert);
    
    bob.clear();
    bob.push_front(5);
    cerr << "bob : ";
    printlist(bob);

    albert = bob;
    albert.push_front(6);
    bob.clear();
    cerr << "albert : ";
    printlist(albert);
        cerr << "bob : ";
    printlist(bob);
#endif
}
