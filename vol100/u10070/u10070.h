#pragma once

class U10070 {
public:
    static const char* libname()
    {
        return "u10070";
    }
    U10070() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10070> pu10070;
#endif
