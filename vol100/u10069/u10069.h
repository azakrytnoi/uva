#pragma once

class U10069 {
public:
    static const char* libname()
    {
        return "u10069";
    }
    U10069() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10069> pu10069;
#endif
