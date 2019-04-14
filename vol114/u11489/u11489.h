#pragma once

class U11489 {
public:
    static const char* libname()
    {
        return "u11489";
    }
    U11489() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U11489> pu11489;
#endif
