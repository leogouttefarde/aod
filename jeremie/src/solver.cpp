#define OLD
#ifdef OLD
    #include "solver.hpp"

    #include <iostream>
    #include <list>

    Solver::Solver (const std::string &src_path, const std::string &tar_path):
        _source (src_path),
        _target (tar_path),
        _states ( (_source.nb_lines()+1) * (_target.nb_lines()+1) )
    {
        std::cerr << '\n' << _source.nb_lines() << " lignes en entree, "
                  << _target.nb_lines() <<  " lignes en sortie." << std::endl;
    }

    void Solver::display () const {
        for (unsigned int j = 0 ; j <= _target.nb_lines() ; ++j) {
            for (unsigned int i = 0 ; i <= _source.nb_lines() ; ++j)
                std::cerr << _states[index(i,j)].cost << " ";
            
            std::cerr << "\n";
        }
        std::cerr << std::endl;
    }

    void Solver::display_solution () {
        std::list<Op> op_list;
        
        /* first we begin at the desired state
         * and we make our way to the state (0,0)
         * storing every operation we go through */
        int i = _source.nb_lines(),
            j = _target.nb_lines();
        while (i != 0 || j != 0) {
            int op = _states[index(i,j)].op;
            op_list.push_front(op);
            
            int di = 0, dj = 0;
            next_indices(di, dj, op);
            i -= di;
            j -= dj;
        }
        
        /* then we begin from (0,0) and apply
         * the operations in the right order */
        for (std::list<Op>::iterator it = op_list.begin() ;
                                     it != op_list.end() ;
                                     ++it) {
            switch (*it) {
                case NONE:
                break;
                case ADD:
                    std::cout << "+ " << i << '\n' << *_target.get_line(j+1);
                break;
                case SUB:
                    std::cout << "= " << i+1 << '\n' << *_target.get_line(j+1);
                break;
                default: //DEST
                    if (*it == DEST+1) // destruction simple
                        std::cout << "d " << i+1 << '\n';
                    else
                        std::cout << "D " << i+1 << " " << *it - DEST << '\n';
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
                std::cerr << percentage << " %" << std::endl;
            }
        }
    }

    void Solver::compute_line (int j) {
        int best_k;//utilisé pour la multidestruction
        
        std::string const *tar_j = _target.get_line(j);
        for (unsigned int i = 1 ; i <= _source.nb_lines() ; ++i) {
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
            
            /* SIMPLE DESTRUCTION */
            tmp = 10 + get_cost(i-1,j);
            if (tmp < cost) {
                cost = tmp;
                _states[index(i,j)].cost = cost;
                _states[index(i,j)].op = DEST + 1;
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
                _states[index(i,j)].op = DEST + best_k;
            }
            
            best_k++;/* comme i augmente, pour stabiliser i-best_k
                      * on incremente best_k */
        }
    }

    void Solver::compute_sides () {
        _states[index(0,0)].cost = 0;
        
        _states[index(1,0)].cost = 10;
        _states[index(1,0)].op = DEST+1;
        for (unsigned int i = 2 ; i <= _source.nb_lines() ; ++i) {
            _states[index(i,0)].cost = 15;
            _states[index(i,0)].op = DEST + i;
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
        _patch (NULL)
    {
        cerr << '\n' << _source.nb_lines() << " lignes en entree, "
                  << _target.nb_lines() <<  " lignes en sortie." << endl;
    }

    Solver::~Solver () {
        delete _patch;
    }

    void Solver::display_solution () {
        int i = 0, j = 0;
        
        for (list<Op>::reverse_iterator it = _patch->rbegin() ; it != _patch->rend() ; ++it) {
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
        line1[1].ops.push_front(DEST+1);
        for (int i = _source.nb_lines() ; i > 1 ; --i) {
            line1[i].cost = 15;
            line1[i].ops.push_front(DEST+i);
        }
        
        vector<State> *curr_line = &line1, *prev_line = &line2;
        
        int step = _target.nb_lines() / 10; //pour l'affichage de l'avancement
        if (step == 0)
            step = 1;
        for (unsigned int j = 1 ; j <= _target.nb_lines() ; ++j) {
            std::swap (prev_line, curr_line);
            compute_line (*curr_line, *prev_line, j);
             
            if (disp && j % step == 0) {
                int percentage = 10*(j+1) / (_target.nb_lines()) * 10;
                cerr << percentage << " %" << endl;
            }
        }
        
        _patch = (*curr_line)[_source.nb_lines()].ops.get_list();
        _patch_cost = (*curr_line)[_source.nb_lines()].cost;
    }

    void Solver::compute_line (vector<State> &curr_line, vector<State> &prev_line,
                               int j) {
        string const *tar_j = _target.get_line(j);

        int best_k;//utilisé pour la multidestruction
        for (unsigned int i = 0 ; i <= _source.nb_lines() ; ++i) {
            /* ADD */
            Op op = ADD;
            int cost = 10 + tar_j->length() + prev_line[i].cost;
            
            if (i > 0) {//si on peut faire autre chose qu'un ajout
                string const *src_i  = _source.get_line(i);
                
                /* SUB */
                int tmp = prev_line[i-1].cost;
                if (*src_i != *tar_j)
                    tmp += 10 + tar_j->length();
                if (tmp < cost) { // si sub est meilleur que les precedents
                    cost = tmp;
                    op = (cost == prev_line[i-1].cost) ? NONE : SUB;
                }
                
                /* SIMPLE DESTRUCTION */
                tmp = 10 + curr_line[i-1].cost;
                if (tmp < cost) {  // si simple dest est meilleur que les precedents
                    cost = tmp;
                    op = DEST + 1;
                }
                
                if (i > 1) {// si on peut faire une multi destruction
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
                }
            }
            
            /* ici cost est le cout optimal et op l'operation a faire */
            curr_line[i].cost = cost;
            
            int di=0, dj=0;
            update_coords (di, dj, op);
            if (dj == 0) // si le meilleur cout vient d'une destruction
                curr_line[i].ops = curr_line[i-di].ops;
            else
                curr_line[i].ops = prev_line[i-di].ops;
            curr_line[i].ops.push_front(op);
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
