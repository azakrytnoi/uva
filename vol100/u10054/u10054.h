#pragma once

class U10054 {
public:
    static const char* libname()
    {
        return "u10054";
    }
    U10054() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10054> pu10054;
#endif
