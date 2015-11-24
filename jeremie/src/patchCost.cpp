#include "file.hpp"

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

/* suppose le patch correct */
int main (int argc, char **argv) {
    if (argc < 2) {
        cout << "Usage : " << argv[0] << " <patch>" << endl;
        return EXIT_SUCCESS;
    }
    
    int cost = 0;
    File patch(argv[1]);
    for (int i=1 ; i <= patch.nb_lines() ; ++i) {
        std::string const* line = patch.get_line(i);
        
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
}
