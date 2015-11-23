#ifndef FILE_HPP_
#define FILE_HPP_

#include <string>
#include <fstream>

/* classe pour parcourir un fichier dans l'ordre */
class File {
    private:
        int           _curr_line,
                      _nb_lines;
        std::string   _line;
        std::ifstream _file;
    
    public:
        File (const char *path);
        ~File ();
        
        inline int nb_lines () const {
            return _nb_lines;
        }
        
        /* lines start at 0
         * index must be > curr_line */
        std::string const* get_line (int index);
        
        /* sets the cursor at the beginning of the file */
        void restart();
};

#endif
