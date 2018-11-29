#pragma once

class U11805 {
public:
    static const char* libname()
    {
        return "u11805";
    }
    U11805() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U11805> pu11805;
#endif
