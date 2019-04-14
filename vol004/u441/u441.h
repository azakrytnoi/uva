#pragma once

class U441 {
public:
    static const char* libname()
    {
        return "u441";
    }
    U441() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U441> pu441;
#endif
