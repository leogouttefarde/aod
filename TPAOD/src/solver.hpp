#ifndef SOLVER_HPP__
#define SOLVER_HPP__

#include "file.hpp"

#include <vector>
#include <string>

/**
 * Enumeration representing an operation (inner type is an int).
 * If op > DEST, then the operation is the destruction of (op - DEST) lines.
 * Particularly, op - DEST == 1  =>  simple destruction.
 */
typedef enum Op_ {
    NONE = 0,
    ADD,
    SUB,
    DEST
} Op;

/**
 * Structure used for position state representation
 */
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
 * If source file has I lines & target file has J lines,
 * stores an I x J vector of State.
 */
class Solver {

    private:
        File _source;
        File _target;
        std::vector<State> _states;

    public:

       /**
        * Constructor. Loads the two files in memory.
        * @throws runtime_error if one of two paths is invalid.
        * @param    src_path    Source path
        * @param    tar_path    Target path
        */
        Solver (const std::string &src_path, const std::string &tar_path);

       /**
        * Computes every cost. This method should be called first.
        * @param disp   Whether or not a progression indcator is shown.
        */
        void compute_costs (bool disp=false);

       /**
        * Debug function
        */
        void display () const;
        
       /**
        * Displays the optimal patch on the cout stream.
        * compute_costs() must have been called first.
        */
        void display_solution ();
        
       /**
        * Gives the cost of the optimal patch.
        * \warning  compute_costs() must have been called first
        */
        int get_min_cost () const;

    private:

       /**
        * Since the grid is stored in a 1D array, makes the conversion
        * from 2D coordinates to 1D coordinates.
        *
        * @param    x   X coordinate
        * @param    y   Y coordinate
        */
        inline int index (int x, int y) const {
            return y * (_source.nb_lines()+1) + x;
        }

       /**
        * Gives the minimal cost for a certain state.
        *
        * @param    x   X coordinate
        * @param    y   Y coordinate
        */
        inline int get_cost (int x, int y) const {
             return _states[index(x,y)].cost;
        }

        /**
         * Computes states on the sides.
         */
        void compute_sides ();

       /**
        * Computes a given line.
        * @param    j   The line
        */
        void compute_line (int j);

       /**
        * Computes the next coordinates, assuming we are in (i,j)
        * and we do the operation op.
        * Stores the result in i and j.
        */
        void next_indices (int &i, int &j, Op op) const;
};

#endif
