#ifndef FILE_HPP_
#define FILE_HPP_

#include <vector>
#include <string>

/* Class used for loading and accessing a file. */
class File {
    private:
        std::vector<std::string*> _lines;
    
    public:
        File (const std::string &path);
        ~File ();
        
        unsigned int nb_lines () const;
        
        /* lines start at 1
         * if index out of range, exits with an error */
        std::string const* get_line (unsigned int index) const;
};

#endif
