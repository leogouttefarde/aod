#define OLD
#ifdef OLD
    #include "solver.hpp"

    #include <iostream>
    #include <fstream>
    #include <list>

    /* structure utilisée seulement dans display_solution */
    struct Solution {
        int _i, _j; // etat courant
        int  _op;// operation a faire quand on est dans l'etat courant
        
        Solution (int i=0, int j=0, int o=NONE):
            _i(i),
            _j(j),
            _op(o)
        {}
        
        void set (int i, int j, int o) {
            _i = i;
            _j = j;
            _op = o;
        }
    };

    Solver::Solver (const char *source_path, const char *target_path):
        _source (source_path),
        _target (target_path),
        
        _states ( (_source.nb_lines()+1) * (_target.nb_lines()+1) )
    {
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
        std::list<Solution> sol_list;
        Solution  solution;
        
        int i = _source.nb_lines(),
            j = _target.nb_lines();

        while (index(i,j) > 0) {
            int op = _states[index(i,j)].op;
            
            // on recule d'un cran dans le temps
            indices_from(i, j, op);
            if (op != NONE) {// si l'opération était utile, on la note
                solution.set(i, j, op);
                sol_list.push_front(solution);
            }
        }
        
        _target.restart();
        for (std::list<Solution>::iterator iSol = sol_list.begin() ;
                                            iSol != sol_list.end() ;
                                            ++iSol) {
            int i = iSol->_i,
                j = iSol->_j;
            
            switch (iSol->_op) {
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
                    if (iSol->_op == DEST+1) // destruction simple
                        std::cout << "d " << i+1 << '\n';
                    else
                        std::cout << "D " << i+1 << " "
                                          << iSol->_op - DEST << '\n';
                break;
            }
            
            indices_to (i, j, iSol->_op);
        }
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
#else
    #include "solver.hpp"

    #include <iostream>
    #include <fstream>
    #include <list>
    #include <algorithm>

    using namespace std;

    Solver::Solver (const char *source_path, const char *target_path):
        _source (source_path),
        _target (target_path),
        _patch ()
    {
        cerr << '\n' << _source.nb_lines() << " lignes en entree, "
                  << _target.nb_lines() <<  " lignes en sortie." << endl;
    }

    void Solver::display_solution () const {
        int i = 0, j = 0;
        for (list<Op>::const_iterator it = _patch.begin() ;
                                      it != _patch.end()  ;
                                      ++it) {
            switch (*it) {
                case NONE:
                break;
                case ADD:
                    cout << "+ " << i << '\n' << *_target.get_line(j+1);
                break;
                case SUB:
                    cout << "= " << i+1 << '\n' << *_target.get_line(j+1);
                break;
                default: //DEST
                    if (*it == DEST+1) // destruction simple
                        cout << "d " << i+1 << '\n';
                    else
                        cout << "D " << i+1 << " " << *it - DEST << '\n';
                break;
            }
            
            update_coords (i, j, *it);
         }
    }

    int Solver::get_min_cost () const {
        return _patch_cost;
    }

    void Solver::compute_costs (bool disp){
        /* allocation des deux lignes */
        vector<State> line1(_source.nb_lines()+1),
                      line2(_source.nb_lines()+1);
        
        /* initialisation de la ligne j=0 */
        line1[0].cost = 0;
        line1[1].cost = 10;
        line1[1].ops.push_back(DEST+1);
        for (int i = _source.nb_lines() ; i > 1 ; --i) {
            line1[i].cost = 15;
            line1[i].ops.push_back(DEST+i);
        }
        
        vector<State> *curr_line = &line1, *prev_line = &line2;
        
        for (int i = 0 ; i <= _source.nb_lines() ; ++i)
            cerr << (*curr_line)[i].cost << " ";
        cerr << std::endl;
             
        int step = _target.nb_lines() / 10; //pour l'affichage de l'avancement
        if (step == 0)
            step = 1;
        for (int j = 1 ; j <= _target.nb_lines() ; ++j) {
            std::swap (prev_line, curr_line);
            compute_line (*curr_line, *prev_line, j);
            
            for (int i = 0 ; i <= _source.nb_lines() ; ++i)
                cerr << (*curr_line)[i].cost << " ";
             cerr << std::endl;
             
            if (disp && j % step == 0) {
                int percentage = 10*(j+1) / (_target.nb_lines()) * 10;
                cerr << percentage << " %" << endl;
            }
        }
        
        _patch = (*curr_line)[_source.nb_lines()].ops;
        _patch_cost = (*curr_line)[_source.nb_lines()].cost;
    }

    void Solver::compute_line (vector<State> &curr_line, vector<State> &prev_line,
                               int j) {
        string const *tar_j = _target.get_line(j);
        
        _source.restart();
        int best_k;//utilisé pour la multidestruction
        for (int i = 0 ; i <= _source.nb_lines() ; ++i) {
            string const *src_i  = _source.get_line(i);
            
            /* ADD */
            Op op = ADD;
            int cost = 10 + tar_j->length() + prev_line[i].cost;
            
            if (i == 0) // si on ne peut faire qu'une addition, on  passe au i suiv.
                continue;
                
            /* SUB */
            int tmp = prev_line[i-1].cost;
            if (*src_i != *tar_j)
                tmp += 10 + tar_j->length();
            if (tmp < cost) { // si sub est meilleur que les precedents
                cost = tmp;
                
                if (cost == prev_line[i-1].cost)
                    op = NONE;
                else
                    op = SUB;
            }
            
            /* SIMPLE DESTRUCTION */
            tmp = 10 + curr_line[i-1].cost;
            if (tmp < cost) {  // si simple dest est meilleur que les precedents
                cost = tmp;
                op = DEST + 1;
            }
            
            if (i == 1)// si on ne peut pas faire de multidestruction, on teste pas
                continue;
           
            /* MULTIPLE DESTRUCTION */
            if (i == 2)// initialisation de best_k lors de la premiere multidest possible
                best_k = 2;
            if (15 + curr_line[i-2].cost < 15 + curr_line[i-best_k].cost)
                best_k = 2;
            tmp = 15 + curr_line[i-best_k].cost;
            if (tmp < cost) {
                cost = tmp;
                op = DEST + best_k;
            }
            best_k++;// comme i augmente, pour stabiliser i-best_k on incremente best_ks
            
            /* ici cost est le cout optimal et op l'operation a faire */
            curr_line[i].cost = cost;
            
            int di=0, dj=0;
            update_coords (di, dj, op);
            if (dj == 0) // si le meilleur cout vient d'une destruction
                curr_line[i].ops = curr_line[i-di].ops;
            else
                curr_line[i].ops = prev_line[i-di].ops;
            curr_line[i].ops.push_back(op);
        }
    }

    void Solver::update_coords (int &i, int &j, Op op) const {
        switch (op) {
            case NONE:
                i++;
                j++;
            break;
            case ADD:
                j++;
            break;
            case SUB:
                i++;
                j++;
            break;
            default: //DEST
                i += op - DEST;
            break;
        }
    }
#endif
