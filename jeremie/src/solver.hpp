#ifndef SOLVER_HPP__
#define SOLVER_HPP__

#define OLD
#ifdef OLD
    #include "file.hpp"

    #include <vector>
    #include <string>

    /* enumeration qui represente une operation (les operations seront des int)
     * si op > DEST, alors l'operation est la destruction de (op - DEST) lignes
     * en particulier, op - DEST = 1  ->  destruction simple */
    enum {NONE=0, ADD, SUB, DEST};
    typedef unsigned int Op;
    
    /* structure utilisée pour représenter un etat */
    struct State {
        int cost; // cout optimal pour arriver a cet etat
        Op  op;  /* operation a faire pour arriver ici,
                   * on en déduit l'état précédent */
        State ():
            cost(-1),
            op (NONE)
        {}
    }; 

    class Solver {
        private:
            File               _source,
                               _target;
            std::vector<State> _states;
            
        public:
            /* si l'un des chemins est invalide
             * quitte en affichant un message d'erreur */
            Solver (const std::string &src_path, const std::string &tar_path);
            
            /* calcule tous les couts
             * disp==true -> affiche l'avancement'*/
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
            inline int index (int i, int j) const {
                return j*(_source.nb_lines()+1) + i;
            }
            
            /* retourne le cout minimal pour arriver dans l'état (i,j) */
            inline int get_cost (int i, int j) const {
                 return _states[index(i,j)].cost;
            }
            
            /* calcule les conditions aux bords (i=0 ou j=0) */
            void compute_sides ();
            
            /* remplit la ligne j, en commençant pas i=1 */
            void compute_line (int j);
            
            /* calcule ou on va sachant qu'on est dans (i,j)
             * et qu'on fait l'opération op
             * met le résultat dans i et j */
            void next_indices (int &i, int &j, Op op) const;
    };
#else
    #include "file.hpp"
    #include "mergelist.hpp"

    #include <string>
    #include <list>
    #include <vector>

    /* enumeration qui represente une operation (les operations seront des int)
     * si op > DEST, alors l'operation est la destruction de (op - DEST) lignes
     * en particulier, op - DEST = 1  ->  destruction simple */
    enum {NONE=0, ADD, SUB, DEST};
    typedef int Op;

    /* structure utilisée pour représenter un etat */
    struct State {
        int            cost; // cout optimal pour arriver a cet etat
        MergeList<Op>  ops; /* operations a faire pour arriver ici */
        
        State (int c=-1):
            cost(c),
            ops ()
        {}
    }; 

    class Solver {
        private:
            File          _source,
                          _target;
            std::list<Op> *_patch; /* patch optimal, calculé dans compute_costs*/
            int           _patch_cost; /* cout du patch optimal */
            
        public:
            /* si l'un des chemins est invalide
             * quitte en affichant un message d'erreur */
            Solver (const char *source_path, const char *target_path);
            ~Solver();

            /* calcule le patch, l'écrit dans ops'*/
            void compute_costs (bool disp=false);
            
            /* affiche le patch optimal sur la sortie standard
             * il faut avoir appelé compute_costs() avant */
            void display_solution ();
            
            int get_min_cost () const;
            
        private:
            
            /* convertit (i,j) en coordonnée 1D pour le tableau
             * ne vérifie pas les valeurs de i et j */
            inline int index(int i, int j) const {
                return j*(_source.nb_lines()+1) + i;
            }
            
            /* remplit la ligne curr_line (ligne j), sachant la ligne j-1 */
            void compute_line (std::vector<State> &curr_line,
                               std::vector<State> &prev_line,
                               int j);
            
            /* met à jour les coordonnées de l'état */
            void update_coords (int &i, int &j, Op op) const;
    };
#endif

#endif
