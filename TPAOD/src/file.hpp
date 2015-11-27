#ifndef FILE_HPP_
#define FILE_HPP_

#include <vector>
#include <string>

/**
 * File class for loading and accessing a text file.
 * Loads the whole file in memory for maximum performance.
 */
class File {
    private:
        std::vector<std::string*> _lines;

    public:

       /**
        * Constructor. Loads the file.
        * \param   path    Filepath
        * @throws   runtime_exception on invalid path
        */
        File (const std::string& path);

       /**
        * Destructor. Clears file from memory.
        */
        ~File ();

       /**
        * Indicates the number of lines.
        * @return   Number of lines in the file
        */
        unsigned int nb_lines () const;

       /**
        * Method for accessing a line from the file.
        * Lines start at line 1.
        *
        * @param   index Line index. Must be >= 1 and <= nb_lines()
        * @return        const pointer to the requested line
        * @throws        range_error if index out of range.
        */
        const std::string* get_line (unsigned int index) const;
};

#endif
