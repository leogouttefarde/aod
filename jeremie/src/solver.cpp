#include "solver.hpp"

#include <iostream>
#include <fstream>

/* structure utilisée seulement dans display_solution */
struct Solution {
    int _i, _j;
    int  _op;
    
    Solution (int i, int j, int o):
        _i(i),
        _j(j),
        _op(o)
    {}
};

Solver::Solver (const char *source_path, const char *target_path):
    _source (source_path),
    _target (target_path),
    
    _states ( (_source.nb_lines()+1) * (_target.nb_lines()+1) )
{
//CHECK TAILLES ?
    std::cerr << std::endl;
    std::cerr << _source.nb_lines() << " lignes en entree, "
              << _target.nb_lines() <<  " lignes en sortie." << std::endl;
}

void Solver::display () const {
    for (int j = _target.nb_lines() ; j >= 0 ; j--) {
        for (int i = 0 ; i <= _source.nb_lines() ; i++)
            std::cerr << _states[index(i,j)].cost << " ";//get_cost(i,j) << " ";
        std::cerr << std::endl;
    }
}

void Solver::display_solution () {
    std::vector<Solution*> sol;

    int i = _source.nb_lines(),
        j = _target.nb_lines();

    while (index(i,j) > 0) {
        int op = _states[index(i,j)].op;
        
        // on recule d'un cran dans le temps
        indices_from(i, j, op);
        if (op != NONE) // si l'opération était utile, on la note
            sol.push_back(new Solution(i, j, op));
    }
    
    _target.restart();
    for (int iSol = sol.size()-1 ; iSol >= 0 ; iSol--) {
        int i = sol[iSol]->_i,
            j = sol[iSol]->_j;
        
        
        switch (sol[iSol]->_op) {
            case NONE:
            break;
            case ADD:
                std::cout << "+ " << i << '\n' << *_target.get_line(j+1);
            break;
            case SUB:
                std::cout << "= " << i+1 << '\n';
                std::cout << *_target.get_line(j+1);
            break;
            default: //DEST
                if (sol[iSol]->_op == DEST+1) // destruction simple
                    std::cout << "d " << i+1 << '\n';
                else
                    std::cout << "D " << i+1 << " " << sol[iSol]->_op - DEST << '\n';
            break;
        }
        
        indices_to (i, j, sol[iSol]->_op);
    }
    
    for (int i = sol.size()-1 ; i >= 0 ; i--)
        delete sol[i];
}

int Solver::get_min_cost () const {
    return get_cost(_source.nb_lines(), _target.nb_lines());
}

void Solver::compute_costs (bool disp){
    //conditions aux bords
    compute_sides ();
    
    //calcul du centre
    int step = _target.nb_lines() / 10; //pour l'affichage de l'avancement
    if (step == 0)
        step = 1;
    for (int j = 1 ; j <= _target.nb_lines() ; ++j) {
            compute_line(j);
            
        if (disp && j % step == 0) {
            int percentage = 100*j / (_target.nb_lines());
            std::cerr << percentage << " %" << std::endl;
        }
    }
}

void Solver::compute_line (int j) {
    int best_k;//utilisé pour la multidestruction
    
    std::string const *tar_j = _target.get_line(j);
    _source.restart();
    for (int i = 1 ; i <= _source.nb_lines() ; ++i) {
        std::string const *src_i  = _source.get_line(i);
                          
        /* ADD */
        int cost = 10 + tar_j->length() + get_cost(i,j-1);
        _states[index(i,j)].cost = cost;
        _states[index(i,j)].op = ADD;
        
        /* SUB */
        int tmp = get_cost(i-1,j-1);
        if (*src_i != *tar_j)
            tmp += 10 + tar_j->length();
        if (tmp < cost) {
            cost = tmp;
            _states[index(i,j)].cost = cost;
            if (cost == get_cost(i-1,j-1))
                _states[index(i,j)].op = NONE;
            else
                _states[index(i,j)].op = SUB;
        }
        
         // si on ne peut rien détruire, on ne teste pas
        if (i > 0) {
            /* SIMPLE DESTRUCTION */
            tmp = 10 + get_cost(i-1,j);
            if (tmp < cost) {
                cost = tmp;
                _states[index(i,j)].cost = cost;
                _states[index(i,j)].op = DEST + 1;
            }
            
            // si on ne peut pas faire de multidestruction, on teste pas
            if (i > 1) {
                /* MULTIPLE DESTRUCTION */
                if (i == 2)// initialisation de best_k lors de la premiere multidest possible
                    best_k = 2;
                tmp = 15 + get_cost(i-best_k,j);
                if (15 + get_cost(i-2,j) < tmp) {
                    best_k = 2;
                    tmp = 15 + get_cost(i-2,j);
                }
                if (tmp < cost) {
                    cost = tmp;
                    _states[index(i,j)].cost = cost;
                    _states[index(i,j)].op = DEST + best_k;
                }
                
                best_k++;// comme i augmente, pour stabiliser i-best_k on incremente best_k
            }
        }
    }
}

void Solver::compute_sides () {
    _states[index(0,0)].cost = 0;
    
    _states[index(1,0)].cost = 10;
    _states[index(1,0)].op = DEST+1;
    for (int i = 2 ; i <= _source.nb_lines() ; i++) {
        _states[index(i,0)].cost = 15;
        _states[index(i,0)].op = DEST + i;
    }
    
    _target.restart();
    for (int j = 1 ; j <= _target.nb_lines() ; j++) {
        _states[index(0,j)].cost = 10 + _target.get_line(j)->length()
                                      + get_cost(0,j-1);
        _states[index(0,j)].op = ADD;
    }
    _target.restart();
}

void Solver::indices_from (int &i, int &j, int op) const {
    int di = 0, dj = 0;
    indices_to(di, dj, op);

    i -= di;
    j -= dj;
}

void Solver::indices_to (int &i, int &j, int op) const {
    if (op == NONE) {
        i++;
        j++;
    } else if (op == ADD) {
        j++;
    } else if (op == SUB) {
        i++;
        j++;
    } else {// DEST
        i += op - DEST;
    }
}
