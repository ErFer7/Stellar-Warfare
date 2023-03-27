#ifndef debug_h
#define debug_h

#include <iostream>
#include "traits.h"

__BEGIN_API

class Debug
{
public:
    template<typename T>
    Debug & operator<<(T p) { std::cout << p; return *this; }

    struct Begl {};
    struct Err {};

    Debug & operator<<(const Begl & begl) { return *this; }

    Debug & operator<<(const Err & err) { _error = true; return *this; }
private:
    volatile bool _error;
public:
    static Begl begl;
    static Err error;
};

class Null_Debug
{
public:
    template<typename T>
    Null_Debug & operator<<(const T & o) { return *this; }

    template<typename T>
    Null_Debug & operator<<(const T * o) { return *this; }
};

template<bool debugged>
class Select_Debug: public Debug {};
template<>
class Select_Debug<false>: public Null_Debug {};

// Error
enum Debug_Error {ERR = 1};

template<typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)>
db(Debug_Error l)
{
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)>() << Debug::begl;
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)>() << Debug::error;
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)>();
}

template<typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::error)>
db(Debug_Error l)
{

    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::error)>() << Debug::begl;
    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::error)>() << Debug::error;
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::error)>();
}

// Warning
enum Debug_Warning {WRN = 2};

template<typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::warning)>
db(Debug_Warning l)
{
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::warning)>() << Debug::begl;
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::warning)>();
}

template<typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::warning)>
db(Debug_Warning l)
{
    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::warning)>() << Debug::begl;
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::warning)>();
}

// Info
enum Debug_Info {INF = 3};

template<typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::info)>
db(Debug_Info l)
{
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::info)>() << Debug::begl;
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::info)>();
}

template<typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::info)>
db(Debug_Info l)
{
    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::info)>() << Debug::begl;
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::info)>();
}

// Trace
enum Debug_Trace {TRC = 4};

template<typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::trace)>
db(Debug_Trace l)
{
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::trace)>() << Debug::begl;
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::trace)>();
}

template<typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::trace)>
db(Debug_Trace l)
{
    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::trace)>() << Debug::begl;
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::trace)>();
}

__END_API

#endif
