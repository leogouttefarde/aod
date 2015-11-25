#include "file.hpp"

#define LIGHT
#ifdef LIGHT
#include <iostream>
#include <cstdlib>

File::File (const char *path):
    _curr_line(0),
    _nb_lines(0),
    _line(),
    _file(path)
{
    if (!_file.is_open()) {
        std::cerr << "Impossible d'ouvrir '" << path << "'." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::string line;
    _nb_lines = 0;
    while (std::getline(_file, line))
        _nb_lines++;
    
    restart();
}

File::~File () {
    _file.close();
}

std::string const* File::get_line (int index) {
    if (index <= 0 || index < _curr_line || index > _nb_lines) {
        std::cerr << "Erreur lecture : " << _curr_line << " ; " << index << std::endl;
        exit(EXIT_FAILURE);
    }
    
    while (_curr_line != index) {
        std::getline(_file, _line);
        _curr_line++;
    }

    _line += '\n';
    return &_line;
}

void File::restart() {
    _curr_line = 0;
    
    _file.clear();
    _file.seekg(0, std::ios::beg);
}



#else
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

File::File (const char *path)
{
    ifstream file (path);
    
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir '" << path << "'." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::string line;
    int i = 0;
    while (std::getline(file, line))
       i++;
    
    _file.resize(i, NULL);
    
    file.clear();
    file.seekg(0, ios::beg);
    i = 0;
    while (std::getline(file, line)) {
        _file[i] = new string(line + '\n');
        i++;
    }
    
    file.close();
}

File::~File () {
    for (vector<string*>::iterator it = _file.begin() ; it != _file.end() ; ++it)
        delete *it;
}

std::string const* File::get_line (int index) const {
    return _file[index-1];
}
#endif
