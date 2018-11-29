#pragma once

class U10141 {
public:
    static const char* libname()
    {
        return "u10141";
    }
    U10141() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10141> pu10141;
#endif
