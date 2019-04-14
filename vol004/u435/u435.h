#pragma once

class U435 {
public:
    static const char* libname()
    {
        return "u435";
    }
    U435() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U435> pu435;
#endif
