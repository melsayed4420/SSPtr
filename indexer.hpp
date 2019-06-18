/*
 * SSPtr, (Safe-Sturdy Pointer)
 *      Author : Mohamed ElSayed
 *      m.elsayed4420@gmail.com
 *      llcode.wordpress.com
 */


#include "exc.hpp"

template <class T> class Index {
    T *ptr;
    T *last;
    T *first;
    unsigned length;

public:
    Index():
        ptr(nullptr), last(nullptr), first(nullptr), length(0) {}

    Index(T *p, T *first, T *last):
        ptr(p), first(first), last(last), length( last - first) {}

    unsigned size() { return length;}

    T &operator *() const {
        if((ptr >= last) || (ptr < first))
            throw Exc();
        return *ptr;
    }

    T *operator ->() const {
        if((ptr >= last) || (ptr < first))
            throw Exc();
        return ptr;
    }

    Index operator --(int) {
        T *po_tmp = ptr--;
        return Index<T>(po_tmp, first, last);
    }

    Index operator ++(int) {
        T *po_tmp = ptr++;
        return Index<T>(po_tmp, first, last);
    }

    T &operator[](int i) const {
        if( (i < 0 ) || (i > length) )
            throw Exc(i, last-first);
        return ptr[i];
    }

    bool operator==(Index arg)  {
        return ptr == arg.ptr;
    }

    bool operator!=(Index arg)  {
        return ptr != arg.ptr;
    }

    bool operator<(Index arg) {
        return ptr < arg.ptr;
    }

    bool operator>(Index arg)  {
        return ptr > arg.ptr;
    }

    bool operator<=(Index arg)  {
        return ptr <= arg.ptr;
    }

    bool operator>=(Index arg)  {
        return ptr >= arg.ptr;
    }

    Index &operator-(int n) {
        ptr -= n;
        return *this;
    }

    Index &operator+(int n) {
        ptr += n;
        return *this;
    }

    Index &operator-=(int n) {
        ptr -= n;
        return *this;
    }

    Index &operator+=(int n) {
        ptr += n;
        return *this;
    }

};
