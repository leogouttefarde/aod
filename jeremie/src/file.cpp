#include "file.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

File::File (const string &path):
    _lines()
{
    /* first we try to open the file */
    ifstream file (path.c_str());
    if (!file.is_open()) {
        cerr << "Impossible d'ouvrir '" << path << "'." << endl;
        exit(EXIT_FAILURE);
    }
    
    /* then we count the lines */
    string line;
    int i = 0;
    while (std::getline(file, line))
       i++;
    _lines.resize(i, NULL);
    
    /* then we load them */
    file.clear();
    file.seekg(0, ios::beg);
    i = 0;
    while (std::getline(file, line)) {
        _lines[i] = new string(line + '\n');
        i++;
    }
    
    file.close();
}

File::~File () {
    for (vector<string*>::iterator it = _lines.begin() ;
                                   it != _lines.end() ;
                                   ++it)
        delete *it;
}

unsigned int File::nb_lines () const {
    return _lines.size();
}

std::string const* File::get_line (unsigned int index) const {
    if (index == 0 || index > nb_lines()) {
        cerr << "Out of range index : " << index << endl;
        exit(EXIT_FAILURE);
    }
    
    return _lines[index-1];
}
