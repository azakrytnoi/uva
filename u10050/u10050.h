#pragma once

class U10050 {
public:
    static const char* libname()
    {
        return "u10050";
    }
    U10050() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10050> pu10050;
#endif
