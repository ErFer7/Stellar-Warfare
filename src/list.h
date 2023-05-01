#ifndef list_h
#define list_h

#include "traits.h"
#include "debug.h"

__BEGIN_API

// List Element Rank (for ordered lists)
class List_Element_Rank
{
public:
    List_Element_Rank(int r = 0): _rank(r) {}

    operator int() const { return _rank; }

protected:
    int _rank;
};

// List Elements
namespace List_Elements
{
    typedef List_Element_Rank Rank;

    // Ordered List Element
    template<typename T, typename R = Rank>
    class Doubly_Linked_Ordered
    {
    public:
        typedef T Object_Type;
        typedef Rank Rank_Type;
        typedef Doubly_Linked_Ordered Element;

    public:
        Doubly_Linked_Ordered() { }
        Doubly_Linked_Ordered(const T * o,  const R & r = 0): _object(o), _rank(r), _prev(0), _next(0) {}

        T * object() const { return const_cast<T *>(_object); }

        Element * prev() const { return _prev; }
        Element * next() const { return _next; }
        void prev(Element * e) { _prev = e; }
        void next(Element * e) { _next = e; }

        const R & rank() const { return _rank; }
        void rank(const R & r) { _rank = r; }
        int promote(const R & n = 1) { _rank -= n; return _rank; }
        int demote(const R & n = 1) { _rank += n; return _rank; }

    private:
        const T * _object;
        R _rank;
        Element * _prev;
        Element * _next;
    };

}

// List Iterators
namespace List_Iterators
{
    // Forward Iterator (for singly linked lists)
    template<typename El>
    class Forward
    {
    private:
        typedef Forward<El> Iterator;

    public:
        typedef El Element;

    public:
        Forward(): _current(0) {}
        Forward(Element * e): _current(e) {}

        operator Element *() const { return _current; }

        Element & operator*() const { return *_current; }
        Element * operator->() const { return _current; }

        Iterator & operator++() { _current = _current->next(); return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++*this; return tmp; }

        bool operator==(const Iterator & i) const { return _current == i._current; }
        bool operator!=(const Iterator & i) const { return _current != i._current; }

    protected:
        Element * _current;
    };

    // Bidirectional Iterator (for doubly linked lists)
    template<typename El>
    class Bidirecional
    {
    private:
        typedef Bidirecional<El> Iterator;

    public:
        typedef El Element;

    public:
        Bidirecional(): _current(0) {}
        Bidirecional(Element * e): _current(e) {}

        operator Element *() const { return _current; }

        Element & operator*() const { return *_current; }
        Element * operator->() const { return _current; }

        Iterator & operator++() { _current = _current->next(); return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++*this; return tmp; }

        Iterator & operator--() { _current = _current->prev(); return *this; }
        Iterator operator--(int) { Iterator tmp = *this; --*this; return tmp; }

        bool operator==(const Iterator & i) const { return _current == i._current; }
        bool operator!=(const Iterator & i) const { return _current != i._current; }

    private:
        Element * _current;
    };
}

// Doubly-Linked List
template<typename T,
          typename El>
class List
{
public:
    typedef T Object_Type;
    typedef El Element;
    typedef List_Iterators::Bidirecional<El> Iterator;

public:
    List(): _size(0), _head(0), _tail(0) {}

    bool empty() const { return (_size == 0); }
    unsigned int size() const { return _size; }

    Element * head() { return _head; }
    Element * tail() { return _tail; }

    Iterator begin() { return Iterator(_head); }
    Iterator end() { return Iterator(0); }

    void insert(Element * e) { insert_tail(e); }

    void insert_head(Element * e) {
        db<Lists>(TRC) << "List::insert_head(e=" << e
                       << ") => {p=" << (e ? e->prev() : (void *) -1)
                       << ",o=" << (e ? e->object() : (void *) -1)
                       << ",n=" << (e ? e->next() : (void *) -1)
                       << "}\n";

        print_head();
        print_tail();

        if(empty())
            insert_first(e);
        else {
            e->prev(0);
            e->next(_head);
            _head->prev(e);
            _head = e;
            _size++;
        }

        print_head();
        print_tail();
    }

    void insert_tail(Element * e) {
        db<Lists>(TRC) << "List::insert_tail(e=" << e
                       << ") => {p=" << (e ? e->prev() : (void *) -1)
                       << ",o=" << (e ? e->object() : (void *) -1)
                       << ",n=" << (e ? e->next() : (void *) -1)
                       << "}\n";

        print_head();
        print_tail();

        if(empty())
            insert_first(e);
        else {
            _tail->next(e);
            e->prev(_tail);
            e->next(0);
            _tail = e;
            _size++;
        }

        print_head();
        print_tail();
    }

    Element * remove() { return remove_head(); }

    Element * remove(Element * e) {
        db<Lists>(TRC) << "List::remove(e=" << e
                       << ") => {p=" << (e ? e->prev() : (void *) -1)
                       << ",o=" << (e ? e->object() : (void *) -1)
                       << ",n=" << (e ? e->next() : (void *) -1)
                       << "}\n";

        print_head();
        print_tail();

        if(last())
            remove_last();
        else if(!e->prev())
            remove_head();
        else if(!e->next())
            remove_tail();
        else {
            e->prev()->next(e->next());
            e->next()->prev(e->prev());
            _size--;
        }

        print_head();
        print_tail();

        return e;
    }

    Element * remove_head() {
        db<Lists>(TRC) << "List::remove_head()\n";

        print_head();
        print_tail();

        if(empty())
            return 0;
        if(last())
            return remove_last();
        Element * e = _head;
        _head = _head->next();
        _head->prev(0);
        _size--;

        print_head();
        print_tail();

        return e;
    }

    Element * remove_tail() {
        db<Lists>(TRC) << "List::remove_tail()\n";

        print_head();
        print_tail();

        if(empty())
            return 0;
        if(last())
            return remove_last();
        Element * e = _tail;
        _tail = _tail->prev();
        _tail->next(0);
        _size--;

        print_head();
        print_tail();

        return e;
    }

    Element * remove(const Object_Type * obj) {
        Element * e = search(obj);
        if(e)
            return remove(e);
        return 0;
    }

    Element * search(const Object_Type * obj) {
        Element * e = _head;
        for(; e && (e->object() != obj); e = e->next());
        return e;
    }

protected:
    bool last() const { return (_size == 1); }

    void insert(Element * e, Element * p,  Element * n) {
        db<Lists>(TRC) << "List::insert(e=" << e << ",p=" << p << ",n=" << n
                       << ") => {p=" << (e ? e->prev() : (void *) -1)
                       << ",o=" << (e ? e->object() : (void *) -1)
                       << ",n=" << (e ? e->next() : (void *) -1)
                       << "},{p=" << (p ? p->prev() : (void *) -1)
                       << ",o=" << (p ? p->object() : (void *) -1)
                       << ",n=" << (p ? p->next() : (void *) -1)
                       << "},{p=" << (n ? n->prev() : (void *) -1)
                       << ",o=" << (n ? n->object() : (void *) -1)
                       << ",n=" << (n ? n->next() : (void *) -1)
                       << "}\n";

        print_head();
        print_tail();

        p->next(e);
        n->prev(e);
        e->prev(p);
        e->next(n);
        _size++;

        print_head();
        print_tail();
    }

    void insert_first(Element * e) {
        db<Lists>(TRC) << "List::insert_first(e=" << e
                       << ") => {p=" << (e ? e->prev() : (void *) -1)
                       << ",o=" << (e ? e->object() : (void *) -1)
                       << ",n=" << (e ? e->next() : (void *) -1)
                       << "}\n";

        print_head();
        print_tail();

        e->prev(0);
        e->next(0);
        _head = e;
        _tail = e;
        _size++;

        print_head();
        print_tail();
    }

    Element * remove_last() {
        db<Lists>(TRC) << "List::remove_last()\n";

        print_head();
        print_tail();

        Element * e = _head;
        _head = 0;
        _tail = 0;
        _size--;

        print_head();
        print_tail();

        return e;
    }

    void print_head() {
        db<Lists>(INF) << "List[" << this << "]::head=" << head()
                       << " => {p=" << (head() ? head()->prev() : (void *) -1)
                       << ",o=" << (head() ? head()->object() : (void *) -1)
                       << ",n=" << (head() ? head()->next() : (void *) -1)
                       << "}\n";
    }

    void print_tail() {
        db<Lists>(INF) << "List[" << this << "]::tail=" << tail()
                       << " => {p=" << (tail() ? tail()->prev() : (void *) -1)
                       << ",o=" << (tail() ? tail()->object() : (void *) -1)
                       << ",n=" << (tail() ? tail()->next() : (void *) -1)
                       << "}\n";
    }

private:
    unsigned int _size;
    Element * _head;
    Element * _tail;
};

// Doubly-Linked, Ordered List
template<typename T,
          typename R = List_Element_Rank,
          typename El = List_Elements::Doubly_Linked_Ordered<T, R>,
          bool relative = false>
class Ordered_List: public List<T, El>
{
private:
    typedef List<T, El> Base;

public:
    typedef T Object_Type;
    typedef R Rank_Type;
    typedef El Element;
    typedef List_Iterators::Bidirecional<El> Iterator;

public:
    using Base::empty;
    using Base::size;
    using Base::head;
    using Base::tail;
    using Base::begin;
    using Base::end;
    using Base::insert_first;
    using Base::insert_head;
    using Base::insert_tail;
    using Base::print_head;
    using Base::search;

    void insert(Element * e) {
        db<Lists>(TRC) << "Ordered_List::insert(e=" << e
                       << ") => {p=" << (e ? e->prev() : (void *) -1)
                       << ",o=" << (e ? e->object() : (void *) -1)
                       << ",n=" << (e ? e->next() : (void *) -1)
                       << "}\n";

        if(empty())
            insert_first(e);
        else {
            Element * next;
            for(next = head();
                (next->rank() <= e->rank()) && next->next();
                next = next->next())
                if(relative)
                    e->rank(e->rank() - next->rank());
            if(next->rank() <= e->rank()) {
                if(relative)
                    e->rank(e->rank() - next->rank());
                insert_tail(e);
            } else if(!next->prev()) {
                if(relative)
                    next->rank(next->rank() - e->rank());
                insert_head(e);
            } else {
                if(relative)
                    next->rank(next->rank() - e->rank());
                Base::insert(e, next->prev(), next);
            }
        }
    }

    Element * remove() {
        db<Lists>(TRC) << "Ordered_List::remove()\n";
        Element * e = Base::remove_head();
        if(e && relative && e->next())
            e->next()->rank(e->next()->rank() + e->rank());
        return e;
    }

    Element * remove(Element * e) {
        db<Lists>(TRC) << "Ordered_List::remove(e=" << e
                       << ") => {p=" << (e ? e->prev() : (void *) -1)
                       << ",o=" << (e ? e->object() : (void *) -1)
                       << ",n=" << (e ? e->next() : (void *) -1)
                       << "}\n";

        Base::remove(e);
        if(relative && e->next())
            e->next()->rank(e->next()->rank() + e->rank());

        return e;
    }

    Element * remove(const Object_Type * obj) {
        db<Lists>(TRC) << "Ordered_List::remove(o=" << obj << ")\n";

        Element * e = search(obj);
        if(e)
            return remove(e);
        else
            return 0;
    }
};

__END_API

#endif
