#ifndef SOLVER_HPP__
#define SOLVER_HPP__

#include "file.hpp"

#include <string>
#include <vector>

/* op > DEST => destruction
 * avec k = op - DEST */
enum {NONE=0, ADD, SUB, DEST};

// état courant
struct State {
    int cost; //cout optimal pour arriver a cet etat
    int  op;  //operation a faire pour arriver ici, on en déduit l'état précédent
    
    State ():
        cost(-1),
        op (NONE)
    {}
}; 

class Solver {
    private:
        File _source,
             _target;
        
        std::vector<State> _states;
        
    public:
        Solver (const char *source_path, const char *target_path);
        ~Solver ();
        
        void display ();
        
        void display_solution ();
        
        void write_solution ();
            
        int get_cost (int i, int j);
        
    private:
        /* expects i, j in range ;  >= 0 */
        inline int index(int i, int j) {
            return j*(_source.nb_lines()+1) + i;
        }
        
        void indices (int index, int &i, int &j) {
            j = index / (_source.nb_lines()+1);//commence à 0
            i = index % (_source.nb_lines()+1);
        }
        
        void compute_costs ();
        void compute_state (int i, int j);
        
        /* si on est dans (i,j) et est venu en faisant l'opération op,
         * met dans i et j d'ou on vient */
        void indices_from (int &i, int &j, int op);
        void indices_to (int &i, int &j, int op);
};

#endif
