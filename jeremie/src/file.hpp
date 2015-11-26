#ifndef FILE_HPP_
#define FILE_HPP_

#include <vector>
#include <string>

/**
 * Class used for loading and accessing a file.
 * It loads the whole file in the memory
 */
class File {
    private:
        std::vector<std::string*> _lines;
    
    public:
       /**
        * Constructor. Loaads the file.
        * @throws   runtime_exception if invalid path
        */
        File (const std::string &path);
        ~File ();
        
        unsigned int nb_lines () const;
        
       /**
        * Method for accessing a line of the file.
        * Lines start at line 1.
        *
        * @param   index Line index. Must be >= 1 and <= nb_lines()
        * @return        const pointer to the requested line
        * @throws        range_error if index out of range.
        */
        std::string const* get_line (unsigned int index) const;
};

#endif
