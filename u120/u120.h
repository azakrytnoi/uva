#pragma once

class U120 {
public:
    static const char* libname()
    {
        return "u120";
    }
    U120() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U120> pu120;
#endif
