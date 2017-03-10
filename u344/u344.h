#pragma once

class U344 {
public:
    static const char* libname()
    {
        return "u344";
    }
    U344() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U344> pu344;
#endif
