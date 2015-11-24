#include "file.hpp"

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
        _line += '\n';
        _curr_line++;
    }

    return &_line;
}

void File::restart() {
    _curr_line = 0;
    
    _file.clear();
    _file.seekg(0, std::ios::beg);
}
