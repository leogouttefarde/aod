#include "solver.hpp"

#include <iostream>
#include <fstream>

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
//CHECK TAILLES
    std::cerr << _source.nb_lines() << " lignes en entree, "
              << _target.nb_lines() <<  " lignes en sortie." << std::endl;
    
    compute_costs();
    
    std::cerr << "Cout total : " << get_cost(_source.nb_lines(), _target.nb_lines()) << std::endl;
}

Solver::~Solver (){
}

void Solver::display () {
    for (int j = _target.nb_lines() ; j >= 0 ; j--) {
        for (int i = 0 ; i <= _source.nb_lines() ; i++)
            std::cerr << _states[index(i,j)].op << " ";//get_cost(i,j) << " ";
        std::cerr << std::endl;
    }
}

void Solver::display_solution () {
    std::cerr << "Ecriture du patch..." << std::endl;

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
    
    for (int iSol = sol.size()-1 ; iSol >= 0 ; iSol--) {
        int i = sol[iSol]->_i,
            j = sol[iSol]->_j;
        
        
        switch (sol[iSol]->_op) {
            case NONE:
                std::cout << "albert";
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

int Solver::get_cost (int i, int j) {
    return _states[index(i,j)].cost;
}

void Solver::compute_costs (){
    std::cerr << "Calcul des couts..." << std::endl;
    
    /* conditions aux bords */
    _states[index(0,0)].cost = 0;
    
    _states[index(1,0)].cost = 10;
    _states[index(1,0)].op = DEST+1;
    for (int i = 2 ; i <= _source.nb_lines() ; i++) {
        _states[index(i,0)].cost = 15;
        _states[index(i,0)].op = DEST + i;
    }
    
    for (int j = 1 ; j <= _target.nb_lines() ; j++) {
        _states[index(0,j)].cost = 10 + _target.get_line(j)->length()
                                      + get_cost(0,j-1);
        _states[index(0,j)].op = ADD;
    }
    _target.restart();
    
    int step = _target.nb_lines() / 10; //pour l'affichage de l'avancement
    if (step == 0)
        step = 1;
    for (int i = 1 ; i <= _source.nb_lines() ; i++){
        for (int j = 1 ; j <= _target.nb_lines() ; j++)
            compute_state(i, j);
        
        _target.restart();
        if (i % step == 0) {
            int percentage = 100*i / (_target.nb_lines());
            std::cerr << percentage << " %" << std::endl;
        }
    }

    std::cerr << "Calcul termine !" << std::endl;
}

void Solver::compute_state (int i, int j) {
    //std::cerr << "(" << i << " ; " << j << ")" << std::endl;
    std::string const *src_i  = _source.get_line(i),
                      *tar_j  = _target.get_line(j);
    
    /* ADD */
    int cost = 10 + tar_j->length() + get_cost(i,j-1);
    _states[index(i,j)].cost = cost;
    _states[index(i,j)].op = ADD;
    
    /* SUB */
    int tmp = get_cost(i-1,j-1);
    if (*src_i != *tar_j)
        tmp += tar_j->length();
    if (tmp < cost) {
        cost = tmp;
        _states[index(i,j)].cost = cost;
        if (cost == get_cost(i-1,j-1))
            _states[index(i,j)].op = NONE;
        else
            _states[index(i,j)].op = SUB;
    }
    
    /* SIMPLE DESTRUCTION */
    tmp = 10 + get_cost(i-1,j);
    if (tmp < cost) {
        cost = tmp;
        _states[index(i,j)].cost = cost;
        _states[index(i,j)].op = DEST + 1;
    }
    
    /* MULTIPLE DESTRUCTION */
    if (i > 1) {
        int best_k = 2;
        tmp = 15 + get_cost(i-2,j);
        for (int k = 3 ; k <= i ; k++) {
            int tmp2 = 15 + get_cost(i-k,j);
            if (tmp2 <= tmp) {
                tmp = tmp2;
                best_k = k;
            }
        }
        if (tmp < cost) {
            cost = tmp;
            _states[index(i,j)].cost = cost;
            _states[index(i,j)].op = DEST + best_k;
        }
    }
}

void Solver::indices_from (int &i, int &j, int op) {
    if (op == NONE) {
        i--;
        j--;
    } else if (op == ADD) {
        j--;
    } else if (op == SUB) {
        i--;
        j--;
    } else {// DEST
        i -= op - DEST;
    }
}

void Solver::indices_to (int &i, int &j, int op) {
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
