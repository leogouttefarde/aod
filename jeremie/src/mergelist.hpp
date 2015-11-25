#ifndef MERGELIST_HPP__
#define MERGELIST_HPP__

#include "mergeelt.hpp"

#include <vector>

template<typename T>
class MergeList {
    private:
        MergeElt<T> *_first_elt;
    public:
        MergeList();
        ~MergeList();
        
        /* insertion en début de liste */
        void insert(T elt);
        
        void clear ();
        
        std::vector<T>* get_elt_vect();
};

template<typename T>
MergeList<T>::MergeList ():
    _first_elt (NULL)
{
}

template<typename T>
MergeList<T>::~MergeList ()
{
    clear();
}

template<typename T>
void MergeList<T>::clear () {
    if (_first_elt != NULL) {
        _first_elt->decrease_nb_pred();
        
        if (_first_elt->get_nb_pred() <= 0)
            delete _first_elt;
        
        _first_elt = NULL;
    }
}

template<typename T>
void MergeList<T>::insert (T elt)
{
    MergeElt<T> *tmp = new MergeElt<T> (elt);
    tmp->set_next (_first_elt);
    
    if (_first_elt == NULL)
        _first_elt = tmp;
}

template<typename T>
std::vector<T>* MergeList<T>::get_elt_vect() {
    std::vector<T> *tab = new std::vector<T>();
    
    MergeElt<T> const *elt = _first_elt;
    while (elt != NULL) {
        tab->push_back (elt->get_value());
        elt = elt->get_next();
    }
    
    return tab;
}

#include "mergelist.tpp"

#endif
