#pragma once

class U11716 {
public:
    static const char* libname()
    {
        return "u11716";
    }
    U11716() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U11716> pu11716;
#endif
