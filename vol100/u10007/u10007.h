#pragma once

class U10007 {
public:
    static const char* libname()
    {
        return "u10007";
    }
    U10007() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10007> pu10007;
#endif
