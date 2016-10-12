#pragma once

class U406
{
public:
    static const char * libname()
    {
        return "u406";
    }
    U406() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U406> pu406;
#endif
