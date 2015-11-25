#ifndef MERGELIST_HPP__
#define MERGELIST_HPP__

#include "mergeelt.hpp"

#include <list>

template<typename T>
class MergeList {
    private:
        MergeElt<T> *_first_elt;

    public:
        MergeList ();
        ~MergeList ();
        
        MergeList<T>& operator= (const MergeList<T> &other);

        /* insertion en début de liste */
        void push_front (const T &elt);
        void push_front (const MergeList<T> &list);

        void clear ();
        
        std::list<T>* get_list () const;
};

#include "mergelist.tpp"

#endif
