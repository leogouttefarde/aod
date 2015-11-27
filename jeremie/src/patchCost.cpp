/*! \file applyPatch.c
 *  \brief	   Calcule le coût d'un patch.
 */

#include "file.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;


/* Suppose le patch correct */
int main (int argc, char **argv) {

    if (argc < 2) {
        cout << "Usage : " << argv[0] << " <patch>" << endl;
        return EXIT_FAILURE;
    }

    File patch(argv[1]);
    int cost = 0;

    for (unsigned int i = 1; i <= patch.nb_lines(); ++i) {
        string const* line = patch.get_line(i);
        
        if ( (*line)[0] == '+' || (*line)[0] == '=') {
            i++;
            cost += 10 + patch.get_line(i)->length();
        } else if ( (*line)[0] == 'd' ) {
            cost += 10;
        } else if ( (*line)[0] == 'D' ) {
            cost += 15;
        }
    }
    
    cout << "Le cout de ce patch est " << cost << "." << endl;

    return EXIT_SUCCESS;
}
