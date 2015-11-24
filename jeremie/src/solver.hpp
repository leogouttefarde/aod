#ifndef SOLVER_HPP__
#define SOLVER_HPP__

#include "file.hpp"

#include <string>
#include <vector>

/* enumeration qui represente une operation (les operations seront des int)
 * si op > DEST, alors l'operation est la destruction de (op - DEST) lignes
 * en particulier, op - DEST = 1  ->  destruction simple */
enum {NONE=0, ADD, SUB, DEST};

/* structure utilisée pour représenter un etat */
struct State {
    int cost; // cout optimal pour arriver a cet etat
    int  op;  /* operation a faire pour arriver ici,
               * on en déduit l'état précédent */
    
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
        /* si l'un des chemins est invalide
         * quitte en affichant un message d'erreur */
        Solver (const char *source_path, const char *target_path);
        
        /* calcule tous les couts */
        void compute_costs (bool disp=false);
        
        //for debugging purposes
        void display () const;
        
        /* affiche le patch optimal sur la sortie standard
         * il faut avoir appelé compute_costs() avant */
        void display_solution ();
        
        /* retourne le cout du patch minimum
         * il faut avoir appelé compute_costs() avant */
        int get_min_cost () const;
        
    private:
        
        /* convertit (i,j) en coordonnée 1D pour le tableau
         * ne vérifie pas les valeurs de i et j */
        inline int index(int i, int j) const {
            return j*(_source.nb_lines()+1) + i;
        }
        
        /* retourne le cout minimal pour arriver dans l'état (i,j) */
        inline int get_cost (int i, int j) const {
             return _states[index(i,j)].cost;
        }
        
        void indices (int index, int &i, int &j) const {
            j = index / (_source.nb_lines()+1);
            i = index % (_source.nb_lines()+1);
        }
        
        /* calcule les conditions aux bords (i=0 ou j=0) */
        void compute_sides ();
        
        /* remplit la ligne j, en commençant pas i=1 */
        void compute_line (int j);
        
        /* calcule d'ou on vient sachant qu'on est dans (i,j)
         * et qu'on est venu en faisant l'opération op,
         * met le résultat dans i et j */
        void indices_from (int &i, int &j, int op) const;
        void indices_to (int &i, int &j, int op) const;
};

#endif
