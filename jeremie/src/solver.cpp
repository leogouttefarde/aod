#include "solver.hpp"
#include <iostream>
#include <list>
#include <stdlib.h>

using namespace std;


Solver::Solver (const string& src_path, const string& tar_path):
    _source (src_path),
    _target (tar_path),
    _states ()
{
    _states.resize( (_source.nb_lines()+1) * (_target.nb_lines()+1) );

    cerr << endl << _source.nb_lines() << " lignes en entree, "
              << _target.nb_lines() <<  " lignes en sortie." << endl;
}

void Solver::display () const {
    for (unsigned int j = 0 ; j <= _target.nb_lines() ; ++j) {
        for (unsigned int i = 0 ; i <= _source.nb_lines() ; ++j)
            cerr << _states[index(i,j)].cost << " ";
        
        cerr << endl;
    }
    cerr << endl;
}

void Solver::display_solution () {
    list<Op> op_list;
    
    /* first we begin at the desired state
     * and we make our way to the state (0,0)
     * storing every operation we go through */
    int i = _source.nb_lines(),
        j = _target.nb_lines();
    while (i != 0 || j != 0) {
        Op op = _states[index(i,j)].op;
        op_list.push_front(op);
        
        int di = 0, dj = 0;
        next_indices(di, dj, op);
        i -= di;
        j -= dj;
    }
    
    /* then we begin from (0,0) and apply
     * the operations in the right order */
    for (list<Op>::iterator it = op_list.begin() ;
                                 it != op_list.end() ;
                                 ++it) {
        switch (*it) {
            case NONE:
            break;
            case ADD:
                cout << "+ " << i << endl << *_target.get_line(j+1);
            break;
            case SUB:
                cout << "= " << i+1 << endl << *_target.get_line(j+1);
            break;
            default: //DEST
                if (*it == DEST+1) // destruction simple
                    cout << "d " << i+1 << endl;
                else
                    cout << "D " << i+1 << " " << *it - DEST << endl;
            break;
        }
        
        next_indices (i, j, *it);
    }
}

int Solver::get_min_cost () const {
    return get_cost(_source.nb_lines(), _target.nb_lines());
}

void Solver::compute_costs (bool disp) {
    //conditions aux bords
    compute_sides ();
    
    //calcul du centre
    int step = _target.nb_lines() / 10; //for progression displaying
    if (step == 0)
        step = 1;
    for (unsigned int j = 1 ; j <= _target.nb_lines() ; ++j) {
            compute_line(j);
            
        if (disp && j % step == 0) {
            int percentage = 100*j / (_target.nb_lines());
            cerr << percentage << " %" << endl;
        }
    }
}

void Solver::compute_line (int j) {
    int best_k = 0;//utilisé pour la multidestruction
    
    string const *tar_j = _target.get_line(j);
    for (unsigned int i = 1 ; i <= _source.nb_lines() ; ++i) {
        string const *src_i  = _source.get_line(i);
        
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
        
        /* SIMPLE DESTRUCTION */
        tmp = 10 + get_cost(i-1,j);
        if (tmp < cost) {
            cost = tmp;
            _states[index(i,j)].cost = cost;
            _states[index(i,j)].op = (Op)(DEST + 1);
        }
        
        /* si on ne peut pas faire de multidestruction, on ne teste pas */
        if (i == 1)
            continue;
            
        /* MULTIPLE DESTRUCTION */
        if (i == 2)
            best_k = 2;
        tmp = 15 + get_cost(i-best_k, j);
        if (15 + get_cost(i-2,j) < tmp) {
            best_k = 2;
            tmp = 15 + get_cost(i-2,j);
        }
        if (tmp < cost) {
            cost = tmp;
            _states[index(i,j)].cost = cost;
            _states[index(i,j)].op = (Op)(DEST + best_k);
        }
        
        best_k++;/* comme i augmente, pour stabiliser i-best_k
                  * on incremente best_k */
    }
}

void Solver::compute_sides () {
    _states[index(0,0)].cost = 0;
    
    _states[index(1,0)].cost = 10;
    _states[index(1,0)].op = (Op)(DEST + 1);
    for (unsigned int i = 2 ; i <= _source.nb_lines() ; ++i) {
        _states[index(i,0)].cost = 15;
        _states[index(i,0)].op = (Op)(DEST + i);
    }
    
    for (unsigned int j = 1 ; j <= _target.nb_lines() ; ++j) {
        _states[index(0,j)].cost = 10 + _target.get_line(j)->length()
                                      + get_cost(0,j-1);
        _states[index(0,j)].op = ADD;
    }
}

void Solver::next_indices (int &i, int &j, Op op) const {
    switch (op) {
        case NONE:
            i++;
            j++;
        break;
        case ADD:
            j++;
        break;;
        case SUB:
            i++;
            j++;
        break;
        default: //DESTRUCTION
            i += op - DEST;
        break;
    }
}

