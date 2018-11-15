#pragma once

class U10424 {
public:
    static const char* libname()
    {
        return "u10424";
    }
    U10424() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10424> pu10424;
#endif
