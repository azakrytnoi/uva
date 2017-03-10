#pragma once

class U10896 {
public:
    static const char* libname()
    {
        return "u10896";
    }
    U10896() {}

    void operator()() const;
};
#ifdef POPULATE_CACHE
    populate <U10896> pu10896;
#endif

