#pragma once

class U11172 {
public:
    static const char* libname()
    {
        return "u11172";
    }
    U11172() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U11172> pu11172;
#endif
