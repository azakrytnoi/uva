#pragma once

class U10189 {
public:
    static const char* libname()
    {
        return "u10189";
    }
    U10189() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10189> pu10189;
#endif
