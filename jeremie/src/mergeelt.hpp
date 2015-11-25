#ifndef MERGEELT_HPP__
#define MERGEELT_HPP__

template<typename T>
class MergeElt {
    private:
        T        _elt;
        MergeElt *_next;
        int      _nb_pred;
    
    public:
        MergeElt (T elt);
        ~MergeElt ();
        
        T const& get_value () const;
        int get_nb_pred () const;
        MergeElt const * get_next () const;
        
        void set_next (MergeElt<T> *next);
        
        void increase_nb_pred ();
        void decrease_nb_pred ();
    
    private:
        void detach_from_next ();
};

#include "mergeelt.tpp"

#endif
