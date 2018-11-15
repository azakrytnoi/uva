#pragma once

class U11159 {
public:
    static const char* libname()
    {
        return "u11159";
    }
    U11159() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U11159> pu11159;
#endif
