/*!
 * \file      patchCost.cpp
 * \author    Jérémie Piellard
 * \author    Léo Gouttefarde
 * \date      November, 2015
 * \brief     Computes a patch's cost.
 * \warning   Usage: patchCost patchFile
 * \copyright GNU Public License.
 *
 * This is the patchCost main program, which computes a patch's cost.
 */

#include "file.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;


/*!
 * \brief  	Main program
 * \warning	Assumes the patch is a valid one
 * \param   argc    Number of arguments
 * \param   argv    Arguments
 */
int main (int argc, char **argv) {

    if (argc < 2) {
        cout << "Usage : " << argv[0] << " <patch>" << endl;
        return EXIT_FAILURE;
    }

    int cost = 0;
    File patch(argv[1]);

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
