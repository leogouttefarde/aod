#ifndef FILE_HPP_
#define FILE_HPP_

//version light : ne charge pas le fichier en memoire
#define LIGHT
#ifdef LIGHT
#include <string>
#include <fstream>

/* classe pour parcourir un fichier dans l'ordre (sinon quitte)*/
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
#else
#include <vector>
#include <string>

/* classe pour parcourir un fichier, peu importe l'ordre */
class File {
    private:
        std::vector<std::string*> _file;
    
    public:
        File (const char *path);
        ~File ();
        
        inline int nb_lines () const {
            return _file.size();
        }
        
        /* lines start at 1
         * index must be > curr_line */
        std::string const* get_line (int index) const;
        
        /* sets the cursor at the beginning of the file */
        /* TODO inutile ici, à supprimer */
        void restart(){};
};
#endif

#endif
