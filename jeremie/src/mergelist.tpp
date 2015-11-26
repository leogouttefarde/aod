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
MergeList<T>& MergeList<T>::operator= (const MergeList<T> &other) {
    if (this != &other) {
        clear();
        this->_first_elt = other._first_elt;
        
        if (this->_first_elt != NULL)
            this->_first_elt->increase_nb_pred();
    }
    
    return *this;
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
void MergeList<T>::push_front (const T &elt)
{
    MergeElt<T> *tmp = new MergeElt<T> (elt);
    tmp->set_next (_first_elt);
    
    if (_first_elt != NULL)
        _first_elt->decrease_nb_pred();
    _first_elt = tmp;
}

template<typename T>
std::list<T>* MergeList<T>::get_list() const {
    std::list<T> *tab = new std::list<T>();
    
    MergeElt<T> const *elt = _first_elt;
    while (elt != NULL) {
        tab->push_back (elt->get_value());
        elt = elt->get_next();
    }
    
    return tab;
}