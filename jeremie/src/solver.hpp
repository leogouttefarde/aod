#ifndef SOLVER_HPP__
#define SOLVER_HPP__

#include "file.hpp"

#include <vector>
#include <string>

/* enumeration qui represente une operation (les operations seront des int)
 * si op > DEST, alors l'operation est la destruction de (op - DEST) lignes
 * en particulier, op - DEST == 1  =>  destruction simple */
typedef enum Op_ {
    NONE = 0,
    ADD,
    SUB,
    DEST
} Op;

/* structure utilisée pour représenter un etat */
struct State {
    int cost; // cout optimal pour arriver a cet etat
    Op  op;  /* operation a effectuer pour arriver ici,
               * on en déduit l'état précédent */
    State ():
        cost(-1),
        op (NONE)
    {}
};

/**
 * Class used for computing an optimal patch.
 * If source file has I lines et target file has J lines,
 * stores a I*J vector of State.
 */
class Solver {

    private:
        File _source;
        File _target;
        std::vector<State> _states;

    public:

       /**
        * Constructor. Load the two files in memory.
        * @throws runtime_error if one of two paths is invalid.
        */
        Solver (const std::string &src_path, const std::string &tar_path);

       /**
        * Computes every cost. This method should be called first.
        * @param disp   Whether or not a progression indcator is shown.
        */
        void compute_costs (bool disp=false);
        
        //for debugging purposes
        void display () const;
        
       /**
        * Displays the optimal patch on the cout stream.
        * compute_costs() must have been called first.
        */
        void display_solution ();
        
       /**
        * Gives the cost of theoptimal patch.
        * compute_costs() must have been called first
        */
        int get_min_cost () const;
        
    private:
        
       /**
        * Since the grid is stored in a 1D array, makes the conversion
        * from 2D coords to 1D coord.
        */
        inline int index (int i, int j) const {
            return j*(_source.nb_lines()+1) + i;
        }
        
       /**
        * Gives the minimal cost for a certain state/
        */
        inline int get_cost (int i, int j) const {
             return _states[index(i,j)].cost;
        }
        
        /**
         * Computes states on the sides.
         */
        void compute_sides ();
        
        void compute_line (int j);
        
       /**
        * Computes the next coordinates, assuming we are in (i,j)
        * and we do the operation op.
        * Stores the result in i and j.
        */
        void next_indices (int &i, int &j, Op op) const;
};

#endif
