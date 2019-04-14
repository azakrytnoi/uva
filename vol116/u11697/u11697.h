#pragma once

class U11697 {
public:
    static const char* libname()
    {
        return "u11697";
    }
    U11697() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U11697> pu11697;
#endif
