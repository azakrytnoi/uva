#pragma once

class U10065 {
public:
    static const char* libname()
    {
        return "u10065";
    }
    U10065() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10065> pu10065;
#endif
