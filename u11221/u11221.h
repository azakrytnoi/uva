#pragma once

class U11221 {
public:
    static const char* libname()
    {
        return "u11221";
    }
    U11221() {}

    void operator()() const;
};
#ifdef POPULATE_CACHE
    populate <U11221> pu11221;
#endif

