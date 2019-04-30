#pragma once

class U10043 {
public:
    static const char* libname()
    {
        return "u10043";
    }
    U10043() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10043> pu10043;
#endif
