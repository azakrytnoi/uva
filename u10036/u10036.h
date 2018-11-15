#pragma once

class U10036 {
public:
    static const char* libname()
    {
        return "u10036";
    }
    U10036() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10036> pu10036;
#endif
