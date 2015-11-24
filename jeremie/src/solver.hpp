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
        
        void compute_costs (bool disp=false);
        
        //for debugging purposes
        void display () const;
        
        void display_solution ();
        
        int get_min_cost () const;
        
        
        
    private:
        
        /* expects i, j in range ;  >= 0 */
        inline int index(int i, int j) const {
            return j*(_source.nb_lines()+1) + i;
        }
        
        inline int get_cost (int i, int j) const {
             return _states[index(i,j)].cost;
        }
        
        void indices (int index, int &i, int &j) const {
            j = index / (_source.nb_lines()+1);//commence à 0
            i = index % (_source.nb_lines()+1);
        }
        
        
        void compute_sides (); //conditions aux bords
        void compute_line (int j); //remplit la ligne j, en commencant à i=1
        
        /* calcule d'ou on vient sachant qu'on est dans (i,j) et venu en faisant l'opération op,
         * met le résultat dans i et j */
        void indices_from (int &i, int &j, int op) const;
        void indices_to (int &i, int &j, int op) const;
};

#endif
