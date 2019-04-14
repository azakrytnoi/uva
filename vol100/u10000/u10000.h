#pragma once

class U10000 {
public:
    static const char* libname()
    {
        return "u10000";
    }
    U10000() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10000> pu10000;
#endif
