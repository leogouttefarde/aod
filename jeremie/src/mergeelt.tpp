template<typename T>
MergeElt<T>::MergeElt (T elt):
    _elt(elt),
    _next(NULL),
    _nb_pred(1)
{
}

template<typename T>
MergeElt<T>::~MergeElt ()
{
    detach_from_next ();
}

template<typename T>
T const& MergeElt<T>::get_value () const {
    return _elt;
}

template<typename T>
int MergeElt<T>::get_nb_pred () const {
    return _nb_pred;
}

template<typename T>
MergeElt<T> const* MergeElt<T>::get_next () const {
    return _next;
}

template<typename T>
void MergeElt<T>::set_next (MergeElt<T> *next)
{
    if (next == NULL)
        return;
    
    detach_from_next ();
    
    next->_nb_pred++;
    
    _next = next;
}

template<typename T>
void MergeElt<T>::decrease_nb_pred ()
{
    _nb_pred--;
}

template<typename T>
void MergeElt<T>::increase_nb_pred ()
{
    _nb_pred++;
}

template<typename T>
void MergeElt<T>::detach_from_next () {
    if (_next != NULL) {
        _next->decrease_nb_pred();
        
        if (_next->_nb_pred <= 0)
            delete _next;
        
        _next = NULL;
    }
}
