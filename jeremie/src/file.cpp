#include "file.hpp"

#include <fstream>
#include <stdexcept>

using namespace std;

File::File (const string &path):
    _lines(0, NULL)
{
    /* First, open the file */
    ifstream file (path.c_str());
    if (!file.is_open())
        throw std::invalid_argument("Impossible d'ouvrir : " + path);

    string line;
    while (std::getline(file, line))
        _lines.push_back(new string(line + '\n'));

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
    if (index == 0 || index > nb_lines())
        throw std::range_error ("Out of range index : " + index);
    
    return _lines[index-1];
}
